/**************************************************************
 *
 *  Part of the ARDUVISION project
 *
 *  by David Sanz Kirbis
 *
 *  Receive the image and/or tracking data from a CMOS sensor
 *  controlled by an Arduino board.
 *  In the arduino side use the example sketch "ov_fifo_test.ino"
 *
 *  More info: http://www.arduvision.com
 *
 *
 *
 **************************************************************/


import processing.serial.*;
import java.util.*;



Serial serialPort;     

request_t       request;
requestStatus_t reqStatus = requestStatus_t.IDLE;

// incoming serial 
byte[]   ackBuff = new byte[G_DEF.ACK_BUF_LEN];
byte[][] pix     = new byte[G_DEF.F_H][G_DEF.MAX_ROW_LEN];

double waitTimeout    = 0;
double fpsTimeStamp   = 0;

PFont  myFont;
PImage currFrame;

boolean bSerialDebug = true;

int currRow = 0;
int thresh  = 30;

float tmp_x0 = 0, tmp_y0 = 0, tmp_x1 = 0, tmp_y1 = 0;
float x0 = 0, y0 = 0, x1 = 0, y1 = 0;

boolean bKalmanEnabled = true;
simpleKalman filter_x0 = new simpleKalman();
simpleKalman filter_y0 = new simpleKalman();
simpleKalman filter_x1 = new simpleKalman();
simpleKalman filter_y1 = new simpleKalman();
  
// ************************************************************
//                          SETUP
// ************************************************************
 
void setup() {
  size(G_DEF.SCR_W, G_DEF.SCR_H+G_DEF.FONT_BKG_SIZE);
  frameRate(30);
  
  currFrame = createImage(G_DEF.F_W, G_DEF.F_H, RGB);
  
  
  // create a font with the third font available to the system:
  myFont = createFont("Arial", G_DEF.FONT_SIZE);
  textFont(myFont);

  // List all the available serial serialPorts:
  printArray(Serial.list());

  delay(500);
  serialPort = new Serial(this, "/dev/ttyUSB0", G_DEF.BAUDRATE);
  serialPort.bufferUntil(G_DEF.LF);
  serialPort.clear();
  delay(2000);
  reqStatus = reqStatus.IDLE;
  request = request.NONE;
}
  
// ************************************************************
//                          DRAW
// ************************************************************
void draw() {
  switch (request) {
    case NONE:        reqStatus = requestStatus_t.IDLE;
                      background(0);
                      drawInfo();
                       break;
    case TRACKDARK: 
    case TRACKBRIG:    switch (reqStatus) {
                            case RECEIVED:  drawTracking();
                                            drawFPS();
                                            reqStatus = requestStatus_t.IDLE;
                                            // continue
                            case TIMEOUT:   
                            case IDLE:      reqTracking(request);
                                            reqStatus = requestStatus_t.REQUESTED; 
                                            waitTimeout = G_DEF.SERIAL_TIMEOUT + millis();
                                            break;
                            case REQUESTED:  if (millis() > waitTimeout) reqStatus = requestStatus_t.TIMEOUT;
                                            break;
                            case ARRIVING:  break;
                            default : break;
                       }
                       drawInfo();
                       break;
     case STREAM0PPB:
     case STREAM1PPB:
     case STREAM2PPB:
     case STREAM4PPB:
     case STREAM8PPB:  switch (reqStatus) {
                            case RECEIVED:  reqStatus = requestStatus_t.PROCESSING;
                                            buff2pixFrame(pix, currFrame, request);
                                            noSmooth();
                                            image(currFrame,0,0, G_DEF.F_W*G_DEF.DRAW_SCALE,G_DEF.F_H*G_DEF.DRAW_SCALE);
                                            smooth();
                                            drawFPS();
                                            reqStatus = requestStatus_t.IDLE;
                                            break;
                            case TIMEOUT:   
                            case IDLE:      reqImage(request);
                                            reqStatus = requestStatus_t.REQUESTED; 
                                            waitTimeout = G_DEF.SERIAL_TIMEOUT + millis();
                                            break;
                            case REQUESTED:  if (millis() > waitTimeout) reqStatus = requestStatus_t.TIMEOUT;
                                            break;
                            case ARRIVING:  break;
                            case PROCESSING:  break;
                            default : break;
                          }   
                      drawInfo();
                      break;
      default :        break;
    }                                       
}

// ************************************************************
//                     SERIAL EVENT HANDLER
// ************************************************************
void serialEvent(Serial serialPort) {

    if (reqStatus == requestStatus_t.REQUESTED) {
          if ( (serialPort.readBytesUntil(G_DEF.LF, ackBuff) >= 3) && 
               (ackBuff[0] == 'A') &&
               (ackBuff[1] == 'C') &&
               (ackBuff[2] == 'K') ) {
            reqStatus = requestStatus_t.ARRIVING;
          } else if (millis() > waitTimeout) {
            reqStatus = requestStatus_t.TIMEOUT;
          }
    }
    else if (reqStatus == requestStatus_t.ARRIVING) {
        parseSerialData();
    }
    else {
      if (bSerialDebug) print(serialPort.readStringUntil(G_DEF.LF));
    }
}

