/*
 * A V4L2 driver for OmniVision OV7670 cameras.
 *
 * Copyright 2006 One Laptop Per Child Association, Inc.  Written
 * by Jonathan Corbet with substantial inspiration from Mark
 * McClelland's ovcamchip code.
 *
 * Copyright 2006-7 Jonathan Corbet <corbet@lwn.net>
 *
 * This file may be distributed under the terms of the GNU General
 * Public License, version 2.
 
 *  Aug 18, 2014 - Adapted by David Sanz Kirbis:
*/



#ifndef _OV7670_REGS_H
#define _OV7670_REGS_H


#include <stdint.h>
#include <avr/pgmspace.h>
#include "sensor.h"


#define OV7670_RD_ADDR  0x42
#define OV7670_WR_ADDR  0x43

#define VGA_WIDTH       640
#define VGA_HEIGHT      480
#define QVGA_WIDTH      320
#define QVGA_HEIGHT     240
#define QQVGA_WIDTH      160
#define QQVGA_HEIGHT     120
#define QQQVGA_WIDTH      80
#define QQQVGA_HEIGHT     60
#define CIF_WIDTH       352
#define CIF_HEIGHT      288
#define QCIF_WIDTH      176
#define QCIF_HEIGHT     144
 

// Control 1
#define OV7670_REG_COM7	0x12	// Control 7
#define   COM7_RESET	  0x80	  // Register reset
#define   COM7_FMT_MASK	  0x38
#define   COM7_FMT_VGA	  0x00
#define	  COM7_FMT_CIF	  0x20	  // CIF format
#define   COM7_FMT_QVGA	  0x10	  // QVGA format
#define   COM7_FMT_QCIF	  0x08	  // QCIF format
#define	  COM7_RGB	  0x04	  // bits 0 and 2 - RGB format
#define	  COM7_YUV	  0x00	  // YUV
#define	  COM7_BAYER	  0x01	  // Bayer format
#define	  COM7_PBAYER	  0x05	  // "Processed bayer"
#define OV7670_REG_RGB444	0x8c	// RGB 444 control
#define   R444_DISABLE	  0x00	  // Turn on RGB444, overrides 5x5
#define   R444_ENABLE	  0x02	  // Turn on RGB444, overrides 5x5
#define   R444_RGBX	  0x01	  // Empty nibble at end
#define OV7670_REG_COM9	0x14	// Control 9  - gain ceiling
#define OV7670_REG_COM10	0x15	// Control 10
#define OV7670_REG_COM13	0x3d	// Control 13
#define   COM13_GAMMA	  0x80	  // Gamma enable
#define	  COM13_UVSAT	  0x40	  // UV saturation auto adjustment
#define   COM13_UVSWAP	  0x01	  // V before U - w/TSLB
#define OV7670_REG_COM15	0x40	// Control 15
#define   COM15_R10F0	  0x00	  // Data range 10 to F0
#define	  COM15_R01FE	  0x80	  //            01 to FE
#define   COM15_R00FF	  0xc0	  //            00 to FF
#define   COM15_RGB565	  0x10	  // RGB565 output
#define   COM15_RGB555	  0x30	  // RGB555 output
#define OV7670_REG_COM11	0x3b	// Control 11
#define   COM11_NIGHT	  0x80	  // NIght mode enable
#define   COM11_NMFR	  0x60	  // Two bit NM frame rate
#define   COM11_HZAUTO	  0x10	  // Auto detect 50/60 Hz
#define	  COM11_50HZ	  0x08	  // Manual 50Hz select
#define   COM11_EXP	  0x02
#define OV7670_REG_COM17       0x42    // Control 17
#define COM17_AECWIN    0xc0    // AEC window - must match COM4
#define COM17_CBAR      0x08    // DSP Color bar
#define OV7670_REG_TSLB	0x3a	// lots of stuff
#define   TSLB_YLAST	  0x04	  // UYVY or VYUY - see com13
#define   TSLB_AUTO_OW  0x01  // Sensor automatically sets output window when resolution changes
#define MTX1            0x4f    // Matrix Coefficient 1
#define MTX2            0x50    // Matrix Coefficient 2
#define MTX3            0x51    // Matrix Coefficient 3
#define MTX4            0x52    // Matrix Coefficient 4
#define MTX5            0x53    // Matrix Coefficient 5
#define MTX6            0x54    // Matrix Coefficient 6
#define OV7670_REG_CONTRAS     0x56    // Contrast control
#define MTXS            0x58    // Matrix Coefficient Sign
#define AWBC7           0x59    // AWB Control 7
#define AWBC8           0x5a    // AWB Control 8
#define AWBC9           0x5b    // AWB Control 9
#define AWBC10          0x5c    // AWB Control 10
#define AWBC11          0x5d    // AWB Control 11
#define AWBC12          0x5e    // AWB Control 12
#define OV7670_REG_GFIX        0x69    // Fix gain control
#define GGAIN           0x6a    // G Channel AWB Gain
#define OV7670_REG_DBLV            0x6b  //  PLL control & internal regulator ( reg =  on )
#define    DBLV_X0      0x0A   // Bypass PLL
#define    DBLV_X4      0x4A   // Input clock x4
#define    DBLV_X6      0x8A   // Input clock x6
#define    DBLV_X8      0xCA   // Input clock x8
#define AWBCTR3         0x6c    // AWB Control 3
#define AWBCTR2         0x6d    // AWB Control 2
#define AWBCTR1         0x6e    // AWB Control 1
#define AWBCTR0         0x6f    // AWB Control 0
#define OV7670_REG_COM8	0x13	// Control 8
#define   COM8_FASTAEC	  0x80	  // Enable fast AGC/AEC
#define   COM8_AECSTEP	  0x40	  // Unlimited AEC step size
#define   COM8_BFILT	  0x20	  // Band filter enable
#define   COM8_AGC	  0x04	  // Auto gain enable
#define   COM8_AWB	  0x02	  // White balance enable
#define   COM8_AEC	  0x01	  // Auto exposure enable
#define OV7670_REG_COM3        0x0c    // Control 3
#define COM3_SWAP       0x40    // Byte swap
#define COM3_SCALEEN    0x08    // Enable scaling
#define COM3_DCWEN      0x04    // Enable downsamp/crop/window
#define OV7670_REG_BRIGHT      0x55    // Brightness
#define OV7670_REG_COM14	0x3e	// Control 14
#define   COM14_DCWEN	  0x10	  // DCW/PCLK-scale enable
#define   COM14_PCLK_1	  0x00	  // PCLK Divided by 1
#define   COM14_PCLK_2	  0x01	  // PCLK Divided by 2
#define   COM14_PCLK_4	  0x02	  // PCLK Divided by 4
#define   COM14_PCLK_8	  0x03	  // PCLK Divided by 8
#define   COM14_PCLK_16	  0x04	  // PCLK Divided by 16
#define OV7670_REG_HSTART      0x17    // Horiz start high bits
#define OV7670_REG_HSTOP       0x18    // Horiz stop high bits
#define OV7670_REG_VSTART      0x19    // Vert start high bits
#define OV7670_REG_VSTOP       0x1a    // Vert stop high bits
#define OV7670_REG_HREF        0x32    // HREF pieces
#define OV7670_REG_VREF	0x03	// Pieces of GAIN, VSTART, VSTOP
// Registers
#define OV7670_REG_GAIN	0x00	// Gain lower 8 bits (rest in vref)
#define OV7670_REG_BLUE	0x01	// blue gain
#define OV7670_REG_RED		0x02	// red gain
#define OV7670_REG_VREF	0x03	// Pieces of GAIN, VSTART, VSTOP
#define OV7670_REG_COM1	0x04	// Control 1
#define  COM1_CCIR656	  0x40  // CCIR656 enable
#define OV7670_REG_BAVE	0x05	// U/B Average level
#define OV7670_REG_GbAVE	0x06	// Y/Gb Average level
#define OV7670_REG_AECHH	0x07	// AEC MS 5 bits
#define OV7670_REG_RAVE	0x08	// V/R Average level
#define OV7670_REG_COM2	0x09	// Control 2
#define  COM2_SSLEEP	  0x10	// Soft sleep mode
#define OV7670_REG_PID		0x0a	// Product ID MSB
#define OV7670_REG_VER		0x0b	// Product ID LSB
#define OV7670_REG_COM3	0x0c	// Control 3
#define  COM3_SWAP	  0x40	  // Byte swap
#define  COM3_SCALEEN	  0x08	  // Enable scaling
#define  COM3_DCWEN	  0x04	  // Enable downsamp/crop/window
#define OV7670_REG_COM4	0x0d	// Control 4
#define OV7670_REG_COM5	0x0e	// All "reserved"
#define OV7670_REG_COM6	0x0f	// Control 6
#define OV7670_REG_AECH	0x10	// More bits of AEC value
#define OV7670_REG_CLKRC	0x11	// Clocl control
#define   CLK_EXT	  0x40	  // Use external clock directly
#define   CLKRC_0          0x00
#define   CLKRC_2	          0x01	  // internal clock / 2
#define   CLKRC_3	          0x02	  // internal clock / 3
#define   CLKRC_4	          0x03	  // internal clock / 4
#define   CLKRC_5	          0x04	  // internal clock / 5
#define   CLKRC_6	          0x05	  // internal clock / 5
#define   CLK_SCALE	  0x3f	  // Mask for internal clock scale
#define OV7670_REG_COM7	0x12	// Control 7
#define   COM7_RESET	  0x80	  // Register reset
#define   COM7_FMT_MASK	  0x38
#define   COM7_FMT_VGA	  0x00
#define	  COM7_FMT_CIF	  0x20	  // CIF format
#define   COM7_FMT_QVGA	  0x10	  // QVGA format
#define   COM7_FMT_QCIF	  0x08	  // QCIF format
#define	  COM7_RGB	  0x04	  // bits 0 and 2 - RGB format
#define	  COM7_YUV	  0x00	  // YUV
#define	  COM7_BAYER	  0x01	  // Bayer format
#define	  COM7_PBAYER	  0x05	  // "Processed bayer"
#define OV7670_REG_COM8	0x13	// Control 8
#define   COM8_FASTAEC	  0x80	  // Enable fast AGC/AEC
#define   COM8_AECSTEP	  0x40	  // Unlimited AEC step size
#define   COM8_BFILT	  0x20	  // Band filter enable
#define   COM8_AGC	  0x04	  // Auto gain enable
#define   COM8_AWB	  0x02	  // White balance enable
#define   COM8_AEC	  0x01	  // Auto exposure enable
#define OV7670_REG_COM9	0x14	// Control 9  - gain ceiling
#define OV7670_REG_COM10	0x15	// Control 10
#define   COM10_HSYNC	  0x40	  // HSYNC instead of HREF
#define   COM10_PCLK_HB	  0x20	  // Suppress PCLK on horiz blank
#define   COM10_HREF_REV  0x08	  // Reverse HREF
#define   COM10_VS_LEAD	  0x04	  // VSYNC on clock leading edge
#define   COM10_VS_NEG	  0x02	  // VSYNC negative
#define   COM10_HS_NEG	  0x01	  // HSYNC negative 
#define OV7670_REG_HSTART	0x17	// Horiz start high bits 
#define OV7670_REG_HSTOP	0x18	// Horiz stop high bits 
#define OV7670_REG_VSTART	0x19	// Vert start high bits 
#define OV7670_REG_VSTOP	0x1a	// Vert stop high bits 
#define OV7670_REG_PSHFT	0x1b	// Pixel delay after HREF 
#define OV7670_REG_MIDH	0x1c	// Manuf. ID high 
#define OV7670_REG_MIDL	0x1d	// Manuf. ID low 
#define OV7670_REG_MVFP	0x1e	// Mirror / vflip 
#define   MVFP_MIRROR	  0x20	  // Mirror image 
#define   MVFP_FLIP	  0x10	  // Vertical flip 

