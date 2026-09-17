/*******************************************************************************************************************************
* @brief	DAC driver
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
#include "SWM330_dac.h"


/*******************************************************************************************************************************
* @brief	DAC init
* @param	DACx is the DAC to init
* @param	initStruct is data used to init the DAC
* @return
*******************************************************************************************************************************/
void DAC_Init(DAC_TypeDef * DACx, DAC_InitStructure * initStruct)
{
	switch((uint32_t)DACx)
	{
	case ((uint32_t)DAC0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_DAC0_Pos);
		break;
	}
	
	DACx->CR = (initStruct->Format	  << DAC_CR_DHRFMT_Pos) |
			   (initStruct->OutBuffer << DAC_CR_BUFOFF_Pos) |
			   (initStruct->Vref	  << DAC_CR_REFSEL_Pos);
}

/*******************************************************************************************************************************
* @brief	DAC open
* @param	DACx is the DAC to open
* @return
*******************************************************************************************************************************/
void DAC_Open(DAC_TypeDef * DACx)
{
	DACx->CR |= (1 << DAC_CR_EN_Pos);
}

/*******************************************************************************************************************************
* @brief	DAC close
* @param	DACx is the DAC to close
* @return
*******************************************************************************************************************************/
void DAC_Close(DAC_TypeDef * DACx)
{
	DACx->CR &= ~DAC_CR_EN_Msk;
}
