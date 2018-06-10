#include "main.h"

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

void setWskaz(int w) {
	wskaz = w;
}

void setupButtons(void) {
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == RESET) {
		setEditingStep(0);
		HD44780_CursorOff();
		HD44780_BlinkOff();
		wskaz = 0;
		Delayms(500);
		HD44780_Clear();
		if (program == 4) {
			program = 0;
		}
		program++;
		if (program == 1) {
			clockFirstButton();
		}
		if (program == 2) {
			alarmClockFirstButton();
		}
		if (program == 3) {
			stopWatchFirstButton();
			program = 4;
		}
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == RESET) {
		Delayms(200);
		if (program != 4) {
			if (program == 2) {
				setAlarmClock(false);
			}
			HD44780_CursorOn();
			HD44780_BlinkOn();
			if (getEditingStep() == 0) {
				setEditingStep(1);
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
			changeStopWatchRunning();
		}
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == RESET) {
		Delayms(200);
		if (program == 1) {
			setEditingStep(2);
		}
		if (wskaz == 1) {
			if (program == 2) {
				increaseAlarmHour();
			}
			if (program == 1) {
				increaseClockHour();
			}

		} else if (wskaz == 2) {
			if (program == 2) {
				increaseAlarmMinute();
			}
			if (program == 1) {
				increaseClockMinute();
			}
		}
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == RESET) {
		Delayms(200);
		if (getEditingStep() != 0) {
			setEditingStep(3);
		}
		if (program == 4) {
			stopWatchFourthButton();
		}
		if (program == 2 && !isAlarmClockSet()) {
			setEditingStep(0);
			setAlarmClock(true);
			wskaz = 0;
			HD44780_CursorOff();
			HD44780_BlinkOff();
		} else {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
			setAlarmClock(false);
		}
	}
}

void init() {
	init_buttons();
	init_LCD();
	TM_I2C_Init(I2C1, TM_I2C_PinsPack_2, 50000);
	HD44780_Init(16, 2);
	HD44780_Puts(0, 0, "STM32F4 Discover");
	HD44780_Puts(0, 1, "ZEGAREK");
	program = 0;
}

int main(void) {
	init();
	for (;;) {
		TM_I2C_ReadMulti(I2C1, 0xD0, 0x00, dataRead, 3);
		if (isAlarmClockSet() && getValue(dataRead[2]) == getAlarmClockHour()
				&& getValue(dataRead[1]) == getAlarmClockMinute()) {
			init_DAC();
			init_Timer3(525 - 1, 10 - 1);
			init_Timer3_Interruption();
		}
		if (getEditingStep() == 0) {
			setClockHour(getValue(dataRead[2]));
			setClockMinute(getValue(dataRead[1]));
		}
		if (program == 1) {
			editHour(dataRead[2], dataRead[1], dataRead[0]);
		}
		watchStopWatch(dataRead, program);
		setupButtons();
	}
}
