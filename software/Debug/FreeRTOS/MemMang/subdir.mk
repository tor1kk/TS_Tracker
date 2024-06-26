################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/MemMang/heap_4.c 

OBJS += \
./FreeRTOS/MemMang/heap_4.o 

C_DEPS += \
./FreeRTOS/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/MemMang/%.o FreeRTOS/MemMang/%.su FreeRTOS/MemMang/%.cyclo: ../FreeRTOS/MemMang/%.c FreeRTOS/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I"C:/Users/Viktor/Desktop/labaratorni/Projects/TS_Tracker/software/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Viktor/Desktop/labaratorni/Projects/TS_Tracker/software/FreeRTOS/Source/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeRTOS-2f-MemMang

clean-FreeRTOS-2f-MemMang:
	-$(RM) ./FreeRTOS/MemMang/heap_4.cyclo ./FreeRTOS/MemMang/heap_4.d ./FreeRTOS/MemMang/heap_4.o ./FreeRTOS/MemMang/heap_4.su

.PHONY: clean-FreeRTOS-2f-MemMang

