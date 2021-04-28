/*
 * usbd_descriptors.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Avinash J
 */

#ifndef INC_USBD_DESCRIPTORS_H_
#define INC_USBD_DESCRIPTORS_H_

#include "usb_standards.h"

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

#endif /* INC_USBD_DESCRIPTORS_H_ */
