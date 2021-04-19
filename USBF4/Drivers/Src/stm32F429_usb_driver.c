/*
 * usb_driver.c
 *
 *  Created on: Mar 30, 2021
 *      Author: Avinash J
 */
#include "stm32F429_usb_driver.h"

void USB_Gpio_Init()
{
	//Enable clock for GPIOB
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);


	//Configure Alternate function for pin 14 and 15
	MODIFY_REG(GPIOB->AFR[1],
			GPIO_AFRH_AFSEL14 | GPIO_AFRH_AFSEL15,
			_VAL2FLD(GPIO_AFRH_AFSEL14, 0xC) | _VAL2FLD(GPIO_AFRH_AFSEL15, 0xC));


	//Configure pin as alternate mode
	MODIFY_REG(GPIOB->MODER,
			GPIO_MODER_MODER14 | GPIO_MODER_MODER15,
			_VAL2FLD(GPIO_MODER_MODER14, 2) | _VAL2FLD(GPIO_MODER_MODER15, 2));


}
 void USB_Core_Init()
{
	 //Enable peripheral clock to USB module
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN);

	 //Initialize the core to run in device mode
	 MODIFY_REG(USB_OTG_HS->GUSBCFG,
	 		USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL | USB_OTG_GUSBCFG_TRDT,
	 		USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL | _VAL2FLD(USB_OTG_GUSBCFG_TRDT, 0x09));


	 //Initialize USB Speed to run at Full Speed
	 MODIFY_REG(USB_OTG_HS_DEVICE->DCFG,
	 		USB_OTG_DCFG_DSPD,
	 		_VAL2FLD(USB_OTG_DCFG_DSPD, 0x03));


	 //Enable VBUS sensing device
	 SET_BIT(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_VBUSBSEN);

	 // Unmasks the main USB core interrupts.
	 SET_BIT(USB_OTG_HS->GINTMSK,
	 		USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_SOFM |
	 		USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_IEPINT |
	 		USB_OTG_GINTSTS_OEPINT | USB_OTG_GINTMSK_RXFLVLM);

	 	// Clears all pending core interrupts.
	 WRITE_REG(USB_OTG_HS->GINTSTS, 0xFFFFFFFF);

	 // Unmasks USB global interrupt.
	 SET_BIT(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_GINT);




//	 //Enable USB core interrupts
//	 /*WUIM: Resume/remote wakeup detected interrupt mask
//	  *USBRST: USB reset mask
//	  *ENUMDNEM: Enumeration done mask
//	  *SOFM: Start of frame mask
//	  *USBSUSPM: USB suspend mask
//	  *WUIM: Resume/remote wakeup detected interrupt mask
//	  *IEPINT: IN endpoints interrupt mask
//	  *RXFLVLM: Receive FIFO non-empty mask*/
//
//	 SET_BIT(USB_OTG_HS->GINTMSK,
//			 USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_SOFM |
//			 USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_IEPINT |
//			 USB_OTG_GINTMSK_RXFLVLM | USB_OTG_GINTMSK_OTGINT | USB_OTG_GINTMSK_MMISM );
//
//	 //Clear pending interrupts
//	 WRITE_REG(USB_OTG_HS->GINTSTS,0xFFFFFFFF);
//
//	 //Enable USB global interrupt
//	 SET_BIT(USB_OTG_HS->GAHBCFG,USB_OTG_GAHBCFG_GINT);
}

 void connect()
 {
	 //Power the transceiver on
	 SET_BIT(USB_OTG_HS->GCCFG,USB_OTG_GCCFG_PWRDWN);

	 //Connect the device to the bus
	 CLEAR_BIT(USB_OTG_HS_DEVICE->DCTL,USB_OTG_DCTL_SDIS);
 }

 void disconnect()
 {
	 //Disconnect the the device from the bus
	 SET_BIT(USB_OTG_HS_DEVICE->DCTL,USB_OTG_DCTL_SDIS);

	 //Power the transceiver off
	 CLEAR_BIT(USB_OTG_HS->GCCFG,USB_OTG_GCCFG_PWRDWN);
 }

