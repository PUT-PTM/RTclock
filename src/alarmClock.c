#include "alarmClock.h"

int hourAlarmClock = 0;
int minuteAlarmClock = 0;

bool alarmClockSet = false;

void alarmClockFirstButton() {
	itoa(hourAlarmClock, buf, 10);
	if (hourAlarmClock > 9) {
		HD44780_Puts(0, 0, buf);
	} else {
		HD44780_Puts(0, 0, "0");
		HD44780_Puts(1, 0, buf);
	}
	HD44780_Puts(2, 0, ":");
	itoa(minuteAlarmClock, buf, 10);
	if (minuteAlarmClock > 9) {
		HD44780_Puts(3, 0, buf);
	} else {
		HD44780_Puts(3, 0, "0");
		HD44780_Puts(4, 0, buf);
	}
	HD44780_Puts(0, 1, "BUDZIK");
}


int getAlarmClockHour() {
	return hourAlarmClock;
}

bool isAlarmClockSet() {
	return alarmClockSet;
}

int getAlarmClockMinute() {
	return minuteAlarmClock;
}

void increaseAlarmHour() {
	hourAlarmClock = hourAlarmClock + 1;
		if (hourAlarmClock == 24) {
			hourAlarmClock = 0;
		}

		itoa(hourAlarmClock, buf, 10);
		if (hourAlarmClock < 10) {
			HD44780_Puts(0, 0, "0");
			HD44780_Puts(1, 0, buf);
		} else {
			HD44780_Puts(0, 0, buf);
		}
		HD44780_CursorSet(1, 0);
}

void increaseAlarmMinute() {
	minuteAlarmClock = minuteAlarmClock + 1;
		if (minuteAlarmClock == 60) {
			minuteAlarmClock = 0;
		}
		itoa(minuteAlarmClock, buf, 10);
		if (minuteAlarmClock < 10) {
			HD44780_Puts(3, 0, "0");
			HD44780_Puts(4, 0, buf);
		} else {
			HD44780_Puts(3, 0, buf);
		}
		HD44780_CursorSet(4, 0);
}

void setAlarmClock(bool set) {
	alarmClockSet = set;
}