// ************************************************************
//                       PARSE SERIAL DATA
// ************************************************************
void parseSerialData() {
  switch (request) {
      case NONE:         break;
      case TRACKDARK: 
      case TRACKBRIG:     tmp_x0 = serialPort.read();
                          tmp_y0 = serialPort.read();
                          tmp_x1 = serialPort.read();
                          tmp_y1 = serialPort.read();
                         // if (bSerialDebug) println("x0: "+tmp_x0+"   y0: "+tmp_y0+"    x1: "+tmp_x1+"   y1: "+tmp_y1);
                          reqStatus = requestStatus_t.RECEIVED;
                          break;
       case STREAM0PPB:
       case STREAM1PPB:
       case STREAM2PPB:
       case STREAM4PPB:
       case STREAM8PPB:   serialPort.readBytes(pix[currRow]);
                          serialPort.clear();
                          currRow++;
                          if (currRow >= G_DEF.F_H) {
                              reqStatus = requestStatus_t.RECEIVED; // frame ready on buffer
                              currRow = 0;
                              if (bSerialDebug) println();
                          }
                        break;
        default :       break;
    }      
    
}
  
// ************************************************************
//                      REQUEST IMAGE
// ************************************************************
void reqImage(request_t req) {
      currRow = 0;
      serialPort.clear();
      serialPort.write("send "+Integer.toString(req.getParam())+G_DEF.LF);
}
  
// ************************************************************
//                  REQUEST TRACKING DATA
// ************************************************************
void reqTracking(request_t req) {
    
      serialPort.clear();
      if (req == request_t.TRACKDARK)
          serialPort.write("dark "+Integer.toString(225)+G_DEF.LF);
      else if (req == request_t.TRACKBRIG)
          serialPort.write("brig "+Integer.toString(225)+G_DEF.LF);
       
}
  
// ************************************************************
//                      DRAW SCREEN INFO
// ************************************************************
void drawInfo() {
  String modeStr = "Press space to toggle MODE: "+request;
   pushStyle();
   pushMatrix();
   noStroke();
   fill(0);
   rect(0, height-G_DEF.FONT_BKG_SIZE, width, G_DEF.FONT_BKG_SIZE);
   fill(255);
   textAlign(LEFT, TOP);
   text(modeStr, 20, height-G_DEF.FONT_BKG_SIZE);
   popMatrix();
   popStyle();
}

// ************************************************************
//                           DRAW FPS
// ************************************************************
void drawFPS() {
   long currTime = millis();
   float fps =1000.0/(float)(currTime-fpsTimeStamp);
   
   pushStyle();
   pushMatrix();
   noStroke();
   fill(0);
   rect(20, 20, textWidth("FPS: "+fps), G_DEF.FONT_SIZE);
   fill(255);
   translate(0,-2);
   textAlign(LEFT, TOP);
   text("FPS: "+fps, 20, 20);
   popMatrix();
   popStyle();
   fpsTimeStamp = currTime;
}
  
// ************************************************************
//                      DRAW TRACKING
// ************************************************************
void drawTracking() {
   pushStyle();
   noFill(); 
   strokeWeight(3);
   
   if (tmp_x1-tmp_x0 > 5 && tmp_x1-tmp_x0 < 60 && tmp_y1-tmp_y0 < 60 && tmp_y1-tmp_y0 > 5) {
      stroke(0);  
      rect((float)x0*G_DEF.DRAW_SCALE,(float)y0*G_DEF.DRAW_SCALE, (float)(x1-x0)*G_DEF.DRAW_SCALE, (float)(y1-y0)*G_DEF.DRAW_SCALE );
         if (bKalmanEnabled) {
             x0 = (float)filter_x0.update(tmp_x0);
             y0 = (float)filter_y0.update(tmp_y0);
             x1 = (float)filter_x1.update(tmp_x1);
             y1 = (float)filter_y1.update(tmp_y1);
         }
         else {
             x0 = tmp_x0;
             y0 = tmp_y0;
             x1 = tmp_x1;
             y1 = tmp_y1;
         }
         stroke(255);         
         rect( x0*G_DEF.DRAW_SCALE, y0*G_DEF.DRAW_SCALE,
               (x1-x0)*G_DEF.DRAW_SCALE, (y1-y0)*G_DEF.DRAW_SCALE );
   }
   noStroke();
   popStyle();
}

