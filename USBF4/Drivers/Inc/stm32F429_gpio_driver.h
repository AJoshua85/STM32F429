/*
 * gpio_driver.h
 *
 *  Created on: Mar 31, 2021
 *      Author: Avinash J
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include "stm32F4xx.h"

/*Configuration structure for GPIOx peripheral*/
typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;		/* !<possible values from @GPIO_PIN_MODES>*/
	uint8_t GPIO_PinSpeed;		/* !<possible values from @GPIO_PIN_SPEED>*/
	uint8_t GPIO_PinPuPdCtrl;	/* !<possible values from @GPIO_PIN_PU_PD_CONFIG>*/
	uint8_t	GPIO_PinOPType;		/* !<possible values from @GPIO_PIN_OUTPUT_TYPE>*/
	uint8_t GPIO_PinAltFn;
}GPIO_PinConfig_t;

/*Structure handle for a GPIO pin*/
typedef struct
{
	GPIO_TypeDef *pGPIOx; /* Stores base address of GPIO port it is configured to*/
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;

/*@GPIO_PIN_MODES
 * GPIO pin possible modes*/
#define GPIO_MODE_INPUT 		0
#define GPIO_MODE_OUTPUT		1
#define GPIO_MODE_ALTFN			2
#define GPIO_MODE_ANALOG		3
#define GPIO_MODE_IT_F_EDGE		4
#define GPIO_MODE_IT_R_EDGE		5
#define GPIO_MODE_IT_RF_EDGE	6

/*@GPIO_PIN_OUTPUT_TYPE
 * GPIO pin possible output types*/
#define GPIO_OP_TYPE_PP			0
#define GPIO_OP_TYPE_OD			1

/*@GPIO_PIN_SPEED
 * GPIO pin possible output speeds*/
#define GPIO_SPD_LOW			0
#define GPIO_SPD_MED			1
#define GPIO_SPD_FAST			2
#define GPIO_SPD_HIGH			3

/*@GPIO_PIN_PU_PD_CONFIG
 * GPIO pin pull up and pull down configuration macros*/
#define GPIO_NO_PUPD			0
#define GPIO_PIN_PU				1
#define GPIO_PIN_PD				2

/*Clock Enable Macros for SYSCFG peripheral*/
#define SYSCFG_PCLK_EN() 		( RCC->APB2ENR |= ( 1 << 0 ) )

/*returns port code for given GPIOx base address*/
#define GPIO_BASEADDR_TO_CODE(x)	(	( x == GPIOA ) ? 0: \
										( x == GPIOB ) ? 1: \
										( x == GPIOC ) ? 2: 0) \

/* Clock Enable Macros for GPIOx peripherals*/
#define GPIOA_PCLK_EN()			( RCC->AHB2ENR |= ( 1 << 0 ) )
#define GPIOB_PCLK_EN()			( RCC->AHB2ENR |= ( 1 << 1 ) )
#define GPIOC_PCLK_EN()			( RCC->AHB2ENR |= ( 1 << 2 ) )


/*Clock Disable Macros for GPIOx peripherals */
#define GPIOA_PCLK_DI()			( RCC->AHB2ENR &= ~ ( 1 << 0 ) )
#define GPIOB_PCLK_DI()			( RCC->AHB2ENR &= ~ ( 1 << 1 ) )
#define GPIOC_PCLK_DI()			( RCC->AHB2ENR &= ~ ( 1 << 2 ) )

/* Macros to reset GPIOx peripherals*/
#define GPIOA_REG_RESET()		 do{ ( RCC->AHB2RSTR |= ( 1 << 0 ) ); ( RCC->AHB2RSTR &= ~( 1 << 0 ) ); }while(0)
#define GPIOB_REG_RESET()		 do{ ( RCC->AHB2RSTR |= ( 1 << 1 ) ); ( RCC->AHB2RSTR &= ~( 1 << 1 ) ); }while(0)
#define GPIOC_REG_RESET()		 do{ ( RCC->AHB2RSTR |= ( 1 << 2 ) ); ( RCC->AHB2RSTR &= ~( 1 << 2 ) ); }while(0)

/*generic macros*/
#define ENABLE				1U
#define DISABLE 			0U
#define SET 				ENABLE
#define RESET				DISABLE
#define PIN_SET 			SET
#define PIN_RESET 			RESET
#define FLAG_RESET			RESET
#define FLAG_SET			SET
#define FAIL				SET
#define SUCCESS				RESET


/*****************************************************************************
 * 						APIs supported by this driver
 * 		For more information about the APIs check the function definitions
 *****************************************************************************/

/*Peripheral Clock setup*/
void GPIO_PclkCtrl(GPIO_TypeDef *pGPIOx, uint8_t EnOrDi);

/*Init and De-init*/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeIit(GPIO_TypeDef *pGPIOx);


/*Data read and write*/
uint8_t GPIO_ReadFromInputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNum);
uint16_t GPIO_ReadFromInputPort(GPIO_TypeDef *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNum,uint8_t Val);
void GPIO_WriteToOutputPort(GPIO_TypeDef *pGPIOx, uint16_t Val);
void GPIO_ToggleOutputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNum);

/*IRQ Configuration and ISR Handling*/
void GPIO_IRQITConfig(uint8_t IRQNumber,uint8_t EnOrDi);
void GPIO_IRQHandling(uint8_t PinNumber);


#endif /* GPIO_DRIVER_H_ */
