/*
 * usbd_descriptors.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Avinash J
 */


#ifndef USBD_DESCRIPTORS_H_
#define USBD_DESCRIPTORS_H_

#include "usb_standards.h"
#include "usb_hid_standards.h"
#include "usb_hid_usage_desktop.h"
#include "usb_hid_usage_button.h"
#include "usb_hid.h"

const UsbDeviceDescriptor device_descriptor = {
    .bLength            = sizeof(UsbDeviceDescriptor),
    .bDescriptorType    = USB_DESCRIPTOR_TYPE_DEVICE,
    .bcdUSB             = 0x0200, // 0xJJMN : JJ (Major version) M (Minor version) N (Sub version)
    .bDeviceClass       = USB_CLASS_PER_INTERFACE,
    .bDeviceSubClass    = USB_SUBCLASS_NONE,
    .bDeviceProtocol    = USB_PROTOCOL_NONE,
    .bMaxPacketSize0    = 8,
    .idVendor           = 0x6666,
    .idProduct          = 0x13AA,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0,
    .iProduct           = 0,
    .iSerialNumber      = 0,
    .bNumConfigurations = 1,
};
/*Reference : https://www.beyondlogic.org/usbnutshell/usb5.shtml#ConfigurationDescriptors*/

const uint8_t hid_report_descriptor[] = {
	HID_USAGE_PAGE(HID_PAGE_DESKTOP),
	HID_USAGE(HID_DESKTOP_MOUSE),
	HID_COLLECTION(HID_APPLICATION_COLLECTION),
		HID_USAGE(HID_DESKTOP_POINTER),
		HID_COLLECTION(HID_PHYSICAL_COLLECTION),
			HID_USAGE(HID_DESKTOP_X),
			HID_USAGE(HID_DESKTOP_Y),
			HID_LOGICAL_MINIMUM(-127),
			HID_LOGICAL_MAXIMUM(127),
			HID_REPORT_SIZE(8),//Number of bits used 8 for positional data
			HID_REPORT_COUNT(2),
			HID_INPUT(HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_RELATIVE),

			/*button Report*/
			HID_USAGE_PAGE(HID_PAGE_BUTTON),
			HID_USAGE_MINIMUM(1),
			HID_USAGE_MAXIMUM(3),
			HID_LOGICAL_MINIMUM(0),
			HID_LOGICAL_MAXIMUM(1),
			HID_REPORT_SIZE(1),//Number of bits used 0/1 on off
			HID_REPORT_COUNT(3),
			HID_INPUT(HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
			/*Padding bits for unused bits*/
			HID_REPORT_SIZE(1),
			HID_REPORT_COUNT(5),
			HID_INPUT(HID_IOF_CONSTANT),
		HID_END_COLLECTION,
	HID_END_COLLECTION
};

typedef struct {
	UsbConfigurationDescriptor usbConfigDescriptor;
	UsbInterfaceDescriptor usbInterfaceDescriptor;
	UsbHidDescriptor usbMouseHidDescriptor;
	UsbEndpointDescriptor usbMouseEndpointDescriptor;
} UsbConfigurationDescriptorCombination;

const UsbConfigurationDescriptorCombination configDescriptorCombination = {
	.usbConfigDescriptor = {
		.bLength                = sizeof(UsbConfigurationDescriptor),
		.bDescriptorType        = USB_DESCRIPTOR_TYPE_CONFIGURATION,
		.wTotalLength           = sizeof(UsbConfigurationDescriptorCombination),
		.bNumInterfaces         = 1,
		.bConfigurationValue    = 1,
		.iConfiguration         = 0,
		.bmAttributes           = 0x80 | 0x40,
		.bMaxPower              = 25
	},
	.usbInterfaceDescriptor = {
		.bLength                = sizeof(UsbInterfaceDescriptor),
		.bDescriptorType        = USB_DESCRIPTOR_TYPE_INTERFACE,
		.bInterfaceNumber       = 1,
		.bAlternateSetting      = 0,
		.bNumEndpoints          = 1,
		.bInterfaceClass        = USB_CLASS_HID,
		.bInterfaceSubClass     = USB_PROTOCOL_NONE,
		.bInterfaceProtocol     = USB_PROTOCOL_NONE,
		.iInterface             = 0
	},
    .usbMouseEndpointDescriptor = {
        .bLength                = sizeof(UsbEndpointDescriptor),
        .bDescriptorType        = USB_DESCRIPTOR_TYPE_ENDPOINT,
        .bEndpointAddress       = 0x83,
        .bmAttributes           = USB_ENDPOINT_TYPE_INTERRUPT,
        .wMaxPacketSize         = 64,
        .bInterval              = 50
    },
    .usbMouseHidDescriptor = {
        .bLength                = sizeof(UsbHidDescriptor),
        .bDescriptorType        = USB_DESCRIPTOR_TYPE_HID,
        .bcdHID                 = 0x0100,
        .bCountryCode           = USB_HID_COUNTRY_NONE,
        .bNumDescriptors        = 1,
        .bDescriptorType0       = USB_DESCRIPTOR_TYPE_HID_REPORT,
        .wDescriptorLength0     = sizeof(hid_report_descriptor)
    }
};

typedef struct {
	int8_t      x;
	int8_t      y;
	uint8_t     buttons;
} __attribute__((__packed__)) HidReport;

#endif /* USBD_DESCRIPTORS_H_ */
