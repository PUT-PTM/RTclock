#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "itoa.h"
#include "stm32f4_delay.h"
#include "stm32f4_HD44780.h"
#include "tm_stm32f4_i2c.h"
#include "init.h"

#define SENSOR_ADDRESS 0xD0 // or set this according to your HW configuration

#define HEX_2_DEC(val) (((val)/16)*10+((val)%16))
#define DEC_2_HEX(val) (((val)/10)*16+((val)%10))
#define ADDRESS        0xD0 // 1101 000 0

extern const u8 rawAudio[123200];
float v;
unsigned int ADC_Result1 = 0;
int program;
int editingStep = 0;
int hour = 0;
int minute = 0;
int minuteStopWatch = 0;
int secondStopWatch = 0;
int previousSecondStopWatch = -1;
char buf[10];
int wskaz = 0;

typedef enum {
	false, true
} bool;

bool stopWatchRunning = false;
bool w_gore = true;
unsigned int temp = 0;
void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {

		temp++;
		if (temp == 123200) {
			temp = 0;
		}
		DAC_SetChannel1Data(DAC_Align_12b_R, rawAudio[temp]);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void setupButtons(void) {
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == RESET) {
		HD44780_CursorOff();
		HD44780_BlinkOff();
		Delayms(500);
		HD44780_Clear();
		program++;
		if (program == 1) {
			HD44780_Puts(0, 0, "00:00:");
			HD44780_Puts(0, 1, "GODZINA");
		}
		if (program == 2) {
			HD44780_Puts(2, 0, ":");
			HD44780_Puts(0, 1, "BUDZIK");
		}
		if (program == 3) {
			itoa(minuteStopWatch, buf, 10);
			if (minuteStopWatch > 9) {
				HD44780_Puts(0, 0, buf);
			} else {
				HD44780_Puts(0, 0, "0");
				HD44780_Puts(1, 0, buf);
			}
			HD44780_Puts(2, 0, ":");
			itoa(secondStopWatch, buf, 10);
			if (secondStopWatch < 10) {
				HD44780_Puts(3, 0, buf);
			} else {
				HD44780_Puts(3, 0, "0");
				HD44780_Puts(4, 0, buf);
			}
			HD44780_Puts(0, 1, "STOPER");
		}
		if (program == 4) {
			HD44780_Puts(0, 0, "00:00");
			HD44780_Puts(0, 1, "MINUTNIK");
			program = 0;
		}
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == RESET) {
		Delayms(200);
		if (program != 3) {

			HD44780_CursorOn();
			HD44780_BlinkOn();
			if (editingStep == 0) {
				editingStep = 1;
			}
			if (wskaz == 0) {
				wskaz++;
				HD44780_CursorSet(1, 0);
			} else if (wskaz == 1) {
				wskaz++;
				HD44780_CursorSet(4, 0);
			} else {
				HD44780_CursorOff();
				HD44780_BlinkOff();
				wskaz = 0;
			}
		} else {
			stopWatchRunning = !stopWatchRunning;
		}
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == RESET) {
		Delayms(200);
		editingStep = 2;
		if (wskaz == 1) {
			hour++;
			if (hour == 24) {
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
		} else if (wskaz == 2) {
			minute++;
			if (minute == 60) {
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
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == RESET) {
		editingStep = 3;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
		if (program == 3) {
			minuteStopWatch = 0;
			secondStopWatch = 0;
			previousSecondStopWatch = -1;
			stopWatchRunning = false;
			HD44780_Puts(0, 0, "00:00 ");
		}
	}
}

void printValue(int position, int value) {
	if (value < 10) {
		itoa(value, buf, 10);
		HD44780_Puts(position, 0, "0");
		HD44780_Puts(position + 1, 0, buf);
	} else {
		int val = 0;
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
		itoa(val, buf, 10);
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

int main(void) {
	init_buttons();
	init_LCD();
	TM_I2C_Init(I2C1, TM_I2C_PinsPack_2, 50000);
	HD44780_Init(16, 2);
	//	writeData(23, 55, 00);

	HD44780_Puts(0, 0, "STM32F4 Discover");
	HD44780_Puts(0, 1, "ZEGAREK");

	program = 0;

	for (;;) {
		//testujemy
		uint8_t dataRead[] = { 0, 0, 0 };
		TM_I2C_ReadMulti(I2C1, 0xD0, 0x00, dataRead, 3);
		if (program == 1 && editingStep == 0) {
			printValue(0, dataRead[2]);
			printValue(3, dataRead[1]);
			printValue(6, dataRead[0]);
		} else if (editingStep == 1) {
			printValue(0, dataRead[2]);
			printValue(3, dataRead[1]);
			HD44780_Puts(5, 0, "   ");
			editingStep = 2;
			hour = dataRead[2];
			minute = dataRead[1];
		} else if (editingStep == 3) {
			writeData(hour, minute, 0);
			editingStep = 0;
			HD44780_Puts(5, 0, ":");
			HD44780_CursorOff();
			HD44780_BlinkOff();
			wskaz = 0;
		}
//		if (hour == 3) {
//			init_DAC();
//			init_Timer3(525 - 1, 10 - 1);
//			init_Timer3_Interruption();
//		}
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
//				itoa(minuteStopWatch, buf, 10);
//				HD44780_Puts(0, 0, buf);
//				itoa(secondStopWatch, buf, 10);
//				HD44780_Puts(3, 0, buf);
			}
		}
		setupButtons();
	}
}
