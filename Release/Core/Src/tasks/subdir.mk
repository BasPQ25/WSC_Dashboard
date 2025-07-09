################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/tasks/GPS.c \
../Core/Src/tasks/Software_config.c \
../Core/Src/tasks/buttons.c \
../Core/Src/tasks/buzzer.c \
../Core/Src/tasks/can_receive.c \
../Core/Src/tasks/can_transmit.c \
../Core/Src/tasks/display.c 

OBJS += \
./Core/Src/tasks/GPS.o \
./Core/Src/tasks/Software_config.o \
./Core/Src/tasks/buttons.o \
./Core/Src/tasks/buzzer.o \
./Core/Src/tasks/can_receive.o \
./Core/Src/tasks/can_transmit.o \
./Core/Src/tasks/display.o 

C_DEPS += \
./Core/Src/tasks/GPS.d \
./Core/Src/tasks/Software_config.d \
./Core/Src/tasks/buttons.d \
./Core/Src/tasks/buzzer.d \
./Core/Src/tasks/can_receive.d \
./Core/Src/tasks/can_transmit.d \
./Core/Src/tasks/display.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/tasks/%.o Core/Src/tasks/%.su Core/Src/tasks/%.cyclo: ../Core/Src/tasks/%.c Core/Src/tasks/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F303xE -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-tasks

clean-Core-2f-Src-2f-tasks:
	-$(RM) ./Core/Src/tasks/GPS.cyclo ./Core/Src/tasks/GPS.d ./Core/Src/tasks/GPS.o ./Core/Src/tasks/GPS.su ./Core/Src/tasks/Software_config.cyclo ./Core/Src/tasks/Software_config.d ./Core/Src/tasks/Software_config.o ./Core/Src/tasks/Software_config.su ./Core/Src/tasks/buttons.cyclo ./Core/Src/tasks/buttons.d ./Core/Src/tasks/buttons.o ./Core/Src/tasks/buttons.su ./Core/Src/tasks/buzzer.cyclo ./Core/Src/tasks/buzzer.d ./Core/Src/tasks/buzzer.o ./Core/Src/tasks/buzzer.su ./Core/Src/tasks/can_receive.cyclo ./Core/Src/tasks/can_receive.d ./Core/Src/tasks/can_receive.o ./Core/Src/tasks/can_receive.su ./Core/Src/tasks/can_transmit.cyclo ./Core/Src/tasks/can_transmit.d ./Core/Src/tasks/can_transmit.o ./Core/Src/tasks/can_transmit.su ./Core/Src/tasks/display.cyclo ./Core/Src/tasks/display.d ./Core/Src/tasks/display.o ./Core/Src/tasks/display.su

.PHONY: clean-Core-2f-Src-2f-tasks

