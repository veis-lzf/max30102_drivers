################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/bsp_mkb0908.c 

OBJS += \
./bsp/bsp_mkb0908.o 

C_DEPS += \
./bsp/bsp_mkb0908.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/%.o bsp/%.su bsp/%.cyclo: ../bsp/%.c bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/project/MKB0908/MKB0908_Demo/pfv_drivers" -I"D:/project/MKB0908/MKB0908_Demo/bsp" -I"D:/project/MKB0908/MKB0908_Demo/usrApp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-bsp

clean-bsp:
	-$(RM) ./bsp/bsp_mkb0908.cyclo ./bsp/bsp_mkb0908.d ./bsp/bsp_mkb0908.o ./bsp/bsp_mkb0908.su

.PHONY: clean-bsp

