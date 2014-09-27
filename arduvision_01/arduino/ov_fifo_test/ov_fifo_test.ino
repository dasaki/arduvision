/*********************************************
 *
 *   Part of the ARDUVISION project
 *
 *   by David Sanz Kirbis
 *
 *  More info: http://www.arduvision.com
 *
 *
 *  2014 Aug 19 - Mixed some code to be used with an Atmega 328p
 *                and the Arduino Wire library:
 *                https://github.com/desaster/sensortest
 *                http://forum.arduino.cc/index.php?topic=159557
 *
 *       Aug 28 - Added definitions to be able to work with either
 *                the hardware or the software serial. If using the
 *                hardware UART, the TX and RX pins have to be
 *                disconnected from the fifo or ensure they are not
 *                written while reading the LS data bits. 
 *              - Included PinChangeInt library and change the code
 *                so we dont wait while the CMOS IC loads the frame
 *                into the fifo IC (which takes ~33ms at 30fps).
 *                http://dl.bintray.com/greygnome/generic/pinchangeint-2.30beta.zip
 *                Initial versions will just overwrite the start
 *                of the fifo to store each new frame.
 *       Sep 25 - Cleaned up and reorganized the code for a public release.
 *                Increased baudrate after modifying arduino's HardwareSerial.cpp
 *                see http://mekonik.wordpress.com/2009/03/02/modified-arduino-library-serial/
 *
 ********************************************/

// --------------------------------
// http://subethasoftware.com/2013/04/09/arduino-compiler-problem-with-ifdefs-solved/
// BOF preprocessor bug prevent - insert me on top of your arduino-code
// From: http://www.a-control.de/arduino-fehler/?lang=en
#if 1
__asm volatile ("nop");
#endif
// --------------------------------


#include "IO_config.h"
#include "sensor.h"
#include "fifo.h"
#include <Wire.h>

//#define USE_SOFT_SERIAL
// serial stuff  
static const byte LF = 10; // line feed character
#ifdef USE_SOFT_SERIAL
   #include <SoftwareSerial.h>
   static const unsigned int _BAUDRATE = 19200; // 19200 is the maximum reliable soft serial baud rate for 8MHz processors
   #define txPin A0
   #define rxPin A1
   SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);  
   SoftwareSerial *serialPtr = &mySerial;
#else
   // 38400 is the maximum supposed reliable UART baud rate for 8MHz processors
   // However, I've had succes at 500000bps with an USB-FTDI cable
   // I've modified the file arduino-1.5.6-r2/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp
   // substituting all the lines with operations like this (for both rx and tx buffers):
   //
   //  _rx_buffer_tail = (uint8_t)(_rx_buffer_tail + 1) % SERIAL_BUFFER_SIZE;
   //
   // for two lines like:
   //
   //  _rx_buffer_tail++;
   //  _rx_buffer_tail %= SERIAL_BUFFER_SIZE;
   //
   // see http://mekonik.wordpress.com/2009/03/02/modified-arduino-library-serial/
   //
   static const unsigned long _BAUDRATE = 500000;
   HardwareSerial *serialPtr = &Serial;
#endif    

uint8_t rcvbuf[16], rcvbufpos = 0, c;

// fps calculation stuff
static unsigned int oneSecond = 1000;
unsigned int volatile frameCount = 0;
unsigned int fps = 0;
unsigned long volatile lastTime = 0;
unsigned long volatile timeStamp = 0;

//#define QQVGA
#define QQQVGA

#ifdef QQVGA
static const uint8_t fW = 160;
static const uint8_t fH = 120;
static const frameFormat_t frameFormat = FF_QQVGA;
#else ifdef QQQVGA
static const uint8_t fW = 80;
static const uint8_t fH = 60;
static const frameFormat_t frameFormat = FF_QQQVGA;
#endif


static const uint8_t YUYV_BPP = 2; // bytes per pixel
static const unsigned int MAX_FRAME_LEN = fW * YUYV_BPP;
byte rowBuf[MAX_FRAME_LEN];
unsigned int volatile nRowsSent = 0;
boolean volatile bRequestPending = false;
boolean volatile bFifoLocked = false;
uint8_t volatile thresh = 128;

enum serialRequest_t {
  SEND_NONE = 0,
  SEND_DARK,
  SEND_BRIG,
  SEND_FPS,
  SEND_0PPB = MAX_FRAME_LEN,
  SEND_1PPB = fW,
  SEND_2PPB = fW/2,
  SEND_4PPB = fW/4,
  SEND_8PPB =fW/8 
};

serialRequest_t serialRequest = SEND_NONE;


// *****************************************************
//                          SETUP
// *****************************************************
void setup()
{
  setup_IO_ports();
  
  serialPtr->begin(_BAUDRATE);

  serialPtr->println("Initializing sensor...");
  for (int i = 0; i < 10; i ++) {
       unsigned int result = sensor_init(frameFormat);
      if (result != 0) {
        serialPtr->print("inited OK, sensor PID: ");
        serialPtr->println(result, HEX);
        break;
      }
      else if (i == 5) {
          serialPtr->println("PANIC! sensor init keeps failing!");
          while (1);
      } else {
          serialPtr->println("retrying...");
          delay(300);
      }
  }
 attachInterrupt(VSYNC_INT, &vsyncIntFunc, FALLING);
  delay(100);
}
// *****************************************************
//                          LOOP
// *****************************************************
void loop()
{  
  while (serialPtr->available()) {
    // get the new byte:
    c = serialPtr->read();
     if ((c != LF) && (c >= 32) && (c <= 126)) {
            rcvbuf[rcvbufpos++] = c;
        } else if (c == LF) {
            rcvbuf[rcvbufpos++] = 0;
            rcvbufpos = 0;
            parseSerialBuffer();
        }
  }

}

