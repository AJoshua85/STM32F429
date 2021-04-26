/*
 * usb_framework.c
 *
 *  Created on: Mar 30, 2021
 *      Author: Avinash J
 */

#include "stm32F429_usb_framework.h"
#include "stm32F429_usb_driver.h"
#include "usb_standards.h"
#include "usb_device.h"
#include "logger.h"


static UsbDevice *usbd_handle;

void USB_Init(UsbDevice *Usb_device)
{
	usbd_handle = Usb_device;
	usb_driver.USB_Gpio_Init();
	usb_driver.USB_Core_Init();
	usb_driver.connect();

}
/*******************************************************************
 * @fn				- process_standard_device_request()
 *
 * @brief			- this function reads standard request from host and processes
 * 					  the request
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void process_standard_device_request()
{
	UsbRequest const * request = usbd_handle->ptr_out_buffer;
	switch (request->bRequest)
	{
		case USB_STANDARD_GET_DESCRIPTOR:
			break;
	}
}


/*******************************************************************
 * @fn				- process_request()
 *
 * @brief			- this function reads standard request from host and processes
 * 					  the request
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void process_request()
{
	UsbRequest const * request = usbd_handle->ptr_out_buffer;
	switch(request->bmRequestType & (USB_BM_REQUEST_TYPE_DIRECTION_MASK | USB_BM_REQUEST_TYPE_RECIPIENT_MASK ))
	{
		case ( USB_BM_REQUEST_TYPE_TYPE_STANDARD | USB_BM_REQUEST_TYPE_RECIPIENT_DEVICE ):

		break;
	}
}
/*******************************************************************
 * @fn				- usbd_poll()
 *
 * @brief			- this function monitors the changes
 * 					  in the USB module
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- function is used for bus polling
 */
void usbd_poll()
{
	usb_driver.poll();
}
/*******************************************************************
 * @fn				- USB_reset_handler()
 *
 * @brief			- this function configures the USB device
 * 					  upon reset signal
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void USB_reset_receive_handler()
{
	usbd_handle->in_data_size = 0;
	usbd_handle->out_data_size= 0;
	usbd_handle->configuration_value =0;
	usbd_handle->device_state = USB_DEVICE_STATE_DEFAULT;
	usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_SETUP;
	usb_driver.set_device_addr(0);
}
/*******************************************************************
 * @fn				- setup_data_received_handler()
 *
 * @brief			- this function reads data out of endpoint0 during
 * 					  setup phase of the USB device
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void setup_data_received_handler(uint8_t endpoint_num, uint16_t byte_count)
{
	usb_driver.read_packet(usbd_handle->ptr_out_buffer,byte_count);

	log_debug_array("Setup data:",usbd_handle->ptr_out_buffer, byte_count);
	process_request();
}

UsbEvents usb_events = {
		.on_usb_reset_received = &USB_reset_receive_handler,
		.on_setup_data_received = &setup_data_received_handler
};

