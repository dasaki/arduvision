
/*******************************************************************
 *
 *   by David Sanz Kirbis
 *
 *  Defines ports and pins used to connect an omnivision sensor+al422 
 *  fifo camera module to an ATmega 328p Pro Mini Arduino clone board.
 *  The module tested with this setup has a 12MHz clock connected to
 *  the sensor XCLK and the following header / pinout:
 *
 *                     GND --- 1     2 --- VCC3.3
 *                  OV_SCL --- 3     4 --- FIFO_WRST
 *                  OV_SDA --- 5     6 --- FIFO_RRST
 *                 FIFO_D0 --- 7     8 --- FIFO_OE
 *                 FIFO_D2 --- 9    10 --- FIFO_D1
 *                 FIFO_D4 --- 11   12 --- FIFO_D3
 *                 FIFO_D6 --- 13   14 --- FIFO_D5
 *               FIFO_RCLK --- 15   16 --- FIFO_D7
 *                FIFO_WEN --- 17   18 --- OV_VSYNC
 *
 *
 *  The definitions and implementation design is based on:
 *  arndtjenssen's ov7670_ports.h (Aug 18, 2013)
 *  see: https://github.com/arndtjenssen/ov7670
 *
 *  2014 Aug 19 - Modified original file to be used with an
 *                Atmega 328p
 *       Aug 21 - Changed pin mapping to put all data pins in
 *                the same port, to save 1/3rd of the time employed
 *                to read data from the fifo IC (i.e., to read each
 *                whole byte in just one instruction).
 *                The LS bits of the pixel data coming from the modules
 *                are discarded in order to be able to communicate to
 *                the 328p through the hardware UART TX and RX pins
 *                and allow uploding Arduino sketches.
 *                Also, interrupt pin INT0 is kept available.
 *                The resulting image quality shows the expected
 *                banding due to the reduction of depth but it is
 *                fair enough for detection purposes. 
 *
 *
 *   Wire the module as follows:
 *
 *
 *       OV+FIFO  ------  ATMEGA 328P (3.3V/8MHz)
 *
 *           GND  ------  GND
 *          3.3v  ------  VCC
 *       FIFO_OE  ------  GND
 *      FIFO_WEN  ------  PB0/D8
 *     FIFO_RCLK  ------  PB1/D9 /PWM
 *                ------  PB2/D10/PWM
 *                ------  PB3/D11/PWM
 *     FIFO_WRST  ------  PB4/D12
 *     FIFO_RRST  ------  PB5/D13
 *                        PB6 - crystal
 *                        PB7 - crystal
 *
 *                ------  PC0/A0
 *                ------  PC1/A1
 *                ------  PC2/A2
 *                ------  PC3/A3
 *        OV_SDA  ------  PC4/A4/SDA
 *        OV_SCL  ------  PC5/A5/SCL
 *                        PC6/A6 - TQFP readonly
 *                        PC7/A7 - TQFP readonly
 *
 *                        PD0/D0 - TX
 *                        PD1/D1 - RX
 *      OV_VSYNC  ------  PD2/D2/INT0
 *       FIFO_D3  ------  PD3/D3/INT1/PWM
 *       FIFO_D4  ------  PD4/D4
 *       FIFO_D5  ------  PD5/D5/PWM
 *       FIFO_D6  ------  PD6/D6/PWM
 *       FIFO_D7  ------  PD7/D7
 *
 *
 *
 ********************************************/

#ifndef IO_CONFIG_H_
#define IO_CONFIG_H_

#include <avr/io.h>


// data pins --------------------

#define DATA_DDR	    DDRD
#define DATA_PORT	    PORTD
#define DATA_PINS	    PIND

// control pins --------------------
#define VSYNC_INT 0 

#ifdef VSYNC_INT
  #define OV_VSYNC            _BV(PIND2) 
#else
  #define OV_VSYNC            _BV(PINB0) 
#endif

#define FIFO_WREN          _BV(PINB0)          // Write Enable (active low) 
#define FIFO_RCLK          _BV(PINB1)          // Read clock
#define FIFO_WRST          _BV(PINB4)          // Write Reset (active low)
#define FIFO_RRST          _BV(PINB5)          // Read Reset (active low)

#define WREN_DDR          DDRB
#define WREN_PORT         PORTB

#define RCLK_DDR          DDRB
#define RCLK_PORT         PORTB

#define WRST_DDR          DDRB
#define WRST_PORT         PORTB

#define RRST_DDR          DDRB
#define RRST_PORT         PORTB

#ifdef VSYNC_INT
  #define VSYNC_PIN         PIND
  #define VSYNC_DDR         DDRD
  #define VSYNC_PORT        PORTD
#else
  #define VSYNC_PIN         PINB
  #define VSYNC_DDR         DDRB
  #define VSYNC_PORT        PORTB
#endif

#define GET_VSYNC          (VSYNC_PIN & OV_VSYNC) 

#define DISABLE_RRST        RRST_PORT|=FIFO_RRST
#define ENABLE_RRST          RRST_PORT&=~FIFO_RRST 

#define DISABLE_WRST        WRST_PORT|=FIFO_WRST
#define ENABLE_WRST          WRST_PORT&=~FIFO_WRST 

#define SET_RCLK_H            RCLK_PORT|=FIFO_RCLK   
#define SET_RCLK_L          RCLK_PORT&=~FIFO_RCLK

#define ENABLE_WREN         WREN_PORT |= FIFO_WREN
#define DISABLE_WREN         WREN_PORT &= ~FIFO_WREN


// *************************************
void static inline setup_IO_ports() {
  
  // reset registers and register directions
  DDRB = DDRD = PORTB = PORTD = 0;
  // set fifo data pins as inputs 
  DATA_DDR  = 0;  // set pins as INPUTS
  
  WREN_DDR  |= FIFO_WREN; // set pin as OUTPUT
  RCLK_DDR  |= FIFO_RCLK; // set pin as OUTPUT
  RRST_DDR  |= FIFO_RRST; // set pin as OUTPUT
  WRST_DDR  |= FIFO_WRST; // set pin as OUTPUT
  
  VSYNC_DDR &= ~(OV_VSYNC); // set pin as INPUT
#ifdef VSYNC_INT
  VSYNC_PORT |= OV_VSYNC; // enable pullup (for interruption handler)
#endif
}


#endif /* IO_CONFIG_H_ */
