################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/animation.c \
../Core/Src/App/animation_data.c \
../Core/Src/App/app.c \
../Core/Src/App/button.c \
../Core/Src/App/interrupts.c \
../Core/Src/App/led.c \
../Core/Src/App/usb.c \
../Core/Src/App/w25q.c 

OBJS += \
./Core/Src/App/animation.o \
./Core/Src/App/animation_data.o \
./Core/Src/App/app.o \
./Core/Src/App/button.o \
./Core/Src/App/interrupts.o \
./Core/Src/App/led.o \
./Core/Src/App/usb.o \
./Core/Src/App/w25q.o 

C_DEPS += \
./Core/Src/App/animation.d \
./Core/Src/App/animation_data.d \
./Core/Src/App/app.d \
./Core/Src/App/button.d \
./Core/Src/App/interrupts.d \
./Core/Src/App/led.d \
./Core/Src/App/usb.d \
./Core/Src/App/w25q.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/%.o Core/Src/App/%.su: ../Core/Src/App/%.c Core/Src/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-App

clean-Core-2f-Src-2f-App:
	-$(RM) ./Core/Src/App/animation.d ./Core/Src/App/animation.o ./Core/Src/App/animation.su ./Core/Src/App/animation_data.d ./Core/Src/App/animation_data.o ./Core/Src/App/animation_data.su ./Core/Src/App/app.d ./Core/Src/App/app.o ./Core/Src/App/app.su ./Core/Src/App/button.d ./Core/Src/App/button.o ./Core/Src/App/button.su ./Core/Src/App/interrupts.d ./Core/Src/App/interrupts.o ./Core/Src/App/interrupts.su ./Core/Src/App/led.d ./Core/Src/App/led.o ./Core/Src/App/led.su ./Core/Src/App/usb.d ./Core/Src/App/usb.o ./Core/Src/App/usb.su ./Core/Src/App/w25q.d ./Core/Src/App/w25q.o ./Core/Src/App/w25q.su

.PHONY: clean-Core-2f-Src-2f-App

