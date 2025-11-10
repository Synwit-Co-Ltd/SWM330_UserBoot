/*******************************************************************************************************************************
* @brief	pin function set
*
*
********************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology 
*******************************************************************************************************************************/
#include "SWM330.h"
#include "SWM330_port.h"


/*******************************************************************************************************************************
* @brief	pin function set
* @param	PORTx is port of pin
* @param	n is pin number in port
* @param	func is pin function to use
* @param	digit_in_en is digital input enable
* @return
*******************************************************************************************************************************/
void PORT_Init(PORT_TypeDef * PORTx, uint32_t n, uint32_t func, uint32_t digit_in_en)
{
	if(func > 99)
	{
		uint8_t funmux = func - 100; func = 1;
		
		if(n < PIN6)
		{
			PORTx->FUNMUX0 &= ~(0x1F   << (n * 5));
			PORTx->FUNMUX0 |=  (funmux << (n * 5));
		}
		else if(n < PIN12)
		{
			PORTx->FUNMUX1 &= ~(0x1F   << ((n - 6) * 5));
			PORTx->FUNMUX1 |=  (funmux << ((n - 6) * 5));
		}
		else
		{
			PORTx->FUNMUX2 &= ~(0x1F   << ((n - 12) * 5));
			PORTx->FUNMUX2 |=  (funmux << ((n - 12) * 5));
		}
	}
	
	if(n < PIN8)
	{
		PORTx->FUNC0 &= ~(0x0F << (n*4));
		PORTx->FUNC0 |=  (func << (n*4));
	}
	else
	{
		PORTx->FUNC1 &= ~(0x0F << ((n-8)*4));
		PORTx->FUNC1 |=  (func << ((n-8)*4));
	}
	
	if(digit_in_en)  PORTx->INEN |=  (1 << n);
	else             PORTx->INEN &= ~(1 << n);
}
