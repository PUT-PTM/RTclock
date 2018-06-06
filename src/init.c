#include "stm32f4xx.h"
#include "init.h"

void init_LCD() {
	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
			| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void init_buttons() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
			| GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//void init_RTC(RTC_TimeTypeDef RTC_TimeStruct) {
//	RTC_InitTypeDef RTC_InitStructure;
////	RTC_TimeTypeDef RTC_TimeStruct;
//
//	/* Enable the PWR clock */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
//
//	/* Allow access to RTC */
//	PWR_BackupAccessCmd(ENABLE);
//
//	/* Reset RTC Domain */
//	RCC_BackupResetCmd(ENABLE);
//	RCC_BackupResetCmd(DISABLE);
//
//	/* Enable the LSE OSC */
//	RCC_LSEConfig(RCC_LSE_ON);
//
//	/* Wait until LSE is ready */
//	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
//		;
//
//	/* Select the RTC Clock Source */
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
//
//	/* Configure the RTC data register and RTC prescaler */
//	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
//	RTC_InitStructure.RTC_SynchPrediv = 0xFF;
//	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
//	RTC_Init(&RTC_InitStructure);
//
//	/* Set the time to 00h 00mn 00s AM */
//	RTC_TimeStruct.RTC_H12 = RTC_H12_AM;
//	RTC_TimeStruct.RTC_Hours = 0x00;
//	RTC_TimeStruct.RTC_Minutes = 0x00;
//	RTC_TimeStruct.RTC_Seconds = 0x00;
//	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
//}

void init_DAC(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // zegar dla portu GPIO z którego wykorzystany zostanie pin jako wyjœcie DAC (PA4)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_InitTypeDef DAC_InitStructure;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Cmd(DAC_Channel_1, ENABLE);

	DAC_SetChannel1Data(DAC_Align_12b_R, 1000); //- ewentualnie do muzyczki

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	DAC_Cmd(DAC_Channel_2, ENABLE);
}

void init_Timer3(int period, int prescaler) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef str;
	str.TIM_Period = period;
	str.TIM_Prescaler = prescaler;
	str.TIM_ClockDivision = TIM_CKD_DIV1;
	str.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &str);
	TIM_Cmd(TIM3, ENABLE);
}

void init_Timer3_Interruption() {
	NVIC_InitTypeDef str;
	str.NVIC_IRQChannel = TIM3_IRQn;
	str.NVIC_IRQChannelPreemptionPriority = 0x00;
	str.NVIC_IRQChannelSubPriority = 0x00;
	str.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&str);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}
