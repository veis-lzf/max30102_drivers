################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usrApp/debug.c 

OBJS += \
./usrApp/debug.o 

C_DEPS += \
./usrApp/debug.d 


# Each subdirectory must supply rules for building sources it contributes
usrApp/%.o usrApp/%.su usrApp/%.cyclo: ../usrApp/%.c usrApp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/project/MKB0908/MKB0908_Demo/pfv_drivers" -I"D:/project/MKB0908/MKB0908_Demo/usrApp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usrApp

clean-usrApp:
	-$(RM) ./usrApp/debug.cyclo ./usrApp/debug.d ./usrApp/debug.o ./usrApp/debug.su

.PHONY: clean-usrApp

