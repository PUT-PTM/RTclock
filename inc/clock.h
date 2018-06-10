#include "base.h"

#ifndef CLOCK_H_
#define CLOCK_H_

void clockFirstButton();
int getEditingStep();
void setEditingStep(int step);
void setClockHour(uint8_t h);
void setClockMinute(uint8_t m);
int getClockHour();
int getClockMinute();
void increaseClockHour();
void increaseClockMinute();
void editHour();

#endif /* CLOCK_H_ */
