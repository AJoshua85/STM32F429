/*
 * usb_framework.c
 *
 *  Created on: Mar 30, 2021
 *      Author: Avinash J
 */

#include "stm32F429_usb_framework.h"

void USB_Init()
{
	USB_Gpio_Init();
	USB_Core_Init();
	connect();

}
