################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/stm32F429_gpio_driver.c \
../Drivers/Src/stm32F429_usb_driver.c \
../Drivers/Src/stm32F429_usb_framework.c 

OBJS += \
./Drivers/Src/stm32F429_gpio_driver.o \
./Drivers/Src/stm32F429_usb_driver.o \
./Drivers/Src/stm32F429_usb_framework.o 

C_DEPS += \
./Drivers/Src/stm32F429_gpio_driver.d \
./Drivers/Src/stm32F429_usb_driver.d \
./Drivers/Src/stm32F429_usb_framework.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/stm32F429_gpio_driver.o: ../Drivers/Src/stm32F429_gpio_driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F429I_DISC1 -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DDEBUG -DSTM32F429xx -c -I../Inc -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/CMSIS/Include" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/CMSIS/STM32F4xx/Include" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/Helpers" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Src/stm32F429_gpio_driver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Src/stm32F429_usb_driver.o: ../Drivers/Src/stm32F429_usb_driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F429I_DISC1 -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DDEBUG -DSTM32F429xx -c -I../Inc -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/CMSIS/Include" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/CMSIS/STM32F4xx/Include" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/Helpers" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Src/stm32F429_usb_driver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Src/stm32F429_usb_framework.o: ../Drivers/Src/stm32F429_usb_framework.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F429I_DISC1 -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DDEBUG -DSTM32F429xx -c -I../Inc -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/CMSIS/Include" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/CMSIS/STM32F4xx/Include" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/Helpers" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Src/stm32F429_usb_framework.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

