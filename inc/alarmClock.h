#include "base.h"

#ifndef ALARMCLOCK_H_
#define ALARMCLOCK_H_

void alarmClockFirstButton();
int getAlarmClockHour();
int getAlarmClockMinute();
bool isAlarmClockSet();
void increaseAlarmHour();
void increaseAlarmMinute();
void setAlarmClock(bool set);

#endif /* ALARMCLOCK_H_ */
