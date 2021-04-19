/*
 * usb_driver.h
 *
 *  Created on: Mar 30, 2021
 *      Author: Avinash J
 */

#ifndef USB_DRIVER_H_
#define USB_DRIVER_H_

#include "stm32F4xx.h"

#define USB_OTG_HS_GLOBAL ((USB_OTG_GlobalTypeDef *)USB_OTG_HS_PERIPH_BAS))
#define USB_OTG_HS_DEVICE ((USB_OTG_DeviceTypeDef *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_DEVICE_BASE))
#define USB_OTG_HS_PCGCCTL ((uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_PCGCCTL_BASE))

void disconnect();
void connect();
void USB_Core_Init();
void USB_Gpio_Init();
#endif /* USB_DRIVER_H_ */