// *****************************************************
//               PARSE SERIAL BUFFER
// ****************************************************
void parseSerialBuffer(void) {
   if (strcmp((char *) rcvbuf, "hello") == 0) {
        serialPtr->print("Hello to you too!\n");
    } else if ( strlen((char *) rcvbuf) > 5 && 
                strncmp((char *) rcvbuf, "send ", 5) == 0) {
        serialRequest = (serialRequest_t)atoi((char *) (rcvbuf + 5)); 
        serialPtr->print("ACK\n");
        bRequestPending = true;       
    } 
    else if (strlen((char *) rcvbuf) > 5 &&
            strncmp((char *) rcvbuf, "dark ", 5) == 0) {
              thresh = atoi((char *) (rcvbuf + 5));
              serialPtr->print("ACK\n");
              serialRequest = SEND_DARK;
              bRequestPending = true;
    }
    else if (strlen((char *) rcvbuf) > 5 &&
            strncmp((char *) rcvbuf, "brig ", 5) == 0) {
              thresh = atoi((char *) (rcvbuf + 5));
              serialPtr->print("ACK\n");
              serialRequest = SEND_BRIG;
              bRequestPending = true;
    }
    else if (strlen((char *) rcvbuf) > 7 &&
            strncmp((char *) rcvbuf, "thresh ", 7) == 0) {
              thresh = atoi((char *) (rcvbuf + 7));
    }
}
// *****************************************************
//               VSYNC INTERRUPT HANDLER
// *****************************************************
void static __inline__ vsyncIntFunc() {
      if (!bFifoLocked) {
          DISABLE_WREN; // disable writing to fifo

          ENABLE_WRST;
          //__delay_cycles(500);
          SET_RCLK_H;
          //__delay_cycles(100);
          SET_RCLK_L;
          DISABLE_WRST;
          _delay_cycles(10);
         
          if (bRequestPending) {
            detachInterrupt(VSYNC_INT);
            bFifoLocked = true;
            processRequest();
            bRequestPending = false;
            bFifoLocked = false;
            attachInterrupt(VSYNC_INT, &vsyncIntFunc, FALLING);
          }
          ENABLE_WREN; // enable writing to fifo
      } 
}

// **************************************************************
//                      PROCESS SERIAL REQUEST
// **************************************************************
void processRequest() {
        fifo_rrst();
        switch (serialRequest) {
          case SEND_0PPB: for (int i =0; i< fH; i++) {
                              fifo_readRow0ppb(rowBuf, rowBuf+serialRequest);
                              serialPtr->write(rowBuf, serialRequest);
                              serialPtr->write(LF); 
                          } break;
          case SEND_1PPB: for (int i =0; i< fH; i++) {
                              fifo_readRow1ppb(rowBuf, rowBuf+serialRequest);
                              serialPtr->write(rowBuf, serialRequest); 
                              serialPtr->write(LF); 
                          } break;
          case SEND_2PPB:  for (int i =0; i< fH; i++) {
                              fifo_readRow2ppb(rowBuf, rowBuf+serialRequest);
                              serialPtr->write(rowBuf, serialRequest); 
                              serialPtr->write(LF); 
                          } break;
          case SEND_4PPB: for (int i =0; i< fH; i++) {
                              fifo_readRow4ppb(rowBuf, rowBuf+serialRequest);
                              serialPtr->write(rowBuf, serialRequest); 
                              serialPtr->write(LF); 
                          } break;
          case SEND_8PPB: for (int i =0; i< fH; i++) {
                              fifo_readRow8ppb(rowBuf, rowBuf+serialRequest, thresh);
                              serialPtr->write(rowBuf, serialRequest); 
                              serialPtr->write(LF); 
                          } break;
          case SEND_BRIG: fifo_getBrig(rowBuf, fW, fH, 8, thresh);
                          serialPtr->write(rowBuf, 4);
                          serialPtr->write(LF); 
                          break;
          case SEND_DARK: fifo_getDark(rowBuf, fW, fH, 8, thresh);
                          serialPtr->write(rowBuf, 4);
                          serialPtr->write(LF); 
                          break;
          case SEND_FPS:  calcFPS(fps);
                          serialPtr->print(fps, DEC);
                          serialPtr->write(LF); 
                          break;

          default : break;
        }
}


// **************************************************************
//                      CALCULATE FPS
// **************************************************************
void calcFPS(unsigned int &currentFPS) {
      unsigned long currTime = millis();
      unsigned long currTimeDiff = currTime-lastTime;
      if (currTimeDiff >= oneSecond) {
        lastTime = currTime;
        currentFPS = (oneSecond*frameCount)/currTimeDiff;
       frameCount = 0;
       
      }
      while (GET_VSYNC); // wait for an old frame to end
      while (!GET_VSYNC);// wait for a new frame to start
      frameCount++;
}

/*
void serialEvent() {
  
}
*/

