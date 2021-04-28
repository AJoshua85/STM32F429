/*
 * usb_framework.c
 *
 *  Created on: Mar 30, 2021
 *      Author: Avinash J
 */

#include <stddef.h>
#include "stm32F429_usb_framework.h"
#include "stm32F429_usb_driver.h"
#include "usb_standards.h"
#include "usb_device.h"
#include "usbd_descriptors.h"
#include "logger.h"
#include "math.h"


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

			log_info("Standard get descriptor request received.");
			const uint8_t descriptorType = request->wValue >> 8;
			const uint8_t descriptorLength = request->wLength;

			switch(descriptorType)
			{
				case USB_DESCRIPTOR_TYPE_DEVICE:

					log_info("- Get Device Descriptor.");
					usbd_handle->ptr_in_buffer = &device_descriptor;
					usbd_handle->in_data_size = descriptorLength;

					log_info("Switching control stage to IN-DATA.");
					usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_STATUS_IN;
					break;
			}
			break;

		case USB_STANDARD_SET_ADDRESS:
			log_info("Standard Set Address request received");
			const uint16_t deviceAddr = request->wValue;
			usb_driver.set_device_addr(deviceAddr);
			log_info("Switching control stage to IN-STATUS.");
			usbd_handle->device_state = USB_CONTROL_STAGE_STATUS_IN;
			break;




	}
}


/*******************************************************************
 * @fn				- process_request()
 *
 * @brief			- this function reads request from host and processes
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
				process_standard_device_request();
				break;
	}
}
/*******************************************************************
 * @fn				- process_control_transfer_stage(()
 *
 * @brief			- this function monitors the changes
 * 					  in the USB module
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void process_control_transfer_stage()
{
	switch(usbd_handle->control_transfer_stage)
	{
		case USB_CONTROL_STAGE_SETUP:
			break;
		case USB_CONTROL_STAGE_DATA_IN:
			log_info("Processing IN-DATA Stage");

			//Check for the case when packet size is smaller than max packet size
			uint8_t dataSize = MIN(usbd_handle->in_data_size,device_descriptor.bMaxPacketSize0);

			usb_driver.write_packet(0,usbd_handle->ptr_in_buffer,dataSize);

			//Update the the pointer
			usbd_handle->in_data_size -= dataSize;
			usbd_handle->ptr_in_buffer += dataSize;

			log_info("Switching control stage to IN-Data IDLE");
			//Change the control stage in order for the host to fetch data
			usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN_IDLE;

			if(usbd_handle->in_data_size == 0)
			{
				//Case when last packet sent is max packet size
				if(dataSize == device_descriptor.bMaxPacketSize0)
				{
					log_info("Switching control stage to IN-Data ZERO stage");
					usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN_ZERO;
				}
				//Case when last packet sent had smaller size than max packet size
				else
				{
					log_info("Switching control stage to OUT-Status");
					usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_STATUS_OUT;
				}
			}
			break;

		case USB_CONTROL_STAGE_DATA_IN_IDLE:
			break;

		case USB_CONTROL_STAGE_STATUS_OUT:
			log_info("Switching control stage to Setup");
			usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_SETUP;
			break;

		case USB_CONTROL_STAGE_STATUS_IN:
			usb_driver.write_packet(0,NULL,0);
			log_info("Switching control stage to Setup");
			usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_SETUP;
			break;
	}
}

static void usb_polled_handler()
{
	process_control_transfer_stage();
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
static void setup_data_received_handler(uint8_t endpointNum, uint16_t byte_count)
{
	usb_driver.read_packet(usbd_handle->ptr_out_buffer,byte_count);

	log_debug_array("Setup data:",usbd_handle->ptr_out_buffer, byte_count);
	process_request();
}
/*******************************************************************
 * @fn				- in_transfer_completed_handler()
 *
 * @brief			- this function checks if there is data left
 * 					  to be read and switches the control stage
 *
 * @parem[in]		- endpoint number
 * @return			- none
 * @note			- none
 */

static void in_transfer_completed_handler(uint8_t endpointNum)
{
	if(usbd_handle->in_data_size)
	{
		log_info("Switching control stage to IN-Data");
		usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN;
	}
	else if(usbd_handle->control_transfer_stage == USB_CONTROL_STAGE_DATA_IN_ZERO)
	{
		//Send a zero length data to host no more data to be sent
		usb_driver.write_packet(0,NULL,0);
		log_info("Switching control stage to OUT-Status");
		usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_STATUS_OUT;
	}
}

static void out_transfer_completed_handler(uint8_t endpointNum)
{

}

UsbEvents usb_events = {
		.on_usb_reset_received = &USB_reset_receive_handler,
		.on_setup_data_received = &setup_data_received_handler,
		.on_usb_polled = &usb_polled_handler,
		.on_in_transfer_completed = &in_transfer_completed_handler,
		.on_out_transfer_completed = &out_transfer_completed_handler
};