#define OV7670_REG_AEW		0x24	// AGC upper limit 
#define OV7670_REG_AEB		0x25	// AGC lower limit 
#define OV7670_REG_VPT		0x26	// AGC/AEC fast mode op region 
#define OV7670_REG_HSYST	0x30	// HSYNC rising edge delay 
#define OV7670_REG_HSYEN	0x31	// HSYNC falling edge delay 
#define OV7670_REG_HREF	0x32	// HREF pieces 
#define OV7670_REG_TSLB	0x3a	// lots of stuff 
#define   TSLB_YLAST	  0x04	  // UYVY or VYUY - see com13 
#define   TSLB_AUTO_OW	  0x01	  // Sensor automatically sets output window when resolution changes
#define OV7670_REG_COM11	0x3b	// Control 11 
#define   COM11_NIGHT	  0x80	  // NIght mode enable 
#define   COM11_NMFR	  0x60	  // Two bit NM frame rate 
#define   COM11_HZAUTO	  0x10	  // Auto detect 50/60 Hz 
#define	  COM11_50HZ	  0x08	  // Manual 50Hz select 
#define   COM11_EXP	  0x02
#define OV7670_REG_COM12	0x3c	// Control 12 
#define   COM12_HREF	  0x80	  // HREF always 
#define OV7670_REG_COM13	0x3d	// Control 13 
#define   COM13_GAMMA	  0x80	  // Gamma enable 
#define	  COM13_UVSAT	  0b11001000	  // UV saturation auto adjustment 
#define   COM13_UVSWAP	  0x01	  // V before U - w/TSLB 
#define   COM13_RSVD      0x08
#define OV7670_REG_COM14	  0x3e	// Control 14 
#define   COM14_DCWEN	  0x10	  // DCW/PCLK-scale enable 
#define   COM14_DCW_MSCL  0x08    // Manual scaling
#define   COM14_PCLK_DIV_2 0x01   // divide pclk by 2
#define   COM14_PCLK_DIV_4 0x02   // divide pclk by 4
#define   COM14_PCLK_DIV_8 0x03   // divide pclk by 8
#define   COM14_PCLK_DIV_16 0x04   // divide pclk by 16

