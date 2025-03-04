################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DataServices.cpp \
../MPScrollList.cpp \
../MQTTServices.cpp \
../NetServices.cpp \
../SDServices.cpp 

OBJS += \
./DataServices.o \
./MPScrollList.o \
./MQTTServices.o \
./NetServices.o \
./SDServices.o 

CPP_DEPS += \
./DataServices.d \
./MPScrollList.d \
./MQTTServices.d \
./NetServices.d \
./SDServices.d 


# Each subdirectory must supply rules for building sources it contributes
%.o %.su %.cyclo: ../%.cpp subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m33 -std=gnu++14 -g3 -c -I../. -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean--2e-

clean--2e-:
	-$(RM) ./DataServices.cyclo ./DataServices.d ./DataServices.o ./DataServices.su ./MPScrollList.cyclo ./MPScrollList.d ./MPScrollList.o ./MPScrollList.su ./MQTTServices.cyclo ./MQTTServices.d ./MQTTServices.o ./MQTTServices.su ./NetServices.cyclo ./NetServices.d ./NetServices.o ./NetServices.su ./SDServices.cyclo ./SDServices.d ./SDServices.o ./SDServices.su

.PHONY: clean--2e-

