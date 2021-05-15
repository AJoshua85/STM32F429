/*
 * usb_driver.h
 *
 *  Created on: Mar 30, 2021
 *      Author: Avinash J
 */

#ifndef STM32F429_USB_DRIVER_H_
#define STM32F429_USB_DRIVER_H_

#include "stm32f4xx.h"
#include "usb_standards.h"

#define USB_OTG_HS_GLOBAL ((USB_OTG_GlobalTypeDef *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_GLOBAL_BASE))
#define USB_OTG_HS_DEVICE ((USB_OTG_DeviceTypeDef *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_DEVICE_BASE))
#define USB_OTG_HS_PCGCCTL ((__IO uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_PCGCCTL_BASE)) // Power and clock gating control register

#define ENDPOINT_COUNT 6

/** \brief Returns the structure contains the registers of a specific IN endpoint.
 * \param endpoint_number The number of the IN endpoint we want to access its registers.
 */
inline static USB_OTG_INEndpointTypeDef * IN_ENDPOINT(uint8_t endpoint_number)
{
    return (USB_OTG_INEndpointTypeDef *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_IN_ENDPOINT_BASE + (endpoint_number * 0x20));
}

/** \brief Returns the structure contains the registers of a specific OUT endpoint.
 * \param endpoint_number The number of the OUT endpoint we want to access its registers.
 */
inline static USB_OTG_OUTEndpointTypeDef * OUT_ENDPOINT(uint8_t endpoint_number)
{
    return (USB_OTG_OUTEndpointTypeDef *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_OUT_ENDPOINT_BASE + (endpoint_number * 0x20));
}

inline static __IO uint32_t *FIFO(uint8_t endpoint_number)
{
    return (__IO uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + (endpoint_number * 0x1000));
}


/// \brief USB driver functions exposed to USB framework.
typedef struct
{
	void (*USB_Core_Init)();
	void (*USB_Gpio_Init)();
	void (*set_device_addr)(uint8_t addr);
	void (*connect)();
	void (*disconnect)();
	void (*flush_rxfifo)();
	void (*flush_txfifo)(uint8_t endpoint_num);
	void (*configure_in_endpoint)(uint8_t endpoint_num,enum UsbEndpointType endpoint_type, uint16_t endpoint_size);
	void (*write_packet)(uint8_t endpoint_num, void const *buffer, uint16_t size);
	void (*read_packet)(const void *buffer, uint16_t size);
	void (*poll)();
}UsbDriver;

extern const UsbDriver usb_driver;
extern UsbEvents usb_events;

#endif /* STM32F429_USB_DRIVER_H_ */