#define OV7670_REG_EDGE	0x3f	// Edge enhancement factor 
#define OV7670_REG_COM15	0x40	// Control 15 
#define   COM15_R10F0	  0x00	  // Data range 10 to F0 
#define	  COM15_R01FE	  0x80	  //            01 to FE 
#define   COM15_R00FF	  0xc0	  //            00 to FF 
#define   COM15_RGB565	  0x10	  // RGB565 output 
#define   COM15_RGB555	  0x30	  // RGB555 output 
#define OV7670_REG_COM16	0x41	// Control 16 
#define   COM16_AWBGAIN   0x08	  // AWB gain enable 
#define OV7670_REG_COM17	0x42	// Control 17 
#define   COM17_AECWIN	  0xc0	  // AEC window - must match COM4 
#define   COM17_CBAR	  0x08	  // DSP Color bar 

#define OV7670_REG_SCALING_XSC     0x70   // Bit[6:0]: Horizontal scale factor, def 3A
#define OV7670_REG_SCALING_YSC     0x71   // Bit[6:0]: Vertical scale factor, def 35


#define OV7670_REG_SCALING_DCWCTR 0x72
#define   SCALING_DCWCTR_2 0x11 // downsample by 2
#define   SCALING_DCWCTR_4 0x22 // downsample by 4
#define   SCALING_DCWCTR_8 0x33 // downsample by 8

