
/*******************************************************************
 *
 *   by David Sanz Kirbis
 *
 *  Defines ports and pins used to connect an omnivision sensor+al422 
 *  fifo camera module to an Mega2560 Arduino clone board.
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
 *  2016 June 21 - Modified pin mapping for Mega2560
 *
 *
 *   Wire the module as follows:
 *
 *
 *   WARNING!!!! : sensor works at 3.3V USE A VOLTAGE LEVEL TRANSLATOR
 *
 *       OV+FIFO  -----------------------  ATMEGA 2560 (3.3V/8MHz)
 *
 *           GND  -----------------------  GND
 *          3.3v  -----------------------  3.3v
 *       FIFO_OE  ---------------------->  GND
 *      FIFO_WEN  --> level tranlator -->  PH5 / D8
 *     FIFO_RCLK  --> level tranlator -->  PH6 / D9 /PWM
 *     FIFO_WRST  --> level tranlator -->  PB6 / D12
 *     FIFO_RRST  --> level tranlator -->  PB7 / D13
 *        OV_SDA  <-> level tranlator <->  PD1 / SDA
 *        OV_SCL  <-> level tranlator <->  PD0 / SCL
 *      OV_VSYNC  <-- level tranlator <--  PE4 / D2
 *       FIFO_D3  <-- level tranlator <--  PF3 / A3
 *       FIFO_D4  <-- level tranlator <--  PF4 / A4
 *       FIFO_D5  <-- level tranlator <--  PF5 / A5
 *       FIFO_D6  <-- level tranlator <--  PF6 / A6
 *       FIFO_D7  <-- level tranlator <--  PF7 / A7
 *
 *
 *
 ********************************************/

#ifndef IO_CONFIG_H_
#define IO_CONFIG_H_


// data pins --------------------

#define DATA_DDR	    DDRF
#define DATA_PORT	    PORTF
#define DATA_PINS	    PINF

// control pins --------------------
#define VSYNC_INT 0 

#ifdef VSYNC_INT
  #define OV_VSYNC            _BV(PINE4) 
#else
  #define OV_VSYNC            _BV(PINE5) 
#endif

#define FIFO_WREN          _BV(PINH5)          // Write Enable (active low) 
#define FIFO_RCLK          _BV(PINH6)          // Read clock
#define FIFO_WRST          _BV(PINB6)          // Write Reset (active low)
#define FIFO_RRST          _BV(PINB7)          // Read Reset (active low)

#define WREN_DDR          DDRH
#define WREN_PORT         PORTH

#define RCLK_DDR          DDRH
#define RCLK_PORT         PORTH

#define WRST_DDR          DDRB
#define WRST_PORT         PORTB

#define RRST_DDR          DDRB
#define RRST_PORT         PORTB

#ifdef VSYNC_INT
  #define VSYNC_PIN         PINE
  #define VSYNC_DDR         DDRE
  #define VSYNC_PORT        PORTE
#else
  #define VSYNC_PIN         PINE
  #define VSYNC_DDR         DDRE
  #define VSYNC_PORT        PORTE
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
  //DDRB = DDRD = DDRE = DDRH = DDRF = PORTB = PORTD = PORTE = PORTH = PORTF = 0;
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
