#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "itoa.h"
#include "stm32f4_delay.h"
#include "stm32f4_HD44780.h"

extern const u8 rawAudio[123200];
float v;
unsigned int ADC_Result1 = 0;
int program;
int a = 0;
int b = 0;
char buf[10];
int wskaz = 0;

typedef enum {
	false, true
} bool;

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

void init(void) {
	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
			| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
			| GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//char* getA(int a) {
//	char buffer[2];
//	if(a <= 9) {
//		sprintf(buffer, "0%d", 1);
//	} else {
//		sprintf(buffer, "%d", 1);
//	}
//	return *buffer;
//}

void setupButtons(void) {
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == RESET) {
		HD44780_CursorOff();
		HD44780_BlinkOff();
		Delayms(500);
		HD44780_Clear();
		program++;
		if (program == 1) {
			HD44780_Puts(0, 0, "00:00");
			HD44780_Puts(0, 1, "GODZINA");
		}
		if (program == 2) {
//					char* buf;
//										sprintf(buf,"%d", 12);
//			HD44780_Puts(0, 0, getA());
			HD44780_Puts(2, 0, ":");
//			HD44780_Puts(3, 0, getB());

			HD44780_Puts(0, 1, "BUDZIK");
		}
		if (program == 3) {
			HD44780_Puts(0, 0, "00:00");
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
		HD44780_CursorOn();
		HD44780_BlinkOn();
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
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == RESET) {
		Delayms(200);
		if (wskaz == 1) {
			a++;
			if (a == 24) {
				a = 0;
			}
			itoa(a, buf, 10);
			if (a < 10) {
				HD44780_Puts(0, 0, "0");
				HD44780_Puts(1, 0, buf);
			} else {
				HD44780_Puts(0, 0, buf);
			}
			HD44780_CursorSet(1, 0);
		} else if (wskaz == 2) {
			b++;
			if (b == 60) {
				b = 0;
			}
			itoa(b, buf, 10);
			if (b < 10) {
				HD44780_Puts(3, 0, "0");
				HD44780_Puts(4, 0, buf);
			} else {
				HD44780_Puts(3, 0, buf);
			}
			HD44780_CursorSet(4, 0);
		}

//				HD44780_Clear();
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == RESET) {
		HD44780_Clear();
	}
}

int main(void) {
	init();

	//Inicjalizacja wyswietlacza, podajemy wartosc wierszy i kolumn
	HD44780_Init(16, 2);

	//Wypisanie stringu na wyswietlaczu
	HD44780_Puts(0, 0, "STM32F4 Discover");
	HD44780_Puts(0, 1, "ZEGAREK");

	program = 0;

	for (;;) {
		setupButtons();
	}
}