#define OV7670_REG_SCALING_PCLK_DIV 0x73
#define   SCALING_PCLK_DIV_1 0x00
#define   SCALING_PCLK_DIV_2 0x01
#define   SCALING_PCLK_DIV_4 0x02
#define   SCALING_PCLK_DIV_8 0x03
#define   SCALING_PCLK_DIV_16 0x04

#define OV7670_REG_SCALING_PCLK_DELAY 0xA2  // Bit[6:0]: Scaling output delay, def 0x02

#define OV7670_REG_SATCTR          0xC9


/*
 * This matrix defines how the colors are generated, must be
 * tweaked to adjust hue and saturation.
 *
 * Order: v-red, v-green, v-blue, u-red, u-green, u-blue
 *
 * They are nine-bit signed quantities, with the sign bit
 * stored in 0x58.  Sign for v-red is bit 0, and up from there.
 */
#define	OV7670_REG_CMATRIX_BASE 0x4f
#define   CMATRIX_LEN 6
#define OV7670_REG_CMATRIX_SIGN 0x58


#define OV7670_REG_BRIGHT	0x55	/* Brightness */
#define OV7670_REG_CONTRAS	0x56	/* Contrast control */

#define OV7670_REG_GFIX	0x69	/* Fix gain control */

#define OV7670_REG_REG76	0x76	/* OV's name */
#define   R76_BLKPCOR	  0x81	  /* Black pixel correction enable */
#define   R76_WHTPCOR	  0x41	  /* White pixel correction enable */

