/*
 * usb_driver.c
 *
 *  Created on: Mar 30, 2021
 *      Author: Avinash J
 */
#include "stm32F429_usb_driver.h"
#include "usb_standards.h"
#include "string.h"
#include "logger.h"

static void flush_rxfifo();
static void flush_txfifo(uint8_t endpoint_number);
static void config_endpoint0(uint16_t endpoint_size);
static void configure_in_endpoint(uint8_t endpoint_num,UsbEndpointType endpoint_type, uint16_t endpoint_size);
static void deconfigure_in_endpoint(uint8_t endpoint_num);
static void configure_txfifo_size(uint8_t endpoint_num,uint16_t size);
static void configure_rxfifo_size(uint16_t size);
static void refresh_fifo_start_addr(void);



/*******************************************************************
 * @fn				- USB_Gpio_Init()
 *
 * @brief			- This function Initializes GPIO pin Setting
 * 					  to be configured for USB
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */

static void USB_Gpio_Init()
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
/*******************************************************************
 * @fn				- USB_Core_Init()
 *
 * @brief			- this function configures USB
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void USB_Core_Init()
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

	 /*Enable USB core interrupts
	 *WUIM: Resume/remote wakeup detected interrupt mask
	 *USBRST: USB reset mask
	 *ENUMDNEM: Enumeration done mask
	 *SOFM: Start of frame mask
	 *USBSUSPM: USB suspend mask
	 *WUIM: Resume/remote wakeup detected interrupt mask
	 *IEPINT: IN endpoints interrupt mask
	 *RXFLVLM: Receive FIFO non-empty mask*/
	 SET_BIT(USB_OTG_HS->GINTMSK,
	 		USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_SOFM |
	 		USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_IEPINT |
	 		USB_OTG_GINTSTS_OEPINT | USB_OTG_GINTMSK_RXFLVLM);

	 	// Clears all pending core interrupts.
	 WRITE_REG(USB_OTG_HS->GINTSTS, 0xFFFFFFFF);

	 // Unmasks USB global interrupt.
	 SET_BIT(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_GINT);

/*	 SET_BIT(USB_OTG_HS->GINTMSK,
			 USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_SOFM |
			 USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_IEPINT |
			 USB_OTG_GINTMSK_RXFLVLM | USB_OTG_GINTMSK_OTGINT | USB_OTG_GINTMSK_MMISM );

	 //Clear pending interrupts
	 WRITE_REG(USB_OTG_HS->GINTSTS,0xFFFFFFFF);

	 //Enable USB global interrupt
	 SET_BIT(USB_OTG_HS->GAHBCFG,USB_OTG_GAHBCFG_GINT);*/

	 // Unmasks transfer completed interrupts for all endpoints.
	 SET_BIT(USB_OTG_HS_DEVICE->DOEPMSK, USB_OTG_DOEPMSK_XFRCM);
	 SET_BIT(USB_OTG_HS_DEVICE->DIEPMSK, USB_OTG_DIEPMSK_XFRCM);
}
/*******************************************************************
 * @fn				- set_device_addr()
 *
 * @brief			- this function configures USB
 * 					  device address
 *
 * @parem[in]		- address
 * @return			- none
 * @note			- none
 */
