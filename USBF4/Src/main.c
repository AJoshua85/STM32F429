/**
  ******************************************************************************
  * @file    main.c
  * @author  Auto-generated by STM32CubeIDE
  * @version V1.0
  * @brief   Default main function.
  ******************************************************************************
*/

#include "logger.h"
#include "stm32F429_usb_framework.h"

int main(void)
{
	/*
	GPIO_Handle_t USBPins;
	USBPins.pGPIOx = GPIOA;
	USBPins.GPIO_PinConfig.GPIO_PinNumber =11;
	USBPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	USBPins.GPIO_PinConfig.GPIO_PinAltFn = 10;
	GPIO_Init(&USBPins);

	USBPins.GPIO_PinConfig.GPIO_PinNumber =12;
	GPIO_Init(&USBPins);*/


	USB_Init();
	log_info("Program entry point.");

	for(;;);
}
