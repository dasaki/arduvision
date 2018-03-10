#include <avr/io.h>
#include <Arduino.h>
#include "IO_config.h"
#include "delay.h"

void fifo_loadFrame(void);

void fifo_rrst(void);
void fifo_wrst(void);
uint8_t fifo_readByte(void);
void fifo_readStop(void);

uint8_t fifo_readBytes(uint8_t *destBuf, unsigned long nBytes);

void fifo_sendSerialBytes(Stream &destPort, unsigned long nBytes);

// --------------------------------------
void  __inline__ fifo_skipBytes(unsigned long nBytes)
{
    while (nBytes > 0) {
        SET_RCLK_H;
        _delay_cycles(10);
        SET_RCLK_L;
        _delay_cycles(10);
        nBytes--;
    }
}
// --------------------------------------
static __inline__ void fifo_readRow0ppb(uint8_t* _rowStart, uint8_t* _rowEnd)
{
    while (_rowStart != _rowEnd) {
      SET_RCLK_H;
      *_rowStart++ = DATA_PINS;
      //_delayNanoseconds(5);
      SET_RCLK_L;
      //_delayNanoseconds(5);
    }
}
// --------------------------------------
static __inline__ void fifo_readRow1ppb(uint8_t* _rowStart, uint8_t* _rowEnd)
{
   while (_rowStart != _rowEnd) {
      SET_RCLK_H;
      *_rowStart = DATA_PINS >> 4;
      //_delayNanoseconds(5);
      SET_RCLK_L;
      //_delayNanoseconds(5);
      SET_RCLK_H;
      *_rowStart++ |= DATA_PINS & 0xf0;
      //_delayNanoseconds(5);
      SET_RCLK_L;
      //_delayNanoseconds(5);
   }
}
// --------------------------------------
static __inline__ void fifo_readRow2ppb(uint8_t* _rowStart, uint8_t* _rowEnd)
{
    uint8_t pixValue = 0;
    uint8_t dataValue = 0;
    while (_rowStart != _rowEnd) {

      SET_RCLK_H;
      pixValue = DATA_PINS >> 4;
      SET_RCLK_L;

      SET_RCLK_H;
      SET_RCLK_L;

      SET_RCLK_H;
      pixValue |= (DATA_PINS & 0xF0);
      SET_RCLK_L;

      SET_RCLK_H;
      SET_RCLK_L;

      *_rowStart++ = pixValue;
    }
}
// --------------------------------------
static __inline__ void fifo_readRow4ppb(uint8_t* _rowStart, uint8_t* _rowEnd)
{
    uint8_t pixValue = 0;
    uint8_t dataValue = 0;
    while (_rowStart != _rowEnd) {

      SET_RCLK_H;
      pixValue = DATA_PINS >> 6;
      SET_RCLK_L;

      SET_RCLK_H;
      SET_RCLK_L;

      SET_RCLK_H;
      pixValue |= (DATA_PINS & 0xC0) >> 4;
      SET_RCLK_L;

      SET_RCLK_H;
      SET_RCLK_L;

      SET_RCLK_H;
      pixValue |= (DATA_PINS & 0xC0) >> 2;
      SET_RCLK_L;

      SET_RCLK_H;
      SET_RCLK_L;

      SET_RCLK_H;
      pixValue |= DATA_PINS & 0xC0;      
      SET_RCLK_L;

      SET_RCLK_H;
      SET_RCLK_L;

      *_rowStart++ = pixValue;
    }
}
// --------------------------------------
static __inline__ void fifo_readRow8ppb(uint8_t* _rowStart, uint8_t* _rowEnd, uint8_t thresh)
{
    uint8_t pixValue = 0;
    uint8_t bitIndex = 0;
    
    while (_rowStart != _rowEnd ) {
        //read "Y" byte
        SET_RCLK_H;
        pixValue |= (((DATA_PINS  & 0xF8) > thresh) << bitIndex);
        SET_RCLK_L;
        
        // skip "U/V" byte
        SET_RCLK_H;
        SET_RCLK_L;
        bitIndex++;
        if (bitIndex == 8) {
           *_rowStart++ = pixValue;
           bitIndex = 0;
           pixValue = 0;
        }
     
    }
}
// --------------------------------------------
// --------------------------------------------
static __inline__ void fifo_getBrig(uint8_t* _rowStart, uint8_t _frW,  uint8_t _frH, uint8_t _border, uint8_t _thresh)
{
  
}
// --------------------------------------------
static __inline__ void fifo_getDark(uint8_t* _rowStart, uint8_t _frW,  uint8_t _frH, uint8_t _border, uint8_t _thresh)
{
  uint8_t i = 0;
  uint8_t j = 0;
  uint8_t pix = 255;
  uint8_t x0 = 255;
  uint8_t y0 = 255;
  uint8_t x1 = 0;
  uint8_t y1 = 0;
  uint8_t skipBytesX = _border * 2 ; // BPP
  uint8_t skipBytesX2 = skipBytesX * 2 + 2;
  uint16_t skipBytesY = (_border * _frW) * 2; // BPP
   
  _frH -= _border;
  _frW -= _border+1;
  
  fifo_skipBytes(skipBytesY);
  fifo_skipBytes(skipBytesX);
  for (j =_border; j < _frH; j++) {
      for (i =_border; i < _frW; i++) {
            // Y value of Nth pixel
            SET_RCLK_H;
            pix = DATA_PINS;
            SET_RCLK_L;
            if (pix < _thresh) {
              if (i > x1) x1 = i;
              else if (i < x0) x0 = i;
              if (y0 == 255) y0 = j; // first time only, y0 = 255
              y1 = j;
            }
            // skip "U/v" byte
            SET_RCLK_H;
            _delayNanoseconds(5);
            SET_RCLK_L;
            _delayNanoseconds(5);
     } 
     fifo_skipBytes(skipBytesX2);
  } 
 
  if ( (x0 < x1 ) && (y0 < y1 ) ) {   
      *_rowStart++ = x0;
      *_rowStart++ = y0;
      *_rowStart++ = x1;
      *_rowStart++ = y1;
  }
}
// --------------------------------------
static __inline__ void fifo_loadFrameFast(void)
{
    DISABLE_WREN;              // disable writing to fifo

    ENABLE_WRST;
    //_delayNanoseconds(5);
    SET_RCLK_H;
    //_delayNanoseconds(5);
    SET_RCLK_L;
    DISABLE_WRST;
    _delay_cycles(10);
    while (!GET_VSYNC);     // wait for an old frame to end
    while (GET_VSYNC);    // wait for a new frame to start

    ENABLE_WREN;               // enable writing to fifo
    while (!GET_VSYNC);     // wait for the current frame to end
    
    ENABLE_RRST;
    //_delayNanoseconds(5);
    SET_RCLK_H;
    //_delayNanoseconds(5);
    SET_RCLK_L;
    DISABLE_RRST;
}



