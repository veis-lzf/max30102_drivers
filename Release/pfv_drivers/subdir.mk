################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../pfv_drivers/pfv_i2c.c \
../pfv_drivers/pfv_mkb0908.c \
../pfv_drivers/pfv_usart.c 

OBJS += \
./pfv_drivers/pfv_i2c.o \
./pfv_drivers/pfv_mkb0908.o \
./pfv_drivers/pfv_usart.o 

C_DEPS += \
./pfv_drivers/pfv_i2c.d \
./pfv_drivers/pfv_mkb0908.d \
./pfv_drivers/pfv_usart.d 


# Each subdirectory must supply rules for building sources it contributes
pfv_drivers/%.o pfv_drivers/%.su pfv_drivers/%.cyclo: ../pfv_drivers/%.c pfv_drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/project/MKB0908/MKB0908_Demo/pfv_drivers" -I"D:/project/MKB0908/MKB0908_Demo/usrApp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-pfv_drivers

clean-pfv_drivers:
	-$(RM) ./pfv_drivers/pfv_i2c.cyclo ./pfv_drivers/pfv_i2c.d ./pfv_drivers/pfv_i2c.o ./pfv_drivers/pfv_i2c.su ./pfv_drivers/pfv_mkb0908.cyclo ./pfv_drivers/pfv_mkb0908.d ./pfv_drivers/pfv_mkb0908.o ./pfv_drivers/pfv_mkb0908.su ./pfv_drivers/pfv_usart.cyclo ./pfv_drivers/pfv_usart.d ./pfv_drivers/pfv_usart.o ./pfv_drivers/pfv_usart.su

.PHONY: clean-pfv_drivers

