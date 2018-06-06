#ifndef RTC_H_
#define RTC_H_

typedef enum{
    I2C_Buf_Error = 0x00,
    I2C_Buf_OK
}Error_Stat;

//void I2C_WriteBuffer(I2C_HandleTypeDef handler, uint8_t address, uint8_t *buf, uint8_t buffer_sizee);

//uint8_t I2C_ReadBuffer(I2C_HandleTypeDef handler, uint8_t address, uint8_t *buffer, uint8_t buffer_size);

#endif
