#include "stopWatch.h"

int minuteStopWatch = 0;
int secondStopWatch = 0;
int previousSecondStopWatch = -1;

bool stopWatchRunning;

void stopWatchFirstButton() {
	itoa(minuteStopWatch, buf, 10);
	if (minuteStopWatch > 9) {
		HD44780_Puts(0, 0, buf);
	} else {
		HD44780_Puts(0, 0, "0");
		HD44780_Puts(1, 0, buf);
	}
	HD44780_Puts(2, 0, ":");
	itoa(secondStopWatch, buf, 10);
	if (secondStopWatch > 9) {
		HD44780_Puts(3, 0, buf);
	} else {
		HD44780_Puts(3, 0, "0");
		HD44780_Puts(4, 0, buf);
	}
	HD44780_Puts(0, 1, "STOPER");
}

void stopWatchFourthButton() {
	minuteStopWatch = 0;
	secondStopWatch = 0;
	previousSecondStopWatch = -1;
	stopWatchRunning = false;
	HD44780_Puts(0, 0, "00:00 ");
}

void watchStopWatch(uint8_t dataRead[], int program) {
	if (stopWatchRunning) {
		if (previousSecondStopWatch == -1
				|| (previousSecondStopWatch > 0 && dataRead[0] == 0)) {
			previousSecondStopWatch = dataRead[0];
		} else if (previousSecondStopWatch < dataRead[0]) {
			previousSecondStopWatch = dataRead[0];
			secondStopWatch++;
			if (secondStopWatch == 60) {
				if (minuteStopWatch != 99) {
					minuteStopWatch++;
				}
				secondStopWatch = 0;
			}
			itoa(minuteStopWatch, buf, 10);
			if (program == 4) {
				if (minuteStopWatch > 9) {
					HD44780_Puts(0, 0, buf);
				} else {
					HD44780_Puts(0, 0, "0");
					HD44780_Puts(1, 0, buf);
				}
				HD44780_Puts(2, 0, ":");
				itoa(secondStopWatch, buf, 10);
				if (secondStopWatch > 9) {
					HD44780_Puts(3, 0, buf);
				} else {
					HD44780_Puts(3, 0, "0");
					HD44780_Puts(4, 0, buf);
				}
			}
		}
	}
}

void changeStopWatchRunning() {
	stopWatchRunning = !stopWatchRunning;
}