#define	OV7670_REG_RGB444	0x8c	/* RGB 444 control */
#define	R444_ENABLE	0x02	  /* Turn on RGB444, overrides 5x5 */
#define	R444_RGBX	0x01	  /* Empty nibble at end */

#define OV7670_REG_HAECC1	0x9f	/* Hist AEC/AGC control 1 */
#define OV7670_REG_HAECC2	0xa0	/* Hist AEC/AGC control 2 */

#define OV7670_REG_BD50MAX	0xa5	/* 50hz banding step limit */
#define OV7670_REG_HAECC3	0xa6	/* Hist AEC/AGC control 3 */
#define OV7670_REG_HAECC4	0xa7	/* Hist AEC/AGC control 4 */
#define OV7670_REG_HAECC5	0xa8	/* Hist AEC/AGC control 5 */
#define OV7670_REG_HAECC6	0xa9	/* Hist AEC/AGC control 6 */
#define OV7670_REG_HAECC7	0xaa	/* Hist AEC/AGC control 7 */
#define OV7670_REG_BD60MAX	0xab	/* 60hz banding step limit */


const struct regval_list qqvga_yuv_ov7670[] PROGMEM = {

    	{OV7670_REG_COM8, COM8_FASTAEC|COM8_AECSTEP|COM8_AGC|COM8_AEC|COM8_AWB },
      
      {OV7670_REG_CLKRC, 0x80},
      {OV7670_REG_COM11, 0x0A},
      {OV7670_REG_COM7, 0x00}, // output format: yuv 
      {OV7670_REG_COM15, 0xC0}, //   
  
      // not even sure what all these do, gonna check the oscilloscope and go from there... 
      {OV7670_REG_HSTART, 0x16},
      {OV7670_REG_HSTOP, 0x04},
      {OV7670_REG_HREF, 0x24},
      {OV7670_REG_VSTART, 0x02},
      {OV7670_REG_VSTOP, 0x7a},
      {OV7670_REG_VREF, 0x0a},
      {OV7670_REG_COM10, 0x02},
      {OV7670_REG_COM3, 0x04},
      {OV7670_REG_MVFP, 0x3f},
  
      // 160x120
      {OV7670_REG_COM14, 0x1a}, // divide by 4
      {0x72, 0x22}, // downsample by 4
      {0x73, 0xf2}, // divide by 4
  
      // COLOR SETTING
      {0x4f, 0x80},
      {0x50, 0x80},
      {0x51, 0x00},
      {0x52, 0x22},
      {0x53, 0x5e},
      {0x54, 0x80},
      {0x56, 0x40},
      {0x58, 0x9e},
      {0x59, 0x88},
      {0x5a, 0x88},
      {0x5b, 0x44},
      {0x5c, 0x67},
      {0x5d, 0x49},
      {0x5e, 0x0e},
      {0x69, 0x00},
      {0x6a, 0x40},
      {0x6b, 0x0a},
      {0x6c, 0x0a},
      {0x6d, 0x55},
      {0x6e, 0x11},
      {0x6f, 0x9f},
      {0xb0, 0x84},
      { 0xff, 0xff }	// END MARKER
};


