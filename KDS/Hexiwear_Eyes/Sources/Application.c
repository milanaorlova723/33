/*
 * Application.c
 *
 *  Created on: 01.04.2017
 *      Author: Erich Styger
 */

#include "Application.h"
#include "WAIT1.h"
#include "RGBR.h"
#include "RGBG.h"
#include "RGBB.h"
#include "LCD1.h"
#include "GDisp1.h"
#include "OLEDPower.h"
#include "uncannyEyes.h"
#include "Vcc3V3B_EN.h"
#include "TSL2561.h"

uint32_t APP_GetLux(void) {
  uint8_t res;
  uint16_t broadband, infrared;
  uint32_t lux;

  res = TSL2561_ReadRawDataFull(&broadband);
  if (res!=ERR_OK) {
    for(;;){}
  }
  res = TSL2561_ReadRawDataInfrared(&infrared);
  if (res!=ERR_OK) {
    for(;;){}
  }
  lux = TSL2561_CalculateLux(broadband, infrared);
  return lux;
}

void APP_Run(void) {
  //OLEDPower_ClrVal(); /* turn on power for OLED (low active) */
  EYES_Init();
  LCD1_Init();
  LCD1_Clear();
  GDisp1_DrawFilledBox(0, 0, GDisp1_GetWidth()-1, GDisp1_GetHeight()-1, LCD1_COLOR_RED);
  GDisp1_UpdateFull();
  {
    uint8_t res;
    uint8_t id;
    uint16_t broadband, infrared;
    uint32_t lux;

    /* 3V3B_EN:
     * HI-Z: Disabled
     * LOW: enabled (humidity, temperature, ambiLight
     */
    //Vcc3V3B_EN_SetInput(); /* disable */
    /* enable */
    Vcc3V3B_EN_SetOutput();
    Vcc3V3B_EN_ClrVal();
    WAIT1_Waitms(50);
    TSL2561_Init();

    res = TSL2561_Disable();
    if (res!=ERR_OK) {
      for(;;){}
    }
    WAIT1_Waitms(50);
    res = TSL2561_Enable();
    if (res!=ERR_OK) {
      for(;;){}
    }

    res = TSL2561_SetIntegrationTime(TSL2561_INTEGRATION_TIME_13MS);
    if (res!=ERR_OK) {
      for(;;){}
    }
    res = TSL2561_SetGain(TSL2561_GAIN_16X);
    if (res!=ERR_OK) {
      for(;;){}
    }
  }
  for(;;) {
    EYES_Run();
    RGBR_On();
    WAIT1_Waitms(5);
    RGBR_Off();
  }
}

