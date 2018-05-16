################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/audio.c \
../src/itoa.c \
../src/main.c \
../src/stm32f4_HD44780.c \
../src/stm32f4_delay.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/audio.o \
./src/itoa.o \
./src/main.o \
./src/stm32f4_HD44780.o \
./src/stm32f4_delay.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/audio.d \
./src/itoa.d \
./src/main.d \
./src/stm32f4_HD44780.d \
./src/stm32f4_delay.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F4DISCOVERY -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"D:/Workspace/PTM/RTclock/Utilities" -I"D:/Workspace/PTM/RTclock/StdPeriph_Driver/inc" -I"D:/Workspace/PTM/RTclock/inc" -I"D:/Workspace/PTM/RTclock/CMSIS/device" -I"D:/Workspace/PTM/RTclock/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