const struct regval_list qqqvga_yuv_ov7670[] PROGMEM = {

    	{OV7670_REG_COM8, COM8_AEC|COM8_AWB },
    //	{OV7670_REG_COM8, COM8_FASTAEC|COM8_AECSTEP|COM8_AGC|COM8_AEC|COM8_AWB },
//       {OV7670_REG_REG76, R76_BLKPCOR | R76_WHTPCOR },
    //   {OV7670_REG_COM13, COM13_UVSAT | COM13_GAMMA}, // enable auto UV saturation
   //     {OV7670_REG_SATCTR, 0x60}, // set min satuation value
      
    //  {OV7670_REG_CLKRC, CLKRC_4}, // 15fps (with 12MHz XCLK)
    //  {OV7670_REG_DBLV, DBLV_X8},  // 15fps (with 12MHz XCLK)
  //    {OV7670_REG_CLKRC, CLKRC_2}, // 30fps (with 12MHz XCLK)
  //    {OV7670_REG_DBLV, DBLV_X4},  // 30fps (with 12MHz XCLK)
      {OV7670_REG_CLKRC, CLKRC_2}, // 30fps (with 12MHz XCLK)
      {OV7670_REG_DBLV, DBLV_X8},  // 30fps (with 12MHz XCLK)
  //    {OV7670_REG_CLKRC, CLKRC_0}, // 45fps (with 12MHz XCLK)
    //  {OV7670_REG_DBLV, DBLV_X6},  // 45fps (with 12MHz XCLK)
       
     // {OV7670_REG_COM7, 0x00}, // output format: yuv (is default)
      {OV7670_REG_COM15, 0x00}, //  output range [10] to [F0]
    //  {OV7670_REG_COM15, 0x80}, //  output range [01] to [FE]
   //   {OV7670_REG_COM15, 0xC0}, //  output range [00] to [FF] (default)
     
      // full output window for 640x480 and proportional resolutions
      {OV7670_REG_HSTART, 0x16}, {OV7670_REG_HSTOP, 0x04}, {OV7670_REG_HREF, 0x24},
      {OV7670_REG_VSTART, 0x02}, {OV7670_REG_VSTOP, 0x7a}, {OV7670_REG_VREF, 0x0a},


//------------
// edge enhancement regs:
// 3F OV7670_REG_EDGE Edge Enhancement Adjustment Bit
// [7:5]: Reserved, Bit
// [4:0]: Edge enhancement factor
// def 00
 //{OV7670_REG_EDGE, 0x0F}, 
//------------
// REG75 
// Bit[7:5]: Reserved
// Bit[4:0]: Edge enhancement lower limit
//  {0x75, 0F} 
//------------
// Bit[7]: Black pixel correction: 0: Disable / 1: Enable
// Bit[6]: White pixel correction: 0: Disable / 1: Enable
// Bit[5]:Reserved
// Bit[4:0]: Edge enhancement higher limit
//  {0x76, 01} REG76 

       //test pattern
    //  {0x70, 0x80},
    //  {0x71, 0x80},
      // scale down to 80x60
      {OV7670_REG_COM14, COM14_DCWEN | COM14_PCLK_DIV_8 }, // divide by 8
      {OV7670_REG_SCALING_DCWCTR, SCALING_DCWCTR_8}, // downsample by 8
      {OV7670_REG_COM3, COM3_DCWEN}, // DCW enable, Scale enabled
      {OV7670_REG_SCALING_PCLK_DIV, SCALING_PCLK_DIV_8}, // divide by 8
      { 0xff, 0xff }	// END MARKER

};


