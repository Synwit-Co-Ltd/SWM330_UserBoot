/*******************************************************************************************************************************
* @brief	I2S driver
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
#include "SWM330_i2s.h"


/*******************************************************************************************************************************
* @brief	I2S init
* @param	I2Sx is the I2S to init
* @param	initStruct is data used to init the I2S
* @return
*******************************************************************************************************************************/
void I2S_Init(I2S_TypeDef * I2Sx, I2S_InitStructure * initStruct)
{
	switch((uint32_t)I2Sx)
	{
	case ((uint32_t)I2S0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_I2S0_Pos);
		break;
	}
	
	I2S_Close(I2Sx);
	
	I2Sx->CR1 = (0 << I2S_CR1_NSYNC_Pos) |
				(0 << I2S_CR1_FILTE_Pos);
	
	I2Sx->CR2 = (initStruct->Mode						  << I2S_CR2_MODE_Pos)  |
				(initStruct->ClkPolarity				  << I2S_CR2_CKPOL_Pos) |
				((initStruct->FrameFormat & 0x03)  		  << I2S_CR2_FFMT_Pos)  |
				(initStruct->DataLen               		  << I2S_CR2_DLEN_Pos)  |
				(initStruct->ChannelLen 			  	  << I2S_CR2_CHLEN_Pos) |
				((initStruct->FrameFormat & 0x04 ? 1 : 0) << I2S_CR2_PCMSYNW_Pos);
	
	I2Sx->CR3 = (SystemCoreClock / initStruct->ClkFreq / 2 - 1) << I2S_CR3_SCLKDIV_Pos;
	
	I2S_INTClr(I2Sx, initStruct->INTEn);
	I2S_INTEn(I2Sx, initStruct->INTEn);
	
	if(initStruct->INTEn)
	{
		switch((uint32_t)I2Sx)
		{
		case ((uint32_t)I2S0):  NVIC_EnableIRQ(I2S0_IRQn);	break;
		}
	}
}

/*******************************************************************************************************************************
* @brief	I2S open
* @param	I2Sx is the I2S to open
* @return
*******************************************************************************************************************************/
void I2S_Open(I2S_TypeDef * I2Sx)
{
	I2Sx->CR2 |= I2S_CR2_EN_Msk;
}

/*******************************************************************************************************************************
* @brief	I2S close
* @param	I2Sx is the I2S to close
* @return
*******************************************************************************************************************************/
void I2S_Close(I2S_TypeDef * I2Sx)
{
	I2Sx->CR2 &= ~I2S_CR2_EN_Msk;
}


/*******************************************************************************************************************************
* @brief	I2S interrupt enable
* @param	I2Sx is the I2S to set
* @param	it is interrupt type, can be I2S_IT_SLV_TX_UNR, I2S_IT_RX_OVR, I2S_IT_RX_NOT_EMPTY, I2S_IT_TX_EMPTY and their '|' operation
* @return
*******************************************************************************************************************************/
void I2S_INTEn(I2S_TypeDef * I2Sx, uint32_t it)
{
	I2Sx->IE |= it;
}

/*******************************************************************************************************************************
* @brief	I2S interrupt disable
* @param	I2Sx is the I2S to set
* @param	it is interrupt type, can be I2S_IT_SLV_TX_UNR, I2S_IT_RX_OVR, I2S_IT_RX_NOT_EMPTY, I2S_IT_TX_EMPTY and their '|' operation
* @return
*******************************************************************************************************************************/
void I2S_INTDis(I2S_TypeDef * I2Sx, uint32_t it)
{
	I2Sx->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	I2S interrupt flag clear
* @param	I2Sx is the I2S to set
* @param	it is interrupt type, can be I2S_IT_SLV_TX_UNR, I2S_IT_RX_OVR, I2S_IT_RX_NOT_EMPTY, I2S_IT_TX_EMPTY and their '|' operation
* @return
*******************************************************************************************************************************/
void I2S_INTClr(I2S_TypeDef * I2Sx, uint32_t it)
{
	I2Sx->SR = it;
}

/*******************************************************************************************************************************
* @brief	I2S interrupt state query
* @param	I2Sx is the I2S to set
* @param	it is interrupt type, can be I2S_IT_SLV_TX_UNR, I2S_IT_RX_OVR, I2S_IT_RX_NOT_EMPTY, I2S_IT_TX_EMPTY and their '|' operation
* @return	0 interrupt not happen, !=0 interrupt happened
*******************************************************************************************************************************/
bool I2S_INTStat(I2S_TypeDef * I2Sx, uint32_t it)
{
	return I2Sx->SR & it;
}
