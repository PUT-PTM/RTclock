#include "base.h"

int getValue(int value) {
	int val = 0;
	if(value < 10) {
		val = value;
	}
	if (value > 9 && value < 32) {
		val = value - 6;
	} else if (value > 25 && value < 48) {
		val = value - 12;
	} else if (value > 41 && value < 64) {
		val = value - 18;
	} else if (value > 57 && value < 80) {
		val = value - 24;
	} else if (value > 73) {
		val = value - 30;
	}
	return val;
}

void printValue(int position, int value) {
	if (value < 10) {
		itoa(value, buf, 10);
		HD44780_Puts(position, 0, "0");
		HD44780_Puts(position + 1, 0, buf);
	} else {
		itoa(getValue(value), buf, 10);
		HD44780_Puts(position, 0, buf);
	}
}

int writeValue(int value) {
	if (value > 9 && value < 20) {
		value = value + 6;
	} else if (value > 19 && value < 30) {
		value = value + 12;
	} else if (value > 29 && value < 40) {
		value = value + 18;
	} else if (value > 39 && value < 50) {
		value = value + 24;
	} else if (value > 49) {
		value = value + 30;
	}
	return value;
}

void writeData(int hour, int min, int sec) {
	uint8_t dataWrite[] = { writeValue(sec), writeValue(min), writeValue(hour) };
	TM_I2C_WriteMulti(I2C1, ADDRESS, 0x00, dataWrite, 3);
}
