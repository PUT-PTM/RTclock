#include "stm32f4_HD44780.h"
#include "itoa.h"
#include "tm_stm32f4_i2c.h"
#include <stdio.h>

#ifndef BASE_H_
#define BASE_H_

#define ADDRESS        0xD0 // 1101 000 0

char buf[10];

typedef enum {
	false, true
} bool;

int getValue(int value);
void printValue(int position, int value);
int writeValue(int value);
void writeData(int hour, int min, int sec);

#endif /* BASE_H_ */
