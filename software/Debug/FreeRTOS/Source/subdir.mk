################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/croutine.c \
../FreeRTOS/Source/event_groups.c \
../FreeRTOS/Source/list.c \
../FreeRTOS/Source/queue.c \
../FreeRTOS/Source/stream_buffer.c \
../FreeRTOS/Source/tasks.c \
../FreeRTOS/Source/timers.c 

OBJS += \
./FreeRTOS/Source/croutine.o \
./FreeRTOS/Source/event_groups.o \
./FreeRTOS/Source/list.o \
./FreeRTOS/Source/queue.o \
./FreeRTOS/Source/stream_buffer.o \
./FreeRTOS/Source/tasks.o \
./FreeRTOS/Source/timers.o 

C_DEPS += \
./FreeRTOS/Source/croutine.d \
./FreeRTOS/Source/event_groups.d \
./FreeRTOS/Source/list.d \
./FreeRTOS/Source/queue.d \
./FreeRTOS/Source/stream_buffer.d \
./FreeRTOS/Source/tasks.d \
./FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/%.o FreeRTOS/Source/%.su FreeRTOS/Source/%.cyclo: ../FreeRTOS/Source/%.c FreeRTOS/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I"C:/Users/Viktor/Desktop/labaratorni/Projects/TS_Tracker/software/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Viktor/Desktop/labaratorni/Projects/TS_Tracker/software/FreeRTOS/Source/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeRTOS-2f-Source

clean-FreeRTOS-2f-Source:
	-$(RM) ./FreeRTOS/Source/croutine.cyclo ./FreeRTOS/Source/croutine.d ./FreeRTOS/Source/croutine.o ./FreeRTOS/Source/croutine.su ./FreeRTOS/Source/event_groups.cyclo ./FreeRTOS/Source/event_groups.d ./FreeRTOS/Source/event_groups.o ./FreeRTOS/Source/event_groups.su ./FreeRTOS/Source/list.cyclo ./FreeRTOS/Source/list.d ./FreeRTOS/Source/list.o ./FreeRTOS/Source/list.su ./FreeRTOS/Source/queue.cyclo ./FreeRTOS/Source/queue.d ./FreeRTOS/Source/queue.o ./FreeRTOS/Source/queue.su ./FreeRTOS/Source/stream_buffer.cyclo ./FreeRTOS/Source/stream_buffer.d ./FreeRTOS/Source/stream_buffer.o ./FreeRTOS/Source/stream_buffer.su ./FreeRTOS/Source/tasks.cyclo ./FreeRTOS/Source/tasks.d ./FreeRTOS/Source/tasks.o ./FreeRTOS/Source/tasks.su ./FreeRTOS/Source/timers.cyclo ./FreeRTOS/Source/timers.d ./FreeRTOS/Source/timers.o ./FreeRTOS/Source/timers.su

.PHONY: clean-FreeRTOS-2f-Source

