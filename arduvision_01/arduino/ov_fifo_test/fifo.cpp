/*********************************************
 *
 *   Part of the ARDUVISION project
 *
 *   by David Sanz Kirbis
 *
 *   More info: http://www.arduvision.com
 *
 **********************************************/

#include "fifo.h"


void fifo_loadFrame(void)
{
    fifo_wrst();
    // Capture frame 
    // VSYNC is supposed to be set positive in the ov7725 (reg REG_COM10) 
    while (!GET_VSYNC);     // wait for an old frame to end
    while (GET_VSYNC);    // wait for a new frame to start
    ENABLE_WREN;               // enable writing to fifo
    while (!GET_VSYNC);     // wait for the current frame to end
    DISABLE_WREN;              // disable writing to fifo
    //wait();
    DISABLE_RRST;
}


// Reset the fifo read pointer 
void fifo_rrst(void)
{
  ENABLE_RRST;// reset the read pointer
  _delayMicroseconds(1);
  SET_RCLK_L;
  _delayMicroseconds(1);
  SET_RCLK_H;
  _delayMicroseconds(1);
  SET_RCLK_L;
  _delayMicroseconds(1);
  DISABLE_RRST; 
  _delayMicroseconds(1);
  SET_RCLK_H;
}
//**************************

// Reset the fifo write pointer 
void fifo_wrst(void)
{
    DISABLE_WREN;
    _delayMicroseconds(1);
    ENABLE_WRST;
    _delayMicroseconds(1);
    DISABLE_WRST;
}
//**************************

// Read one byte from the fifo
uint8_t fifo_readByte(void)
{
    uint8_t val;
    SET_RCLK_H;
    val = DATA_PINS;
    //_delay_cycles(10);
    SET_RCLK_L;
    //_delay_cycles(10);

    return val;
}

//**************************
//
//
//**************************
uint8_t fifo_readBytes(uint8_t *destBuf, unsigned long nBytes)
{
    uint8_t *bufIndex = destBuf;
    
    while (nBytes > 0) {
        SET_RCLK_H;
        *bufIndex = DATA_PINS;
        //_delay_cycles(10);
        SET_RCLK_L;
        //_delay_cycles(10);
        bufIndex++;
        nBytes--;
    }
    return (nBytes == 0);
}


//**************************

void fifo_sendSerialBytes(Stream &destPort, unsigned long nBytes) {
  
  uint8_t oneByte;
  // passing the serial por (Stream type) as a pointer would save memory ??? 
  while (nBytes > 0) {
      // faster than passing the fifo_readByte() function to write()
      oneByte = fifo_readByte(); 
      destPort.write(oneByte); //print((char)ov7725_read());
      nBytes--;
  }
}
