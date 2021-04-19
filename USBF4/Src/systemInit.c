/*
 * systemInit.c
 *
 *  Created on: Mar 28, 2021
 *      Author: Avinash J
 */
#include <stdint.h>
#include "system_stm32f4xx.h"
#include "Helpers/logger.h"
#include "stm32f4xx.h"

LogLvl system_log_level = LOG_LEVEL_DEBUG;
uint32_t SysCoreClock = 72000000; //72MHz

//HCLK = 72MHz
//PLL: M = 4, N = 72, P = 2, Q = 3
//APB1 prescaler =2 APB2 prescaler  = 1

static void configureClock(void)
{

	MODIFY_REG(FLASH->ACR,FLASH_ACR_LATENCY,FLASH_ACR_LATENCY_2WS << FLASH_ACR_LATENCY_Pos);

	//Enable HSE
	SET_BIT(RCC->CR,RCC_CR_HSEON);

	//Wait for HSE to be ready
	while(!READ_BIT(RCC->CR,RCC_CR_HSERDY));

	//Turn off PLL
	CLEAR_BIT(RCC->CR,RCC_CR_PLLON);

	//Wait for PLL to be ready be cleared
	while(READ_BIT(RCC->CR,RCC_CR_PLLRDY));


	//Configure PLL: source = HSE, SYCLK=72Mhz
	MODIFY_REG(RCC->PLLCFGR,
			RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLP,
			_VAL2FLD(RCC_PLLCFGR_PLLM, 4) | _VAL2FLD(RCC_PLLCFGR_PLLN, 72) | _VAL2FLD(RCC_PLLCFGR_PLLQ, 3) | RCC_PLLCFGR_PLLSRC_HSE);

	// Configures PPRE1 = 2
		MODIFY_REG(RCC->CFGR,
			RCC_CFGR_PPRE1,_VAL2FLD(RCC_CFGR_PPRE1, 4));

	//Enable PLL Module
	SET_BIT(RCC->CR,RCC_CR_PLLON);

	//Wait until PLL is stable
	while(!READ_BIT(RCC->CR,RCC_CR_PLLRDY));

	//Modify System clock to PLL
	MODIFY_REG(RCC->CFGR,
			RCC_CFGR_SW,
			_VAL2FLD(RCC_CFGR_SW,RCC_CFGR_SW_PLL));


	//Wait until PLL is switched
	while(READ_BIT(RCC->CFGR,RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	//Disable HSI
	CLEAR_BIT(RCC->CR,RCC_CR_HSION);
}

void SystemInit(void)
{
	configureClock();
}

