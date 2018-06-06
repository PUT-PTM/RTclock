#include "stm32f4xx.h"
#include "rtc.h"

//void I2C_WriteBuffer(I2C_HandleTypeDef handler, uint8_t address, uint8_t *buf, uint8_t buffer_sizee)
//{
//    while(HAL_I2C_Master_Transmit(&handler, (uint16_t) address, (uint8_t*)buf, (uint16_t) buffer_size, (uint32_t)500)!=HAL_OK)
//    {
//            if(HAL_I2C_GetError(&hi)!=HAL_I2C_ERROR_AF) {
//                return I2C_Buf_Error;
//            }
//    }
//    return I2C_Buf_OK;
//}
//uint8_t I2C_ReadBuffer(I2C_HandleTypeDef handler, uint8_t address, uint8_t *buffer, uint8_t buffer_size)
//{
//    while(HAL_I2C_Master_Receive(&handler, (uint16_t) address, (uint8_t*)buffer, (uint16_t)buffer_size, (uint32_t)500)!=HAL_OK)
//    {
//            if(HAL_I2C_GetError(&hi)!=HAL_I2C_ERROR_AF){
//                return I2C_Buf_Error;
//            }
//    }
//    return I2C_Buf_OK;
//}
