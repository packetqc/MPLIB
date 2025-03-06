################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../MPSystem.cpp 

OBJS += \
./MPSystem.o 

CPP_DEPS += \
./MPSystem.d 


# Each subdirectory must supply rules for building sources it contributes
%.o %.su %.cyclo: ../%.cpp subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m33 -std=gnu++14 -g3 -DSTM32H573xx -c -I../. -I"D:/MPLIB/MPLIB_STM32_MCU/Drivers/BSP/STM32H573I-DK" -I"D:/MPLIB/MPLIB_STM32_MCU/Drivers/STM32H5xx_HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean--2e-

clean--2e-:
	-$(RM) ./MPSystem.cyclo ./MPSystem.d ./MPSystem.o ./MPSystem.su

.PHONY: clean--2e-

