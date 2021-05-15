/*
 * usb_framework.h
 *
 *  Created on: Mar 30, 2021
 *      Author: Avinash J
 */
#ifndef STM32F429_USB_FRAMEWORK_H_
#define STM32F429_USB_FRAMEWORK_H_

#include <stm32F429_usb_driver.h>

void USB_Init();
void usbd_poll();

#endif /* STM32F429_USB_FRAMEWORK_H_ */
