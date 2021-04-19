################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Helpers/logger.c 

OBJS += \
./Src/Helpers/logger.o 

C_DEPS += \
./Src/Helpers/logger.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Helpers/logger.o: ../Src/Helpers/logger.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F429I_DISC1 -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DDEBUG -DSTM32F429xx -c -I../Inc -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/CMSIS/Include" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/CMSIS/STM32F4xx/Include" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Inc/Helpers" -I"C:/Users/Avinash J/STM32CubeIDE/USB_Prog/USBF4/Drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Helpers/logger.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