static void set_device_addr(uint8_t addr)
{
	MODIFY_REG(USB_OTG_HS_DEVICE->DCFG,USB_OTG_DCFG_DAD,_VAL2FLD(USB_OTG_DCFG_DAD,addr));
}
/*******************************************************************
 * @fn				- connect()
 *
 * @brief			- this function establishes a connection
 * 					  with the USB bus
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void connect()
{
	//Power the transceiver on
	SET_BIT(USB_OTG_HS->GCCFG,USB_OTG_GCCFG_PWRDWN);

	//Connect the device to the bus
	CLEAR_BIT(USB_OTG_HS_DEVICE->DCTL,USB_OTG_DCTL_SDIS);
}
/*******************************************************************
 * @fn				- disconnect()
 *
 * @brief			- this function removes the connection
 * 					  with the USB bus
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void disconnect()
{
	//Disconnect the the device from the bus
	SET_BIT(USB_OTG_HS_DEVICE->DCTL,USB_OTG_DCTL_SDIS);

	//Power the transceiver off
	CLEAR_BIT(USB_OTG_HS->GCCFG,USB_OTG_GCCFG_PWRDWN);
}
/*******************************************************************
 * @fn				- flush_rxfifo()
 *
 * @brief			- this function flushes the TxFIFO of
 * 					  an OUT endpoint
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void flush_rxfifo()
{
	SET_BIT(USB_OTG_HS->GRSTCTL, USB_OTG_GRSTCTL_RXFFLSH);
}
/*******************************************************************
 * @fn				- flush_txfifo()
 *
 * @brief			- this function flushes the TxFIFO of
 * 					  an IN endpoint
 *
 * @parem[in]		- IN endpoint
 * @return			- none
 * @note			- none
 */
