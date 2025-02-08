################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/tasks/Software_config.c \
../Core/Src/tasks/can.c \
../Core/Src/tasks/display.c \
../Core/Src/tasks/errors.c 

OBJS += \
./Core/Src/tasks/Software_config.o \
./Core/Src/tasks/can.o \
./Core/Src/tasks/display.o \
./Core/Src/tasks/errors.o 

C_DEPS += \
./Core/Src/tasks/Software_config.d \
./Core/Src/tasks/can.d \
./Core/Src/tasks/display.d \
./Core/Src/tasks/errors.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/tasks/%.o Core/Src/tasks/%.su Core/Src/tasks/%.cyclo: ../Core/Src/tasks/%.c Core/Src/tasks/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F303xE -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-tasks

clean-Core-2f-Src-2f-tasks:
	-$(RM) ./Core/Src/tasks/Software_config.cyclo ./Core/Src/tasks/Software_config.d ./Core/Src/tasks/Software_config.o ./Core/Src/tasks/Software_config.su ./Core/Src/tasks/can.cyclo ./Core/Src/tasks/can.d ./Core/Src/tasks/can.o ./Core/Src/tasks/can.su ./Core/Src/tasks/display.cyclo ./Core/Src/tasks/display.d ./Core/Src/tasks/display.o ./Core/Src/tasks/display.su ./Core/Src/tasks/errors.cyclo ./Core/Src/tasks/errors.d ./Core/Src/tasks/errors.o ./Core/Src/tasks/errors.su

.PHONY: clean-Core-2f-Src-2f-tasks

