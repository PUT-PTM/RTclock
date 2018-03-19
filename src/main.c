#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int main(void) {
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

	unsigned int i;

	for (;;) {
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == RESET) {

			GPIO_SetBits(GPIOD, GPIO_Pin_12);
		} else {
			GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		}

		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == RESET) {

			GPIO_SetBits(GPIOD, GPIO_Pin_13);
		} else {
			GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == RESET) {

			GPIO_SetBits(GPIOD, GPIO_Pin_14);
		} else {
			GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == RESET) {

			GPIO_SetBits(GPIOD, GPIO_Pin_15);
		} else {
			GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		}
//		GPIO_SetBits(GPIOD,
//				GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
//		for (i = 0; i < 1000000; i++)
//			;
//		GPIO_ResetBits(GPIOD,
//				GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
//		for (i = 0; i < 1000000; i++)
//			;
	}
}
