#include "clock.h"

int editingStep = 0;
int hour = 0;
int minute = 0;

void clockFirstButton() {
	HD44780_Puts(0, 0, "00:00:");
	itoa(hour, buf, 10);
	if (hour < 10) {
		HD44780_Puts(0, 0, "0");
		HD44780_Puts(1, 0, buf);
	} else {
		HD44780_Puts(0, 0, buf);
	}
	itoa(minute, buf, 10);
	if (minute < 10) {
		HD44780_Puts(3, 0, "0");
		HD44780_Puts(4, 0, buf);
	} else {
		HD44780_Puts(3, 0, buf);
	}

	HD44780_Puts(0, 1, "GODZINA");
}

int getEditingStep() {
	return editingStep;
}

void setEditingStep(int step) {
	editingStep = step;
}

void setClockHour(uint8_t h) {
	hour = h;
}

void setClockMinute(uint8_t m) {
	minute = m;
}

int getClockHour() {
	return hour;
}

int getClockMinute() {
	return minute;
}

void increaseClockHour() {
	hour = hour + 1;
	if (hour >= 24) {
		hour = 0;
	}

	itoa(hour, buf, 10);
	if (hour < 10) {
		HD44780_Puts(0, 0, "0");
		HD44780_Puts(1, 0, buf);
	} else {
		HD44780_Puts(0, 0, buf);
	}
	HD44780_CursorSet(1, 0);
}

void increaseClockMinute() {
	minute = minute + 1;
	if (minute >= 60) {
		minute = 0;
	}
	itoa(minute, buf, 10);
	if (minute < 10) {
		HD44780_Puts(3, 0, "0");
		HD44780_Puts(4, 0, buf);
	} else {
		HD44780_Puts(3, 0, buf);
	}
	HD44780_CursorSet(4, 0);
}

void editHour(int h, int m, int s) {
	if (editingStep == 0) {
		printValue(0, h);
		printValue(3, m);
		printValue(6, s);
	} else if (editingStep == 1) {
		printValue(0, h);
		printValue(3, m);
		HD44780_Puts(5, 0, "   ");
		editingStep = 2;
		hour = h;
		minute = m;
	} else if (editingStep == 3) {
		writeData(hour, minute, 0);
		editingStep = 0;
		HD44780_Puts(5, 0, ":");
		HD44780_CursorOff();
		HD44780_BlinkOff();
		setWskaz(0);
	}
}
