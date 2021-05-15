/*
 * usb_hid_standards.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Avinash
 */

#include <stdint.h>


#ifndef INC_USB_HID_STANDARDS_H_
#define INC_USB_HID_STANDARDS_H_


#define USB_DESCRIPTOR_TYPE_HID 0x21
#define USB_DESCRIPTOR_TYPE_HID_REPORT 0x22
#define USB_HID_COUNTRY_NONE 0

/** \brief USB HID descriptor. */
typedef struct {
	uint8_t bLength; /**<\brief Size of the descriptor (bytes). */
	uint8_t bDescriptorType; /**<\brief Type of the descriptor. */
	uint16_t bcdHID; /**<\brief BCD encoded version that the HID descriptor and device complies to. */
	uint8_t bCountryCode; /**<\brief Country code of the localized device. */
	uint8_t bNumDescriptors; /**<\brief Total number of HID report descriptors for the interface. */
	uint8_t bDescriptorType0; /**<\brief First HID report descriptor type. */
	uint16_t wDescriptorLength0; /**<\brief First HID report descriptor length (bytes). */
}__attribute__((__packed__))UsbHidDescriptor;



#endif /* INC_USB_HID_STANDARDS_H_ */
