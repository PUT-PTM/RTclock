#include "alarmClock.h"
#include "clock.h"
#include "stopWatch.h"
#include "init.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_delay.h"
#ifndef MAIN_H_
#define MAIN_H_

#define SENSOR_ADDRESS 0xD0 // or set this according to your HW configuration

#define HEX_2_DEC(val) (((val)/16)*10+((val)%16))
#define DEC_2_HEX(val) (((val)/10)*16+((val)%10))

extern const u8 rawAudio[123200];
float v;
unsigned int ADC_Result1 = 0;

uint8_t dataRead[] = { 0, 0, 0 };
int wskaz = 0;
int program;

void setWskaz(int w);

#endif /* MAIN_H_ */