// ************************************************************
//              CONVERT PIXEL STREAM BUFFER TO PIMAGE
// ************************************************************
void buff2pixFrame(byte[][] pixBuff, PImage dstImg, request_t req) {
 
  int Y0 = 0, U = 0, Y1 = 0, V = 0;
  int Y2 = 0, Y3 = 0, Y4 = 0;
  int reqParam = req.getParam();
  
  dstImg.loadPixels();
  
  switch (req) {
     case STREAM0PPB: for (int y = 0, l = 0, x = 0; y < G_DEF.F_H; y++, x = 0)
                         while (x < reqParam) {
                            Y0 = int(pixBuff[y][x++]);
                            U  = int(pixBuff[y][x++]);
                            Y1 = int(pixBuff[y][x++]);
                            V  = int(pixBuff[y][x++]);
                            dstImg.pixels[l++] = YUV2RGB(Y0,U,V);
                            dstImg.pixels[l++] = YUV2RGB(Y1,U,V);
                         }
                      break;
     case STREAM1PPB: for (int y = 0, l = 0, x = 0; y < G_DEF.F_H; y++, x = 0)
                         while (x < reqParam) {
                           Y0 = 0x08 | ((0x0f & int(pixBuff[y][x])) << 4);
                           U  = 0x00 | ((0xf0 & int(pixBuff[y][x++])));
                           Y1 = 0x08 | ((0x0f & int(pixBuff[y][x])) << 4);
                           V  = 0x00 | ((0xf0 & int(pixBuff[y][x++])));
                           dstImg.pixels[l++] = YUV2RGB(Y0,U,V);
                           dstImg.pixels[l++] = YUV2RGB(Y1,U,V);
                         }
                       break;
     case STREAM2PPB:  for (int y = 0, l = 0, x = 0; y < G_DEF.F_H; y++, x = 0)
                         while (x < reqParam) {
                          Y1 = int(pixBuff[y][x++]);
                          Y0 = ((Y1 << 4) & 0xF0) | 0x1f;
                          Y1 = (Y1 & 0xF0)  | 0x1f;
                          dstImg.pixels[l++] = color(Y0);
                          dstImg.pixels[l++] = color(Y1);
                       }
                       break;
     case STREAM4PPB:  for (int y = 0, l = 0, x = 0; y < G_DEF.F_H; y++, x = 0)
                         while (x < reqParam) {
                          Y3 = int(pixBuff[y][x++]);
                          Y0 = ((Y3 << 6) & 0xC0) | 0x1f;
                          Y1 = ((Y3 << 4) & 0xC0) | 0x1f;
                          Y2 = ((Y3 << 2) & 0xC0) | 0x1f;
                          Y3 = (Y3 & 0xC0) | 0x1f;
                          dstImg.pixels[l++] = color(Y0);
                          dstImg.pixels[l++] = color(Y1);
                          dstImg.pixels[l++] = color(Y2);
                          dstImg.pixels[l++] = color(Y3);
                       }
                       break;
     case STREAM8PPB:  for (int y = 0, l = 0, x = 0; y < G_DEF.F_H; y++, x = 0)
                         while (x < reqParam) {
                          Y0 = int(pixBuff[y][x++]);

                          dstImg.pixels[l++] = ((Y0 & 0x01) == 0? 0:0xffffff);
                          dstImg.pixels[l++] = ((Y0 & 0x02) == 0? 0:0xffffff);
                          dstImg.pixels[l++] = ((Y0 & 0x04) == 0? 0:0xffffff);
                          dstImg.pixels[l++] = ((Y0 & 0x05) == 0? 0:0xffffff);
                          dstImg.pixels[l++] = ((Y0 & 0x10) == 0? 0:0xffffff);
                          dstImg.pixels[l++] = ((Y0 & 0x20) == 0? 0:0xffffff);
                          dstImg.pixels[l++] = ((Y0 & 0x40) == 0? 0:0xffffff);
                          dstImg.pixels[l++] = ((Y0 & 0x80) == 0? 0:0xffffff);
                       }
                       break;
  }
  dstImg.updatePixels();  
}
  
// ************************************************************
//                     YUV TO RGB
// ************************************************************

color YUV2RGB(int Y, int Cb, int Cr) {
     // from OV7670 Software Application note
     float R = Y + 1.371*(Cr-128);
     float G = Y - 0.698*(Cr-128)+0.336*(Cb-128);
     float B = Y + 1.732*(Cb-128);

     return color(R, G, B);
}
  
// ************************************************************
//                      RGB TO YUV
// ************************************************************

color RGB2YUV(int R, int G, int B) {
    // from OV7670 Software Application note
    float Yu = 0.299*R+0.587*G+0.114*B;
    float Cb = 0.568*(B-Y)+128;
    float Cr = 0.713*(R-Y)+128;

    return color(Yu, Cb, Cr);
}
// ************************************************************
//
// ************************************************************
void keyPressed() {

  switch (key) {
    case ' ':  int i = request.ordinal()+1;
               if (i >= request_t.values().length) i=0;
               request = request_t.values()[i];
               serialPort.clear();
               reqStatus = requestStatus_t.IDLE;
               
           break; 
   case 'k':  bKalmanEnabled = !bKalmanEnabled;
           break; 
   case 's':  saveFrame(); break; 
   default: break;
  }
  
}
// ************************************************************
//
// ************************************************************

void mousePressed() {
}