static void flush_txfifo(uint8_t endpoint_num)
{
	//Select specific tx fifo to flush
	MODIFY_REG(USB_OTG_HS->GRSTCTL,USB_OTG_GRSTCTL_TXFNUM,_VAL2FLD(USB_OTG_GRSTCTL_TXFNUM, endpoint_num));

	//Trigger flush
	SET_BIT(USB_OTG_HS->GRSTCTL,USB_OTG_GRSTCTL_TXFFLSH);
}
/*******************************************************************
 * @fn				- config_endpoint0()
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void config_endpoint0(uint16_t endpoint_size)
{
	//Unmasks interrupts of endpoint0 IN (bit 0) /OUT (bit 16)
	SET_BIT(USB_OTG_HS_DEVICE->DAINTMSK,(1 << 0| 1 << 16));

	//Config max packet size,
	MODIFY_REG(IN_ENDPOINT(0)->DIEPCTL,USB_OTG_DIEPCTL_MPSIZ,_VAL2FLD(USB_OTG_DIEPCTL_MPSIZ,endpoint_size));

	//activate endpoint configuration and sets endpoint handshake to NAK (not ready to send data)
	SET_BIT(IN_ENDPOINT(0)->DIEPCTL,USB_OTG_DIEPCTL_USBAEP|USB_OTG_DIEPCTL_SNAK);

	//Clear NAK and enables endpoint data transmission
	SET_BIT(OUT_ENDPOINT(0)->DOEPCTL,USB_OTG_DOEPCTL_EPENA|USB_OTG_DOEPCTL_CNAK);

	//USB Full speed devices has maximum packet size of 64 Bytes
	configure_rxfifo_size(64);
	configure_txfifo_size(0,endpoint_size);

}
/*******************************************************************
 * @fn				- USBReset_handler()
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void USBReset_handler()
{
	log_info("USB reset signal was detected.");
	for(uint8_t i = 0 ; i <=ENDPOINT_COUNT; i++)
	{
		deconfigure_in_endpoint(i);
	}
	usb_events.on_usb_reset_received();
}
/*******************************************************************
 * @fn				- EnumerationDone_handler()
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void EnumerationDone_handler()
{
	log_info("USB device speed enumeration done.");
	//temp value used of 8
	config_endpoint0(8);
}
/*******************************************************************
 * @fn				- RxFifoNonEmpty_handler()
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void RxFifoNonEmpty_handler()
{
	//Pop the metadata info from the RxFIFO
	uint32_t recieve_status = USB_OTG_HS_GLOBAL->GRXSTSP;

	//extract which end point received the data
	uint8_t endpoint_num= _FLD2VAL(USB_OTG_GRXSTSP_EPNUM,recieve_status);
	//extract number of bytes received
	uint8_t bcnt = _FLD2VAL(USB_OTG_GRXSTSP_BCNT,recieve_status);
	//extract the status of received packet
	uint8_t pckstats = _FLD2VAL(USB_OTG_GRXSTSP_PKTSTS,recieve_status);

	switch(pckstats)
	{
		//Setup packet (include data)
		case 0x06:
			usb_events.on_setup_data_received(endpoint_num,bcnt);
			break;
		//out packet (include data)
		case 0x02:
			break;
		//SETUP stage has completed
		case 0x04:
			//Re-enable the transmission on endpoint
			SET_BIT(OUT_ENDPOINT(endpoint_num)->DOEPCTL,USB_OTG_DOEPCTL_CNAK|USB_OTG_DOEPCTL_EPENA);
			break;
		//OUT transfer has completed
		case 0x03:
			//Re-enable the transmission on endpoint
			SET_BIT(OUT_ENDPOINT(endpoint_num)->DOEPCTL,USB_OTG_DOEPCTL_CNAK|USB_OTG_DOEPCTL_EPENA);
			break;

	}
}
/*******************************************************************
 * @fn				- INEndpoint_hander()
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void INEndpoint_hander(void)
{
	// Finds the endpoint caused the interrupt.
	uint8_t endpointNum = ffs(USB_OTG_HS_DEVICE->DAINT) - 1;

	//Check if transfer of given endpoint is completed
	if (IN_ENDPOINT(endpointNum)->DIEPINT & USB_OTG_DIEPINT_XFRC)
    {
        usb_events.on_in_transfer_completed(endpointNum);
        // Clears the interrupt flag.
        SET_BIT(IN_ENDPOINT(endpointNum)->DIEPINT, USB_OTG_DIEPINT_XFRC);
    }
}
/*******************************************************************
 * @fn				- RxFifoNonEmpty_handler
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void OUTEndpoint_hander(void)
{
	// Finds the endpoint caused the interrupt.
	uint8_t endpointNum = ffs(USB_OTG_HS_DEVICE->DAINT >> 16) - 1;

	//Check if transfer of given endpoint is completed
	if (OUT_ENDPOINT(endpointNum)->DOEPINT & USB_OTG_DOEPINT_XFRC)
	{
		usb_events.on_out_transfer_completed(endpointNum);
	    // Clears the interrupt;
	    SET_BIT(OUT_ENDPOINT(endpointNum)->DOEPINT, USB_OTG_DOEPINT_XFRC);
	}
}

/*******************************************************************
 * @fn				- configure_in_endpoint
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void configure_in_endpoint(uint8_t endpoint_num,UsbEndpointType endpoint_type, uint16_t endpoint_size)
{
	//Unmasks IN endpoint interrupts
	SET_BIT(USB_OTG_HS_DEVICE->DAINTMSK,1 << endpoint_num);

	//Config max packet size
	MODIFY_REG(IN_ENDPOINT(endpoint_num)->DIEPCTL,USB_OTG_DIEPCTL_MPSIZ,_VAL2FLD(USB_OTG_DIEPCTL_MPSIZ,endpoint_size));

	//Config endpoint type
	MODIFY_REG(IN_ENDPOINT(endpoint_num)->DIEPCTL,USB_OTG_DIEPCTL_EPTYP,_VAL2FLD(USB_OTG_DIEPCTL_EPTYP,endpoint_type));

	//set the endpoint data PID
	SET_BIT(IN_ENDPOINT(endpoint_num)->DIEPCTL,USB_OTG_DIEPCTL_SD0PID_SEVNFRM);

	//Assign fifo number to same endpoint number
	MODIFY_REG(IN_ENDPOINT(endpoint_num)->DIEPCTL,USB_OTG_DIEPCTL_TXFNUM,_VAL2FLD(USB_OTG_DIEPCTL_TXFNUM,endpoint_num));

	//activate endpoint configuration and sets endpoint handshake to NAK (not ready to send data)
	SET_BIT(IN_ENDPOINT(endpoint_num)->DIEPCTL,USB_OTG_DIEPCTL_USBAEP|USB_OTG_DIEPCTL_SNAK);

	configure_txfifo_size(endpoint_num,endpoint_size);
}
/*******************************************************************
 * @fn				- deconfigure_in_endpoint
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void deconfigure_in_endpoint(uint8_t endpoint_num)
{
	USB_OTG_INEndpointTypeDef *in_endpoint = IN_ENDPOINT(endpoint_num);
	USB_OTG_OUTEndpointTypeDef *out_endpoint = OUT_ENDPOINT(endpoint_num);

	//masks interrupts of IN/OUT endpoint
	CLEAR_BIT(USB_OTG_HS_DEVICE->DAINTMSK,(1 << endpoint_num| 1 << (16 << endpoint_num)));

	//Clear pending endpoint interrupts
	SET_BIT(in_endpoint->DIEPINT, 0x29FF);
	SET_BIT(out_endpoint->DOEPINT, 0x715F);

	//Check if transmission is enabled
	if(in_endpoint->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
	{
		//Disable endpoint transmission
		SET_BIT(in_endpoint->DIEPCTL,USB_OTG_DIEPCTL_EPDIS);
	}

	//Disable IN endpoints
	CLEAR_BIT(in_endpoint->DIEPCTL,USB_OTG_DIEPCTL_USBAEP);

	if(endpoint_num != 0)
	{
		//Check if transmission is enabled
		if(out_endpoint->DOEPCTL & USB_OTG_DOEPCTL_EPENA)
		{
			//Disable endpoint transmission
			SET_BIT(out_endpoint->DOEPCTL,USB_OTG_DOEPCTL_EPDIS);
		}
	}
	//Disable OUT endpoints
	CLEAR_BIT(out_endpoint->DOEPCTL,USB_OTG_DOEPCTL_USBAEP);

	//Flush the FIFOS.
	flush_txfifo(endpoint_num);
	flush_rxfifo();
}
/*******************************************************************
 * @fn				- refresh_fifo_start_addr
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void refresh_fifo_start_addr(void)
{
	//starting address begins after the region of RxFIFO
	uint32_t start_addr = _FLD2VAL(USB_OTG_GRXFSIZ_RXFD,USB_OTG_HS->GRXFSIZ)*4;

	//Update the start address of TxFIFO0.
	MODIFY_REG(USB_OTG_HS->DIEPTXF0_HNPTXFSIZ,USB_OTG_TX0FSA,_VAL2FLD(USB_OTG_TX0FSA,start_addr));

	// Updates the start addresses of the rest TxFIFOs.
	for (uint8_t txfifo_number = 0; txfifo_number < ENDPOINT_COUNT - 1; txfifo_number++)
	{
		MODIFY_REG(USB_OTG_HS->DIEPTXF[txfifo_number],USB_OTG_NPTXFSA,_VAL2FLD(USB_OTG_NPTXFSA, start_addr));
		start_addr += _FLD2VAL(USB_OTG_NPTXFD, USB_OTG_HS->DIEPTXF[txfifo_number]) * 4;
	}
}
/*******************************************************************
 * @fn				- configure_txfifo_size
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void configure_txfifo_size(uint8_t endpoint_num,uint16_t size)
{
	//Get the fifo size in terms of 32bit words
	size = (size + 3) / 4;

	//Configure the depth of the fifo
	if(endpoint_num == 0)
	{
		MODIFY_REG(USB_OTG_HS->DIEPTXF0_HNPTXFSIZ,USB_OTG_TX0FD,_VAL2FLD(USB_OTG_TX0FD,size));
	}
	else
	{
		MODIFY_REG(USB_OTG_HS->DIEPTXF[endpoint_num -1],USB_OTG_NPTXFD,_VAL2FLD(USB_OTG_NPTXFD,size));
	}
	refresh_fifo_start_addr();
}
/*******************************************************************
 * @fn				- configure_rxfifo_size
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void configure_rxfifo_size(uint16_t size)
{
	size = 10 + (2* ((size/4) + 1 ));

	//Configure the depth of the fifo
	MODIFY_REG(USB_OTG_HS->GRXFSIZ,USB_OTG_GRXFSIZ_RXFD,_VAL2FLD(USB_OTG_GRXFSIZ_RXFD,size));
	refresh_fifo_start_addr();
}
/*******************************************************************
 * @fn				- read_packet
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void read_packet(const void *buffer, uint16_t size)
{
	//Only one RxFIFO
	volatile uint32_t *fifo = FIFO(0);
	uint32_t data;

	for(;size >=4; size-=4, buffer +=4)
	{
		//Pop 32bit word from the que
		data = *(fifo);
		//Store the data in the buffer
		*((uint32_t*)buffer) = data;
	}

	if(size > 0)
	{
		data = *(fifo);
		for(;size >= 0; size--, buffer++, data >>= 8)
		{
			//Store the remaining data in the buffer with correct alignment
			*((uint8_t*)buffer) = 0xFF & data;//?? Wrong aligment?
		}
	}
}
/*******************************************************************
 * @fn				- write_packet
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void write_packet(uint8_t endpoint_num, void const *buffer, uint16_t size)
{
	volatile uint32_t *fifo = FIFO(endpoint_num);
	USB_OTG_INEndpointTypeDef *in_endpoint = IN_ENDPOINT(endpoint_num);


	// Configures the number of packets to 1
	MODIFY_REG(in_endpoint->DIEPTSIZ,USB_OTG_DIEPTSIZ_PKTCNT,_VAL2FLD(USB_OTG_DIEPTSIZ_PKTCNT, 1));
	//Configure number of bytes in the packet
	MODIFY_REG(in_endpoint->DIEPTSIZ,USB_OTG_DIEPTSIZ_XFRSIZ,_VAL2FLD(USB_OTG_DIEPTSIZ_XFRSIZ, size));

	// Enables the transmission after clearing both STALL and NAK of the endpoint.
	MODIFY_REG(in_endpoint->DIEPCTL,USB_OTG_DIEPCTL_STALL,USB_OTG_DIEPCTL_CNAK | USB_OTG_DIEPCTL_EPENA);

	// Gets the size in term of 32-bit words (to avoid integer overflow in the loop).
	size = (size + 3) / 4;

	for (; size > 0; size--, buffer += 4)
	{
		// Pushes the data to the TxFIFO.
		*fifo = *((uint32_t *)buffer);
	}
}
/*******************************************************************
 * @fn				- USBInterrupt_handler
 *
 * @brief			- this function configures USART
 * 					  peripheral settings
 *
 * @parem[in]		- none
 * @return			- none
 * @note			- none
 */