const struct regval_list common_reglist_ov7670[] PROGMEM = {//from the linux driver

	{ OV7670_REG_COM3, 0 },	{ OV7670_REG_COM14, 0 },
	/* Mystery scaling numbers */
	{ 0x70, 0x3a },		{ 0x71, 0x35 },
	{ 0x72, 0x11 },		{ 0x73, 0xf0 },
	{ 0xa2,/* 0x02 changed to 1*/1},{ OV7670_REG_COM10, 0x0 },
	/* Gamma curve values */
	{ 0x7a, 0x20 },		{ 0x7b, 0x10 },
	{ 0x7c, 0x1e },		{ 0x7d, 0x35 },
	{ 0x7e, 0x5a },		{ 0x7f, 0x69 },
	{ 0x80, 0x76 },		{ 0x81, 0x80 },
	{ 0x82, 0x88 },		{ 0x83, 0x8f },
	{ 0x84, 0x96 },		{ 0x85, 0xa3 },
	{ 0x86, 0xaf },		{ 0x87, 0xc4 },
	{ 0x88, 0xd7 },		{ 0x89, 0xe8 },
	/* AGC and AEC parameters.  Note we start by disabling those features,
	   then turn them only after tweaking the values. */
	{ OV7670_REG_COM8, COM8_FASTAEC | COM8_AECSTEP },
	{ OV7670_REG_GAIN, 0 },	{ OV7670_REG_AECH, 0 },
	{ OV7670_REG_COM4, 0x40 }, /* magic reserved bit */
	{ OV7670_REG_COM9, 0x18 }, /* 4x gain + magic rsvd bit */
	{ OV7670_REG_BD50MAX, 0x05 },	{ OV7670_REG_BD60MAX, 0x07 },
	{ OV7670_REG_AEW, 0x95 },	{ OV7670_REG_AEB, 0x33 },
	{ OV7670_REG_VPT, 0xe3 },	{ OV7670_REG_HAECC1, 0x78 },
	{ OV7670_REG_HAECC2, 0x68 },	{ 0xa1, 0x03 }, /* magic */
	{ OV7670_REG_HAECC3, 0xd8 },	{ OV7670_REG_HAECC4, 0xd8 },
	{ OV7670_REG_HAECC5, 0xf0 },	{ OV7670_REG_HAECC6, 0x90 },
	{ OV7670_REG_HAECC7, 0x94 },
	{ OV7670_REG_COM8, COM8_FASTAEC|COM8_AECSTEP|COM8_AGC|COM8_AEC },
		{0x30,0},{0x31,0},//disable some delays
	/* Almost all of these are magic "reserved" values.  */
	{ OV7670_REG_COM5, 0x61 },	{ OV7670_REG_COM6, 0x4b },
	{ 0x16, 0x02 },		{ OV7670_REG_MVFP, 0x07 },
	{ 0x21, 0x02 },		{ 0x22, 0x91 },
	{ 0x29, 0x07 },		{ 0x33, 0x0b },
	{ 0x35, 0x0b },		{ 0x37, 0x1d },
	{ 0x38, 0x71 },		{ 0x39, 0x2a },
	{ OV7670_REG_COM12, 0x78 },	{ 0x4d, 0x40 },
	{ 0x4e, 0x20 },		{ OV7670_REG_GFIX, 0 },
	/*{ 0x6b, 0x4a },*/		{ 0x74,0x10},
	{ 0x8d, 0x4f },		{ 0x8e, 0 },
	{ 0x8f, 0 },		{ 0x90, 0 },
	{ 0x91, 0 },		{ 0x96, 0 },
	{ 0x9a, 0 },		{ 0xb0, 0x84 },
	{ 0xb1, 0x0c },		{ 0xb2, 0x0e },
	{ 0xb3, 0x82 },		{ 0xb8, 0x0a },

	/* More reserved magic, some of which tweaks white balance */
	{ 0x43, 0x0a },		{ 0x44, 0xf0 },
	{ 0x45, 0x34 },		{ 0x46, 0x58 },
	{ 0x47, 0x28 },		{ 0x48, 0x3a },
	{ 0x59, 0x88 },		{ 0x5a, 0x88 },
	{ 0x5b, 0x44 },		{ 0x5c, 0x67 },
	{ 0x5d, 0x49 },		{ 0x5e, 0x0e },
	{ 0x6c, 0x0a },		{ 0x6d, 0x55 },
	{ 0x6e, 0x11 },		{ 0x6f, 0x9e }, /* it was 0x9F "9e for advance AWB" */
	{ 0x6a, 0x40 },		{ OV7670_REG_BLUE, 0x40 },
	{ OV7670_REG_RED, 0x60 },
	{ OV7670_REG_COM8, COM8_FASTAEC|COM8_AECSTEP|COM8_AGC|COM8_AEC|COM8_AWB },

	/* Matrix coefficients */
	{ 0x4f, 0x80 },		{ 0x50, 0x80 },
	{ 0x51, 0 },		{ 0x52, 0x22 },
	{ 0x53, 0x5e },		{ 0x54, 0x80 },
	{ 0x58, 0x9e },

	{ OV7670_REG_COM16, COM16_AWBGAIN },	{ OV7670_REG_EDGE, 0 },
	{ 0x75, 0x05 },		{ OV7670_REG_REG76, 0xe1 },
	{ 0x4c, 0 },		{ 0x77, 0x01 },
	{ OV7670_REG_COM13, /*0xc3*/0x48 },	{ 0x4b, 0x09 },
	{ 0xc9, 0x60 },		/*{ OV7670_REG_COM16, 0x38 },*/
	{ 0x56, 0x40 },

	{ 0x34, 0x11 },		{ OV7670_REG_COM11, COM11_EXP|COM11_HZAUTO },
	{ 0xa4, 0x82/*Wax 0x88*/ },		{ 0x96, 0 },
	{ 0x97, 0x30 },		{ 0x98, 0x20 },
	{ 0x99, 0x30 },		{ 0x9a, 0x84 },
	{ 0x9b, 0x29 },		{ 0x9c, 0x03 },
	{ 0x9d, 0x4c },		{ 0x9e, 0x3f },
	{ 0x78, 0x04 },

	/* Extra-weird stuff.  Some sort of multiplexor register */
	{ 0x79, 0x01 },		{ 0xc8, 0xf0 },
	{ 0x79, 0x0f },		{ 0xc8, 0x00 },
	{ 0x79, 0x10 },		{ 0xc8, 0x7e },
	{ 0x79, 0x0a },		{ 0xc8, 0x80 },
	{ 0x79, 0x0b },		{ 0xc8, 0x01 },
	{ 0x79, 0x0c },		{ 0xc8, 0x0f },
	{ 0x79, 0x0d },		{ 0xc8, 0x20 },
	{ 0x79, 0x09 },		{ 0xc8, 0x80 },
	{ 0x79, 0x02 },		{ 0xc8, 0xc0 },
	{ 0x79, 0x03 },		{ 0xc8, 0x40 },
	{ 0x79, 0x05 },		{ 0xc8, 0x30 },
	{ 0x79, 0x26 },
	
	{ 0xff, 0xff }	// END MARKER
};

  
#endif /* _OV7670_REGS_H */


