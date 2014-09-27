
//**************************


//**************************


#include "sensor.h"
#include "ov772x_regs.h"
#include "ov7670_regs.h"


#include <Arduino.h>
#include <Wire.h>

#include "delay.h"

uint16_t sensor_init(frameFormat_t fFormat)
{
    regval_list *format_reglist, *common_reglist;
    uint8_t resetReg, resetCommand;
    
    Wire.begin();

    _delayMilliseconds(5);
 
    uint16_t productID = sensor_readReg(REG_PID);
    switch(productID) {
      case 0x76:  //ov7670
                  productID <<= 8;
                  productID |= sensor_readReg(REG_VER);
                  switch (fFormat) {
                      default: 
                      case FF_QQVGA: format_reglist = (regval_list*)qqvga_yuv_ov7670; break;
                      case FF_QQQVGA: format_reglist = (regval_list*)qqqvga_yuv_ov7670; break;
                  }
                  common_reglist = (regval_list*)common_reglist_ov7670;
                  resetReg = 0x12;
                  resetCommand = 0x80;
                  break;
      case 0x77:  //ov772x
                  productID <<= 8;
                  productID |= sensor_readReg(REG_VER);
                  switch (fFormat) {
                      default: 
                      case FF_QQVGA: format_reglist = (regval_list*)qqvga_yuv_ov772x; break;
                      case FF_QQQVGA: format_reglist = (regval_list*)qqqvga_yuv_ov772x; break;
                  }
                  common_reglist = (regval_list*)common_reglist_ov772x;
                  resetReg = 0x12;
                  resetCommand = 0x80;
                  break;
      default:    return 0;
                  break;
    }
    
    sensor_writeReg(resetReg, resetCommand); // reset to default values
    delay(300); // Setting time for register change
    sensor_writeRegs(common_reglist);
    delay(300); // Setting time for register change
    sensor_writeRegs(format_reglist); 
    delay(300); // Setting time for register change
    return productID;
}
//**************************
// Write byte value regDat to the camera register addressed by regID 
uint8_t sensor_writeReg(uint8_t regID, uint8_t regDat) {
    Wire.beginTransmission(OV772x_WR_ADDR >> 1);
    Wire.write(regID & 0x00FF);    
    Wire.write(regDat & 0x00FF);    
    uint8_t result = Wire.endTransmission();
    //delay(1);
    return result;
}
//**************************
void sensor_writeRegs(const regval_list reglist[]) {
    uint8_t reg_addr;
	uint8_t reg_val;
	const struct regval_list *next = reglist;
	while ((reg_addr != 0xFF) | (reg_val != 0xFF)){
		reg_addr = pgm_read_byte(&next->reg_num);
		reg_val = pgm_read_byte(&next->value);
		sensor_writeReg(reg_addr, reg_val);
	   	next++;              
	}
}
 //**************************
// Read a byte value from the camera addressed at regID, and store it at
// memory location pointed to by regDat. Return 1 on success, 0 on failure.
uint8_t sensor_readReg(uint8_t regID) {
   uint8_t regDat;
    Wire.beginTransmission(OV772x_RD_ADDR >> 1);
    Wire.write(regID & 0x00FF);    
    Wire.endTransmission();
    Wire.requestFrom((OV772x_RD_ADDR >> 1),1);
    if(Wire.available()) {
        regDat = Wire.read();
         //delay(1);
        return regDat;
    } else {
        return 0;
    }
}

//**************************
void sensor_printlnRegs(const regval_list reglist[], Stream &destPort) {
    uint8_t reg_addr;
	uint8_t reg_val;
	uint8_t regRead_val;

	const struct regval_list *next = reglist;
	while ((reg_addr != 0xFF) | (reg_val != 0xFF)){
		reg_addr = pgm_read_byte(&next->reg_num);
		reg_val = pgm_read_byte(&next->value);
		regRead_val = sensor_readReg(reg_addr);
	   	next++;
                destPort.print("{");
                destPort.print(reg_addr, HEX);
                destPort.print(", ");
                destPort.print(regRead_val, HEX);
                destPort.println("},");
                
	}
}