static void USBInterrupt_handler()
{
	volatile uint32_t gintstReg = USB_OTG_HS_GLOBAL->GINTSTS;

	if(gintstReg & USB_OTG_GINTSTS_USBRST)
	{
		USBReset_handler();
		SET_BIT(USB_OTG_HS->GINTSTS,USB_OTG_GINTSTS_USBRST);
	}
	else if(gintstReg & USB_OTG_GINTSTS_ENUMDNE)
	{
		EnumerationDone_handler();
		SET_BIT(USB_OTG_HS->GINTSTS,USB_OTG_GINTSTS_ENUMDNE);
	}
	else if(gintstReg & USB_OTG_GINTSTS_RXFLVL)
	{
		RxFifoNonEmpty_handler();
		SET_BIT(USB_OTG_HS->GINTSTS,USB_OTG_GINTSTS_RXFLVL);
	}
	else if (gintstReg & USB_OTG_GINTSTS_IEPINT)
	{
		INEndpoint_hander();
		SET_BIT(USB_OTG_HS->GINTSTS,USB_OTG_GINTSTS_IEPINT);
	}
	else if (gintstReg & USB_OTG_GINTSTS_OEPINT)
	{
		OUTEndpoint_hander();
		SET_BIT(USB_OTG_HS->GINTSTS,USB_OTG_GINTSTS_OEPINT);
	}

	usb_events.on_usb_polled();
}
const UsbDriver usb_driver = {
		.USB_Core_Init = &USB_Core_Init,
		.USB_Gpio_Init= &USB_Gpio_Init,
		.set_device_addr = &set_device_addr,
		.connect = &connect,
		.disconnect = &disconnect,
		.flush_rxfifo = &flush_rxfifo,
		.configure_in_endpoint= &configure_in_endpoint,
		.write_packet = &write_packet,
		.read_packet = &read_packet,
		.poll = &USBInterrupt_handler
};



