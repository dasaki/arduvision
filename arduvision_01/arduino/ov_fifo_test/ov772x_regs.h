/*
 * A V4L2 driver for OmniVision ov7670 cameras.
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



#ifndef _OV772x_REGS_H
#define _OV772x_REGS_H


#include <stdint.h>
#include <avr/pgmspace.h>
#include "sensor.h"


#define OV772x_RD_ADDR  0x42
#define OV772x_WR_ADDR  0x43

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
 

// register offset
 
#define OV772x_REG_GAIN        0x00 // AGC - Gain control gain setting 
#define OV772x_REG_BLUE        0x01 // AWB - Blue channel gain setting 
#define OV772x_REG_RED         0x02 // AWB - Red   channel gain setting 
#define OV772x_REG_GREEN       0x03 // AWB - Green channel gain setting 
#define OV772x_REG_COM1        0x04 // Common control 1 
#define OV772x_REG_BAVG        0x05 // U/B Average Level 
#define OV772x_REG_GAVG        0x06 // Y/Gb Average Level 
#define OV772x_REG_RAVG        0x07 // V/R Average Level 
#define OV772x_REG_AECH        0x08 // Exposure Value - AEC MSBs 
#define OV772x_REG_COM2        0x09 // Common control 2 
#define OV772x_REG_PID         0x0A // Product ID Number MSB 
#define OV772x_REG_VER         0x0B // Product ID Number LSB 
#define OV772x_REG_COM3        0x0C // Common control 3 
#define OV772x_REG_COM4        0x0D // Common control 4 
#define OV772x_REG_COM5        0x0E // Common control 5 
#define OV772x_REG_COM6        0x0F // Common control 6 
#define OV772x_REG_AEC         0x10 // Exposure Value 
#define OV772x_REG_CLKRC       0x11 // Internal clock 
#define OV772x_REG_COM7        0x12 // Common control 7 
#define OV772x_REG_COM8        0x13 // Common control 8 
#define OV772x_REG_COM9        0x14 // Common control 9 
#define OV772x_REG_COM10       0x15 // Common control 10 
#define OV772x_REG_REG16       0x16 // Register 16 
#define OV772x_REG_HSTART      0x17 // Horizontal sensor size 
#define OV772x_REG_HSIZE       0x18 // Horizontal frame (HREF column) end high 8-bit 
#define OV772x_REG_VSTART      0x19 // Vertical frame (row) start high 8-bit 
#define OV772x_REG_VSIZE       0x1A // Vertical sensor size 
#define OV772x_REG_PSHFT       0x1B // Data format - pixel delay select 
#define OV772x_REG_MIDH        0x1C // Manufacturer ID byte - high 
#define OV772x_REG_MIDL        0x1D // Manufacturer ID byte - low  
#define OV772x_REG_LAEC        0x1F // Fine AEC value 
#define OV772x_REG_COM11       0x20 // Common control 11 
#define OV772x_REG_BDBASE      0x22 // Banding filter Minimum AEC value 
#define OV772x_REG_DBSTEP      0x23 // Banding filter Maximum Setp 
#define OV772x_REG_AEW         0x24 // AGC/AEC - Stable operating region (upper limit) 
#define OV772x_REG_AEB         0x25 // AGC/AEC - Stable operating region (lower limit) 
#define OV772x_REG_VPT         0x26 // AGC/AEC Fast mode operating region 
#define OV772x_REG_REG28       0x28 // Register 28 
#define OV772x_REG_HOUTSIZE    0x29 // Horizontal data output size MSBs 
#define OV772x_REG_EXHCH       0x2A // Dummy pixel insert MSB 
#define OV772x_REG_EXHCL       0x2B // Dummy pixel insert LSB 
#define OV772x_REG_VOUTSIZE    0x2C // Vertical data output size MSBs 
#define OV772x_REG_ADVFL       0x2D // LSB of insert dummy lines in Vertical direction 
#define OV772x_REG_ADVFH       0x2E // MSG of insert dummy lines in Vertical direction 
#define OV772x_REG_YAVE        0x2F // Y/G Channel Average value 
#define OV772x_REG_LUMHTH      0x30 // Histogram AEC/AGC Luminance high level threshold 
#define OV772x_REG_LUMLTH      0x31 // Histogram AEC/AGC Luminance low  level threshold 
#define OV772x_REG_HREF        0x32 // Image start and size control 
#define OV772x_REG_DM_LNL      0x33 // Dummy line low  8 bits 
#define OV772x_REG_DM_LNH      0x34 // Dummy line high 8 bits 
#define OV772x_REG_ADOFF_B     0x35 // AD offset compensation value for B  channel 
#define OV772x_REG_ADOFF_R     0x36 // AD offset compensation value for R  channel 
#define OV772x_REG_ADOFF_GB    0x37 // AD offset compensation value for Gb channel 
#define OV772x_REG_ADOFF_GR    0x38 // AD offset compensation value for Gr channel 
#define OV772x_REG_OFF_B       0x39 // Analog process B  channel offset value 
#define OV772x_REG_OFF_R       0x3A // Analog process R  channel offset value 
#define OV772x_REG_OFF_GB      0x3B // Analog process Gb channel offset value 
#define OV772x_REG_OFF_GR      0x3C // Analog process Gr channel offset value 
#define OV772x_REG_COM12       0x3D // Common control 12 
#define OV772x_REG_COM13       0x3E // Common control 13 
#define OV772x_REG_COM14       0x3F // Common control 14 
#define OV772x_REG_COM15       0x40 // Common control 15
#define OV772x_REG_COM16       0x41 // Common control 16 
#define OV772x_REG_TGT_B       0x42 // BLC blue channel target value 
#define OV772x_REG_TGT_R       0x43 // BLC red  channel target value 
#define OV772x_REG_TGT_GB      0x44 // BLC Gb   channel target value 
#define OV772x_REG_TGT_GR      0x45 // BLC Gr   channel target value 
// for ov7720 
#define OV772x_REG_LCC0        0x46 // Lens correction control 0 
#define OV772x_REG_LCC1        0x47 // Lens correction option 1 - X coordinate 
#define OV772x_REG_LCC2        0x48 // Lens correction option 2 - Y coordinate 
#define OV772x_REG_LCC3        0x49 // Lens correction option 3 
#define OV772x_REG_LCC4        0x4A // Lens correction option 4 - radius of the circular 
#define OV772x_REG_LCC5        0x4B // Lens correction option 5 
#define OV772x_REG_LCC6        0x4C // Lens correction option 6 
// for ov772x 
#define OV772x_REG_LC_CTR      0x46 // Lens correction control 
#define OV772x_REG_LC_XC       0x47 // X coordinate of lens correction center relative 
#define OV772x_REG_LC_YC       0x48 // Y coordinate of lens correction center relative 
#define OV772x_REG_LC_COEF     0x49 // Lens correction coefficient 
#define OV772x_REG_LC_RADI     0x4A // Lens correction radius 
#define OV772x_REG_LC_COEFB    0x4B // Lens B channel compensation coefficient 
#define OV772x_REG_LC_COEFR    0x4C // Lens R channel compensation coefficient 

#define OV772x_REG_FIXGAIN     0x4D // Analog fix gain amplifer 
#define OV772x_REG_AREF0       0x4E // Sensor reference control 
#define OV772x_REG_AREF1       0x4F // Sensor reference current control 
#define OV772x_REG_AREF2       0x50 // Analog reference control 
#define OV772x_REG_AREF3       0x51 // ADC    reference control 
#define OV772x_REG_AREF4       0x52 // ADC    reference control 
#define OV772x_REG_AREF5       0x53 // ADC    reference control 
#define OV772x_REG_AREF6       0x54 // Analog reference control 
#define OV772x_REG_AREF7       0x55 // Analog reference control 
#define OV772x_REG_UFIX        0x60 // U channel fixed value output 
#define OV772x_REG_VFIX        0x61 // V channel fixed value output 
#define OV772x_REG_AWBB_BLK    0x62 // AWB option for advanced AWB 
#define OV772x_REG_AWB_CTRL0   0x63 // AWB control byte 0 
#define OV772x_REG_DSP_CTRL1   0x64 // DSP control byte 1 
#define OV772x_REG_DSP_CTRL2   0x65 // DSP control byte 2 
#define OV772x_REG_DSP_CTRL3   0x66 // DSP control byte 3 
#define OV772x_REG_DSP_CTRL4   0x67 // DSP control byte 4 
#define OV772x_REG_AWB_BIAS    0x68 // AWB BLC level clip 
#define OV772x_REG_AWB_CTRL1   0x69 // AWB control  1 
#define OV772x_REG_AWB_CTRL2   0x6A // AWB control  2 
#define OV772x_REG_AWB_CTRL3   0x6B // AWB control  3 
#define OV772x_REG_AWB_CTRL4   0x6C // AWB control  4 
#define OV772x_REG_AWB_CTRL5   0x6D // AWB control  5 
#define OV772x_REG_AWB_CTRL6   0x6E // AWB control  6 
#define OV772x_REG_AWB_CTRL7   0x6F // AWB control  7 
#define OV772x_REG_AWB_CTRL8   0x70 // AWB control  8 
#define OV772x_REG_AWB_CTRL9   0x71 // AWB control  9 
#define OV772x_REG_AWB_CTRL10  0x72 // AWB control 10 
#define OV772x_REG_AWB_CTRL11  0x73 // AWB control 11 
#define OV772x_REG_AWB_CTRL12  0x74 // AWB control 12 
#define OV772x_REG_AWB_CTRL13  0x75 // AWB control 13 
#define OV772x_REG_AWB_CTRL14  0x76 // AWB control 14 
#define OV772x_REG_AWB_CTRL15  0x77 // AWB control 15 
#define OV772x_REG_AWB_CTRL16  0x78 // AWB control 16 
#define OV772x_REG_AWB_CTRL17  0x79 // AWB control 17 
#define OV772x_REG_AWB_CTRL18  0x7A // AWB control 18 
#define OV772x_REG_AWB_CTRL19  0x7B // AWB control 19 
#define OV772x_REG_AWB_CTRL20  0x7C // AWB control 20 
#define OV772x_REG_AWB_CTRL21  0x7D // AWB control 21 
#define OV772x_REG_GAM1        0x7E // Gamma Curve  1st segment input end point 
#define OV772x_REG_GAM2        0x7F // Gamma Curve  2nd segment input end point 
#define OV772x_REG_GAM3        0x80 // Gamma Curve  3rd segment input end point 
#define OV772x_REG_GAM4        0x81 // Gamma Curve  4th segment input end point 
#define OV772x_REG_GAM5        0x82 // Gamma Curve  5th segment input end point 
#define OV772x_REG_GAM6        0x83 // Gamma Curve  6th segment input end point 
#define OV772x_REG_GAM7        0x84 // Gamma Curve  7th segment input end point 
#define OV772x_REG_GAM8        0x85 // Gamma Curve  8th segment input end point 
#define OV772x_REG_GAM9        0x86 // Gamma Curve  9th segment input end point 
#define OV772x_REG_GAM10       0x87 // Gamma Curve 10th segment input end point 
#define OV772x_REG_GAM11       0x88 // Gamma Curve 11th segment input end point 
#define OV772x_REG_GAM12       0x89 // Gamma Curve 12th segment input end point 
#define OV772x_REG_GAM13       0x8A // Gamma Curve 13th segment input end point 
#define OV772x_REG_GAM14       0x8B // Gamma Curve 14th segment input end point 
#define OV772x_REG_GAM15       0x8C // Gamma Curve 15th segment input end point 
#define OV772x_REG_SLOP        0x8D // Gamma curve highest segment slope 
#define OV772x_REG_DNSTH       0x8E // De-noise threshold 
#define OV772x_REG_EDGE_STRNGT 0x8F // Edge strength  control when manual mode 
#define OV772x_REG_EDGE_TRSHLD 0x90 // Edge threshold control when manual mode 
#define OV772x_REG_DNSOFF      0x91 // Auto De-noise threshold control 
#define OV772x_REG_EDGE_UPPER  0x92 // Edge strength upper limit when Auto mode 
#define OV772x_REG_EDGE_LOWER  0x93 // Edge strength lower limit when Auto mode 
#define OV772x_REG_MTX1        0x94 // Matrix coefficient 1 
#define OV772x_REG_MTX2        0x95 // Matrix coefficient 2 
#define OV772x_REG_MTX3        0x96 // Matrix coefficient 3 
#define OV772x_REG_MTX4        0x97 // Matrix coefficient 4 
#define OV772x_REG_MTX5        0x98 // Matrix coefficient 5 
#define OV772x_REG_MTX6        0x99 // Matrix coefficient 6 
#define OV772x_REG_MTX_CTRL    0x9A // Matrix control 
#define OV772x_REG_BRIGHT      0x9B // Brightness control 
#define OV772x_REG_CNTRST      0x9C // Contrast contrast 
#define OV772x_REG_CNTRST_CTRL 0x9D // Contrast contrast center 
#define OV772x_REG_UVAD_J0     0x9E // Auto UV adjust contrast 0 
#define OV772x_REG_UVAD_J1     0x9F // Auto UV adjust contrast 1 
#define OV772x_REG_SCAL0       0xA0 // Scaling control 0 
#define OV772x_REG_SCAL1       0xA1 // Scaling control 1 
#define OV772x_REG_SCAL2       0xA2 // Scaling control 2 
#define OV772x_REG_FIFODLYM    0xA3 // FIFO manual mode delay control 
#define OV772x_REG_FIFODLYA    0xA4 // FIFO auto   mode delay control 
#define OV772x_REG_SDE         0xA6 // Special digital effect control 
#define OV772x_REG_USAT        0xA7 // U component saturation control 
#define OV772x_REG_VSAT        0xA8 // V component saturation control 
// for ov7720 
#define OV772x_REG_HUE0        0xA9 // Hue control 0 
#define OV772x_REG_HUE1        0xAA // Hue control 1 
// for ov772x 
#define OV772x_REG_HUECOS      0xA9 // Cosine value 
#define OV772x_REG_HUESIN      0xAA // Sine value 

#define OV772x_REG_SIGN        0xAB // Sign bit for Hue and contrast 
#define OV772x_REG_DSPAUTO     0xAC // DSP auto function ON/OFF control 

// register detail
 

// COM2 
#define OV772x_REG_SOFT_SLEEP_MODE 0x10	// Soft sleep mode 
				// Output drive capability 
#define OV772x_REG_OCAP_1x         0x00	// 1x 
#define OV772x_REG_OCAP_2x         0x01	// 2x 
#define OV772x_REG_OCAP_3x         0x02	// 3x 
#define OV772x_REG_OCAP_4x         0x03	// 4x 

// COM3 
#define OV772x_REG_SWAP_MASK       (SWAP_RGB | SWAP_YUV | SWAP_ML)
#define OV772x_REG_IMG_MASK        (VFLIP_IMG | HFLIP_IMG)

#define OV772x_REG_VFLIP_IMG       0x80	// Vertical flip image ON/OFF selection 
#define OV772x_REG_HFLIP_IMG       0x40	// Horizontal mirror image ON/OFF selection 
#define OV772x_REG_SWAP_RGB        0x20	// Swap B/R  output sequence in RGB mode 
#define OV772x_REG_SWAP_YUV        0x10	// Swap Y/UV output sequence in YUV mode 
#define OV772x_REG_SWAP_ML         0x08	// Swap output MSB/LSB 
				// Tri-state option for output clock 
#define OV772x_REG_NOTRI_CLOCK     0x04	//   0: Tri-state    at this period 
				//   1: No tri-state at this period 
				// Tri-state option for output data 
#define OV772x_REG_NOTRI_DATA      0x02	//   0: Tri-state    at this period 
				//   1: No tri-state at this period 
#define OV772x_REG_SCOLOR_TEST     0x01	// Sensor color bar test pattern 

// COM4 
				// PLL frequency control 
#define OV772x_REG_PLL_BYPASS      0x00	//  00: Bypass PLL 
#define OV772x_REG_PLL_4x          0x40	//  01: PLL 4x 
#define OV772x_REG_PLL_6x          0x80	//  10: PLL 6x 
#define OV772x_REG_PLL_8x          0xc0	//  11: PLL 8x 
				// AEC evaluate window 
#define OV772x_REG_AEC_FULL        0x00	//  00: Full window 
#define OV772x_REG_AEC_1p2         0x10	//  01: 1/2  window 
#define OV772x_REG_AEC_1p4         0x20	//  10: 1/4  window 
#define OV772x_REG_AEC_2p3         0x30	//  11: Low 2/3 window 

// COM5 
#define OV772x_REG_AFR_ON_OFF      0x80	// Auto frame rate control ON/OFF selection 
#define OV772x_REG_AFR_SPPED       0x40	// Auto frame rate control speed selection 
				// Auto frame rate max rate control 
#define OV772x_REG_AFR_NO_RATE     0x00	//     No  reduction of frame rate 
#define OV772x_REG_AFR_1p2         0x10	//     Max reduction to 1/2 frame rate 
#define OV772x_REG_AFR_1p4         0x20	//     Max reduction to 1/4 frame rate 
#define OV772x_REG_AFR_1p8         0x30	// Max reduction to 1/8 frame rate 
				// Auto frame rate active point control 
#define OV772x_REG_AF_2x           0x00	//     Add frame when AGC reaches  2x gain 
#define OV772x_REG_AF_4x           0x04	//     Add frame when AGC reaches  4x gain 
#define OV772x_REG_AF_8x           0x08	//     Add frame when AGC reaches  8x gain 
#define OV772x_REG_AF_16x          0x0c	// Add frame when AGC reaches 16x gain 
				// AEC max step control 
#define OV772x_REG_AEC_NO_LIMIT    0x01	//   0 : AEC incease step has limit 
				//   1 : No limit to AEC increase step 

// COM7 
				// SCCB Register Reset 
#define OV772x_REG_SCCB_RESET      0x80	//   0 : No change 
				//   1 : Resets all registers to default 
				// Resolution selection 
#define OV772x_REG_SLCT_MASK       0x40	//   Mask of VGA or QVGA 
#define OV772x_REG_SLCT_VGA        0x00	//   0 : VGA 
#define OV772x_REG_SLCT_QVGA       0x40	//   1 : QVGA 
#define OV772x_REG_ITU656_ON_OFF   0x20	// ITU656 protocol ON/OFF selection 
#define OV772x_REG_SENSOR_RAW	0x10	// Sensor RAW 
				// RGB output format control 
#define OV772x_REG_FMT_MASK        0x0c	//      Mask of color format 
#define OV772x_REG_FMT_GBR422      0x00	//      00 : GBR 4:2:2 
#define OV772x_REG_FMT_RGB565      0x04	//      01 : RGB 565 
#define OV772x_REG_FMT_RGB555      0x08	//      10 : RGB 555 
#define OV772x_REG_FMT_RGB444      0x0c	// 11 : RGB 444 
				// Output format control 
#define OV772x_REG_OFMT_MASK       0x03    //      Mask of output format 
#define OV772x_REG_OFMT_YUV        0x00	//      00 : YUV 
#define OV772x_REG_OFMT_P_BRAW     0x01	//      01 : Processed Bayer RAW 
#define OV772x_REG_OFMT_RGB        0x02	//      10 : RGB 
#define OV772x_REG_OFMT_BRAW       0x03	// 11 : Bayer RAW 

// COM8 
#define OV772x_REG_FAST_ALGO       0x80	// Enable fast AGC/AEC algorithm 
				// AEC Setp size limit 
#define OV772x_REG_UNLMT_STEP      0x40	//   0 : Step size is limited 
				//   1 : Unlimited step size 
#define OV772x_REG_BNDF_ON_OFF     0x20	// Banding filter ON/OFF 
#define OV772x_REG_AEC_BND         0x10	// Enable AEC below banding value 
#define OV772x_REG_AEC_ON_OFF      0x08	// Fine AEC ON/OFF control 
#define OV772x_REG_AGC_ON          0x04	// AGC Enable 
#define OV772x_REG_AWB_ON          0x02	// AWB Enable 
#define OV772x_REG_AEC_ON          0x01	// AEC Enable 

// COM9 
#define OV772x_REG_BASE_AECAGC     0x80	// Histogram or average based AEC/AGC 
				// Automatic gain ceiling - maximum AGC value 
#define OV772x_REG_GAIN_2x         0x00	//    000 :   2x 
#define OV772x_REG_GAIN_4x         0x10	//    001 :   4x 
#define OV772x_REG_GAIN_8x         0x20	//    010 :   8x 
#define OV772x_REG_GAIN_16x        0x30	//    011 :  16x 
#define OV772x_REG_GAIN_32x        0x40	//    100 :  32x 
#define OV772x_REG_GAIN_64x        0x50	// 101 :  64x 
#define OV772x_REG_GAIN_128x       0x60	// 110 : 128x 
#define OV772x_REG_DROP_VSYNC      0x04	// Drop VSYNC output of corrupt frame 
#define OV772x_REG_DROP_HREF       0x02	// Drop HREF  output of corrupt frame 

// COM11 
#define OV772x_REG_SGLF_ON_OFF     0x02	// Single frame ON/OFF selection 
#define OV772x_REG_SGLF_TRIG       0x01	// Single frame transfer trigger 

// HREF 
#define OV772x_REG_HREF_VSTART_SHIFT	6	// VSTART LSB 
#define OV772x_REG_HREF_HSTART_SHIFT	4	// HSTART 2 LSBs 
#define OV772x_REG_HREF_VSIZE_SHIFT	2	// VSIZE LSB 
#define OV772x_REG_HREF_HSIZE_SHIFT	0	// HSIZE 2 LSBs 

// EXHCH 
#define OV772x_REG_EXHCH_VSIZE_SHIFT	2	// VOUTSIZE LSB 
#define OV772x_REG_EXHCH_HSIZE_SHIFT	0	// HOUTSIZE 2 LSBs 

// DSP_CTRL1 
#define OV772x_REG_FIFO_ON         0x80	// FIFO enable/disable selection 
#define OV772x_REG_UV_ON_OFF       0x40	// UV adjust function ON/OFF selection 
#define OV772x_REG_YUV444_2_422    0x20	// YUV444 to 422 UV channel option selection 
#define OV772x_REG_CLR_MTRX_ON_OFF 0x10	// Color matrix ON/OFF selection 
#define OV772x_REG_INTPLT_ON_OFF   0x08	// Interpolation ON/OFF selection 
#define OV772x_REG_GMM_ON_OFF      0x04	// Gamma function ON/OFF selection 
#define OV772x_REG_AUTO_BLK_ON_OFF 0x02	// Black defect auto correction ON/OFF 
#define OV772x_REG_AUTO_WHT_ON_OFF 0x01	// White define auto correction ON/OFF 

// DSP_CTRL3 
#define OV772x_REG_UV_MASK         0x80	// UV output sequence option 
#define OV772x_REG_UV_ON           0x80	//   ON 
#define OV772x_REG_UV_OFF          0x00	//   OFF 
#define OV772x_REG_CBAR_MASK       0x20	// DSP Color bar mask 
#define OV772x_REG_CBAR_ON         0x20	//   ON 
#define OV772x_REG_CBAR_OFF        0x00	//   OFF 

// DSP_CTRL4 
#define OV772x_REG_DSP_OFMT_YUV	0x00
#define OV772x_REG_DSP_OFMT_RGB	0x00
#define OV772x_REG_DSP_OFMT_RAW8	0x02
#define OV772x_REG_DSP_OFMT_RAW10	0x03

// DSPAUTO (DSP Auto Function ON/OFF Control) 
#define OV772x_REG_AWB_ACTRL       0x80 // AWB auto threshold control 
#define OV772x_REG_DENOISE_ACTRL   0x40 // De-noise auto threshold control 
#define OV772x_REG_EDGE_ACTRL      0x20 // Edge enhancement auto strength control 
#define OV772x_REG_UV_ACTRL        0x10 // UV adjust auto slope control 
#define OV772x_REG_SCAL0_ACTRL     0x08 // Auto scaling factor control 
#define OV772x_REG_SCAL1_2_ACTRL   0x04 // Auto scaling factor control 



const struct regval_list qqvga_yuv_ov772x[] PROGMEM = {

   {OV772x_REG_COM7, OV772x_REG_SLCT_VGA}, // QVGA, YUY
   {OV772x_REG_COM12, 0x03}, // DC offset compensation for analog process
   {0x11, 0x01}, // 00/01/03/07 for 60/30/15/7.5fps - set to 30fps for QQVGA

   {OV772x_REG_DSP_CTRL2, 0x2f}, // DCW enable, zoom out enable

  // 160x120
   {0xA0, 0x0a}, // SCAL0, 1/4 vertical & horizontal down sampling
   {0xA1, 0x40}, // SCAL1, Horizontal Zoom Out Control, set ratio to 1/1   
   {0xA2, 0x40}, // SCAL2, Vertical Zoom Out Control, set ratio to 1/1   
  
   {OV772x_REG_HSTART, 0x23},
   {OV772x_REG_HSIZE, 0xa0}, // Horizontal Sensor Size: full VGA (no windowing)
   {0x29, 0x28}, // Horizontal Data Output Size (160px) MSBs
   {OV772x_REG_VSTART, 0x07},
   {OV772x_REG_VSIZE, 0xf0}, // Vertical Sensor Size: full VGA  (no windowing)
   {0x2c, 0x3c}, // Vertical Data Output Size (120px)MSBs
   {OV772x_REG_HREF, 0x00},
   {0x2a, 0x00}, // EXHCH
   {0x15,0x01}, // Output data range selection, Data from [10] to [F0] (8 MSBs)

   { 0xff, 0xff }	// END MARKER

};

const struct regval_list qqqvga_yuv_ov772x[] PROGMEM = {

  {OV772x_REG_COM7, OV772x_REG_SLCT_VGA}, // QVGA, YUY
  {OV772x_REG_COM12, 0x03}, // DC offset compensation for analog process
  {0x11, 0x01}, // 00/01/03/07 for 60/30/15/7.5fps - set to 30fps for QQVGA
  
  {OV772x_REG_DSP_CTRL2, 0x0f}, // 0x2f // DCW enable, zoom out enable
  
  {0xA0, 0x0f}, // SCAL0, 1/8 vertical & horizontal down sampling
  {0xA1, 0x80}, // SCAL1, Horizontal Zoom Out Control, set ratio to 1/2   
  {0xA2, 0x80}, // SCAL2, Vertical Zoom Out Control, set ratio to 1/2  

  {OV772x_REG_HSTART, 0x23},
  {OV772x_REG_HSIZE, 0xa0}, // Horizontal Sensor Size: full VGA (no windowing)
  {0x29, 0x14}, // Horizontal Data Output Size (80px) MSBs
  {OV772x_REG_VSTART, 0x07},
  {OV772x_REG_VSIZE, 0xf0}, // Vertical Sensor Size: full VGA  (no windowing)
  {0x2c, 0x22}, // Vertical Data Output Size (60px)MSBs
  {OV772x_REG_HREF, 0x00},
  {0x2a, 0x00}, // EXHCH
  
  {0x15,0x01}, // Output data range selection, Data from [10] to [F0] (8 MSBs)
  { 0xff, 0xff }	// END MARKER

};


const struct regval_list common_reglist_ov772x[] PROGMEM =
{
  
  // most of these are color correction stuff
{0x42, 0x7f},
{0x4d, 0x09},
{0x63, 0xe0}, // AWB_Ctrl0
{OV772x_REG_COM3, 0x00}, // flip Y with UV
{OV772x_REG_DSP_CTRL1, 0xff},
{OV772x_REG_DSP_CTRL3, 0x00}, // flip Y with UV
{OV772x_REG_DSP_CTRL4, 0x48},
{0x13, 0xf0},
{0x0d, 0x61}, // 51/61/71 for different AEC/AGC window
{0x0f, 0xc5},
{0x14, 0x11},
{0x22, 0x7f}, // ff/7f/3f/1f for 60/30/15/7.5fps
{0x23, 0x03}, // 01/03/07/0f for 60/30/15/7.5fps
{0x24, 0x40},
{0x25, 0x30},
{0x26, 0xa1},
{0x2b, 0x00}, // 00/9e for 60/50Hz
{0x6b, 0xaa},
{0x13, 0xff},
{0x90, 0x05},
{0x91, 0x01},
{0x92, 0x03},
{0x93, 0x00},
{0x94, 0xb0},
{0x95, 0x9d},
{0x96, 0x13},
{0x97, 0x16},
{0x98, 0x7b},
{0x99, 0x91},
{0x9a, 0x1e},
{0x9e, 0x81},
//{0xa6, 0x03}, // Special Digital Effect Control; luma, contrast, saturation hue
//{0x9b,0x00}, // set luma             
//{0x9c,0x25}, // set contrast        
//{0xa7,0x65}, // set saturation   
//{0xa8,0x65}, // set saturation   
//{0xa9,0x80}, // set hue          
//{0xaa,0x80}, // set hue  
{0x7e, 0x0c},
{0x7f, 0x16},
{0x80, 0x2a},
{0x81, 0x4e},
{0x82, 0x61},
{0x83, 0x6f},
{0x84, 0x7b},
{0x85, 0x86},
{0x86, 0x8e},
{0x87, 0x97},
{0x88, 0xa4},
{0x89, 0xaf},
{0x8a, 0xc5},
{0x8b, 0xd7},
{0x8c, 0xe8},
{0x8d, 0x20},
{0xff, 0xff}	// END MARKER
};
  

  
#endif /* _OV772x_REGS_H */


