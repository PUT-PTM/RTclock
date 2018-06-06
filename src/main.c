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
int a = 0;
int b = 0;
char buf[10];
int wskaz = 0;
//RTC_TimeTypeDef* RTC_TimeStruct;
//RTC_DateTypeDef* RTC_DateStruct;

typedef enum {
	false, true
} bool;

//void tellTime() {
//	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
//	printf("hour = %d", RTC_TimeStruct->RTC_Hours);
//	printf("mins = %d", RTC_TimeStruct->RTC_Minutes);
//	printf("secs = %d", RTC_TimeStruct->RTC_Seconds);
//}
//
//void tellDate() {
//	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
//	printf("year %d", RTC_DateStruct->RTC_Year + 2000);
//	printf("month = 0%d", kRTC_DateStruct->RTC_Month);
//	printf("day = %d", RTC_DateStruct->RTC_Date);
//}

//typedef struct {
//	unsigned char second;
//	unsigned char minute;
//	unsigned char hour;
//	unsigned char day;
//	unsigned char month;
//	unsigned char year;
//} rtc_data_t;
//
//typedef struct {
//	unsigned char second;
//	unsigned char minute;
//	unsigned char hour;
//	unsigned char weekday;
//	unsigned char day;
//	unsigned char month;
//	unsigned char year;
//} raw_data_t;
//
//void rtc_get(rtc_data_t* rtc_data) {
//	raw_data_t raw_data;
////	i2c_polling_read(SENSOR_ADDRESS, 0x00, sizeof(raw_data), (char*) &raw_data);
//	rtc_data->second = HEX_2_DEC(raw_data.second);
//	rtc_data->minute = HEX_2_DEC(raw_data.minute);
//	rtc_data->hour = HEX_2_DEC(raw_data.hour);
//	rtc_data->day = HEX_2_DEC(raw_data.day);
//	rtc_data->month = HEX_2_DEC(raw_data.month);
//	rtc_data->year = HEX_2_DEC(raw_data.year);
//}
//
//void rtc_set(rtc_data_t* rtc_data) {
//	raw_data_t raw_data;
//	raw_data.second = DEC_2_HEX(rtc_data->second);
//	raw_data.minute = DEC_2_HEX(rtc_data->minute);
//	raw_data.hour = DEC_2_HEX(rtc_data->hour);
//	raw_data.day = DEC_2_HEX(rtc_data->day);
//	raw_data.month = DEC_2_HEX(rtc_data->month);
//	raw_data.year = DEC_2_HEX(rtc_data->year);
//	raw_data.weekday = RTC_Weekday_Monday; // or calculate the exact day
//	i2c_polling_write(SENSOR_ADDRESS, 0x00, sizeof(raw_data),
//			(char*) &raw_data);
//}

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
//			rtc_data_t* data;
//			rtc_get(data);
//			HD44780_Puts(0, 0, data->minute);
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
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
	}
}

int main(void) {
	uint8_t data[] = {0, 1, 2};
	init_buttons();
	init_LCD();
	TM_I2C_Init(I2C1, TM_I2C_PinsPack_2, 50000);
	TM_I2C_Write(I2C1, ADDRESS, 0x00, 5);
	 /**
	     * Write multi bytes to slave with address ADDRESS
	     * Write to registers starting from 0x00, get data in variable "data" and write 3 bytes
	     */
	    TM_I2C_WriteMulti(I2C1, ADDRESS, 0x00, data, 3);
	    itoa(data[1], buf, 10);

	    //Read single byte from slave with 0xD0 (1101 000 0) address and register location 0x00
	    data[0] = TM_I2C_Read(I2C1, ADDRESS, 0x00);

	    /**
	     * Read 3 bytes of data from slave with 0xD0 address
	     * First register to read from is at 0x00 location
	     * Store received data to "data" variable
	     */
	    TM_I2C_ReadMulti(I2C1, 0xD0, 0x00, data, 3);

//	init_RTC(RTC_TimeStruct);
//	tellTime();

	//Inicjalizacja wyswietlacza, podajemy wartosc wierszy i kolumn
	HD44780_Init(16, 2);

	//Wypisanie stringu na wyswietlaczu
	HD44780_Puts(0, 0, "STM32F4 Discover");
	HD44780_Puts(0, 1, "ZEGAREK");
	HD44780_Puts(0, 1, buf);

	program = 0;

	for (;;) {
		//testujemy
					uint8_t data[] = {0, 0, 0};
					TM_I2C_ReadMulti(I2C1, 0xD0, 0x00, data, 3);
					itoa(data[0], buf, 10);
					HD44780_Puts(6, 0, buf);
		//			itoa(data[1], buf, 10);
		//			HD44780_Puts(7, 0, buf);
		//			itoa(data[2], buf, 10);
		//			HD44780_Puts(8, 0, buf);
					//tutaj
		if (a == 3) {
			init_DAC();
			init_Timer3(525 - 1, 10 - 1);
			init_Timer3_Interruption();
		}
		setupButtons();
	}
}
