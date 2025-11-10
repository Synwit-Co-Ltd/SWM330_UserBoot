/*******************************************************************************************************************************
* @brief	DMA2D driver
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
#include "SWM330_dma2d.h"


/*******************************************************************************************************************************
* @brief	DMA2D init
* @param	initStruct is data used to init the DMA2D
* @return
*******************************************************************************************************************************/
void DMA2D_Init(DMA2D_InitStructure * initStruct)
{
	SYS->CLKEN0 |= (1 << SYS_CLKEN0_DMA2D_Pos);
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	DMA2D->CR = (initStruct->BurstSize << DMA2D_CR_BURST_Pos) |
				(initStruct->BlockSize << DMA2D_CR_BLKSZ_Pos) |
				(initStruct->Interval  << DMA2D_CR_WAIT_Pos);
	
	DMA2D_INTClr(initStruct->IntEn);
	DMA2D_INTEn(initStruct->IntEn);
	
	if(initStruct->IntEn)
		NVIC_EnableIRQ(DMA2D_IRQn);
}

/*******************************************************************************************************************************
* @brief	DMA2D fill specified memory area with specified color
* @param	outLayer specify the memory area to fill
* @param	color is the color to fill
* @return
*******************************************************************************************************************************/
void DMA2D_PixelFill(DMA2D_LayerSetting * outLayer, uint32_t color)
{
	DMA2D->L[DMA2D_LAYER_OUT].COLOR = color;
	
	DMA2D->L[DMA2D_LAYER_OUT].MAR = outLayer->Address;
	DMA2D->L[DMA2D_LAYER_OUT].OR  = outLayer->LineOffset;
	DMA2D->L[DMA2D_LAYER_OUT].PFCCR = (outLayer->ColorMode << DMA2D_PFCCR_CFMT_Pos);
	
	DMA2D->NLR = ((outLayer->LineCount - 1) << DMA2D_NLR_NLINE_Pos) |
				 ((outLayer->LinePixel - 1) << DMA2D_NLR_NPIXEL_Pos);
	
	DMA2D->CR &= ~(DMA2D_CR_MODE_Msk | DMA2D_CR_GPDMA_Msk);
	DMA2D->CR |= (3 << DMA2D_CR_MODE_Pos) |
				 (1 << DMA2D_CR_START_Pos);
}

/*******************************************************************************************************************************
* @brief	DMA2D move pixels in specified memory area to another specified memory area
* @param	fgLayer specify the source memory area
* @param	outLayer specify the destination memory area
* @return
*******************************************************************************************************************************/
void DMA2D_PixelMove(DMA2D_LayerSetting * fgLayer, DMA2D_LayerSetting * outLayer)
{
	DMA2D->L[DMA2D_LAYER_FG].MAR = fgLayer->Address;
	DMA2D->L[DMA2D_LAYER_FG].OR  = fgLayer->LineOffset;
	DMA2D->L[DMA2D_LAYER_FG].PFCCR = (fgLayer->ColorMode << DMA2D_PFCCR_CFMT_Pos);
	
	DMA2D->L[DMA2D_LAYER_OUT].MAR = outLayer->Address;
	DMA2D->L[DMA2D_LAYER_OUT].OR  = outLayer->LineOffset;
	
	DMA2D->NLR = ((outLayer->LineCount - 1) << DMA2D_NLR_NLINE_Pos) |
				 ((outLayer->LinePixel - 1) << DMA2D_NLR_NPIXEL_Pos);
	
	DMA2D->CR &= ~(DMA2D_CR_MODE_Msk | DMA2D_CR_GPDMA_Msk);
	DMA2D->CR |= (0 << DMA2D_CR_MODE_Pos) |
				 (1 << DMA2D_CR_START_Pos);
}

/*******************************************************************************************************************************
* @brief	DMA2D move pixels in specified memory area to another specified memory area, and meanwhile convert pixel format
* @param	fgLayer specify the source memory area
* @param	outLayer specify the destination memory area
* @return
*******************************************************************************************************************************/
void DMA2D_PixelConvert(DMA2D_LayerSetting * fgLayer, DMA2D_LayerSetting * outLayer)
{
	DMA2D->L[DMA2D_LAYER_FG].MAR = fgLayer->Address;
	DMA2D->L[DMA2D_LAYER_FG].OR  = fgLayer->LineOffset;
	DMA2D->L[DMA2D_LAYER_FG].PFCCR = (fgLayer->ColorMode << DMA2D_PFCCR_CFMT_Pos);
	
	DMA2D->L[DMA2D_LAYER_OUT].MAR = outLayer->Address;
	DMA2D->L[DMA2D_LAYER_OUT].OR  = outLayer->LineOffset;
	DMA2D->L[DMA2D_LAYER_OUT].PFCCR = (outLayer->ColorMode << DMA2D_PFCCR_CFMT_Pos);
	
	DMA2D->NLR = ((outLayer->LineCount - 1) << DMA2D_NLR_NLINE_Pos) |
				 ((outLayer->LinePixel - 1) << DMA2D_NLR_NPIXEL_Pos);
	
	DMA2D->CR &= ~(DMA2D_CR_MODE_Msk | DMA2D_CR_GPDMA_Msk);
	DMA2D->CR |= (1 << DMA2D_CR_MODE_Pos) |
				 (1 << DMA2D_CR_START_Pos);
}

/*******************************************************************************************************************************
* @brief	DMA2D blend pixels from fgLayer and bgLayer to outLayer
* @param	fgLayer specify the first source memory area
* @param	bgLayer specify the second source memory area
* @param	outLayer specify the destination memory area
* @return
*******************************************************************************************************************************/
void DMA2D_PixelBlend(DMA2D_LayerSetting * fgLayer, DMA2D_LayerSetting * bgLayer, DMA2D_LayerSetting * outLayer)
{
	DMA2D->L[DMA2D_LAYER_FG].MAR = fgLayer->Address;
	DMA2D->L[DMA2D_LAYER_FG].OR  = fgLayer->LineOffset;
	DMA2D->L[DMA2D_LAYER_FG].PFCCR = (fgLayer->ColorMode << DMA2D_PFCCR_CFMT_Pos) |
									 (fgLayer->AlphaMode << DMA2D_PFCCR_AINV_Pos) |
									 (fgLayer->Alpha     << DMA2D_PFCCR_ALPHA_Pos);
	
	DMA2D->CR &= ~DMA2D_CR_AAREN_Msk;
	if(fgLayer->AlphaMode == DMA2D_AMODE_EXTERN)
	{
		DMA2D->CR |= DMA2D_CR_AAREN_Msk;
		DMA2D->AAR = fgLayer->AlhpaAddr;
	}
	
	DMA2D->L[DMA2D_LAYER_BG].MAR = bgLayer->Address;
	DMA2D->L[DMA2D_LAYER_BG].OR  = bgLayer->LineOffset;
	DMA2D->L[DMA2D_LAYER_BG].PFCCR = (bgLayer->ColorMode << DMA2D_PFCCR_CFMT_Pos) |
									 (bgLayer->AlphaMode << DMA2D_PFCCR_AINV_Pos) |
									 (bgLayer->Alpha     << DMA2D_PFCCR_ALPHA_Pos);
	
	DMA2D->L[DMA2D_LAYER_OUT].MAR = outLayer->Address;
	DMA2D->L[DMA2D_LAYER_OUT].OR  = outLayer->LineOffset;
	DMA2D->L[DMA2D_LAYER_OUT].PFCCR = (outLayer->ColorMode << DMA2D_PFCCR_CFMT_Pos);
	
	DMA2D->NLR = ((outLayer->LineCount - 1) << DMA2D_NLR_NLINE_Pos) |
				 ((outLayer->LinePixel - 1) << DMA2D_NLR_NPIXEL_Pos);
	
	DMA2D->CR &= ~(DMA2D_CR_MODE_Msk | DMA2D_CR_GPDMA_Msk);
	DMA2D->CR |= (2 << DMA2D_CR_MODE_Pos) |
				 (1 << DMA2D_CR_START_Pos);
}

/*******************************************************************************************************************************
* @brief	DMA2D busy query
* @param
* @return	1 DMA2D is busy, 0 DMA2D is dile, operation done
*******************************************************************************************************************************/
uint32_t DMA2D_IsBusy(void)
{
	return (DMA2D->CR & DMA2D_CR_START_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	DMA2D memcpy
* @param	destin is destination address, must be half-word/word aligned when unit_size == DMA2D_UNIT_HALF/DMA2D_UNIT_WORD
* @param	source is source address, must be half-word/word aligned when unit_size == DMA2D_UNIT_HALF/DMA2D_UNIT_WORD
* @param	unit_size: DMA2D_UNIT_BYTE, DMA2D_UNIT_HALF, DMA2D_UNIT_WORD
* @param	unit_count is unit count to transfer from src_addr to dest_addr
* @return
*******************************************************************************************************************************/
void DMA2D_memcpy(void * destin, const void * source, uint8_t unit_size, uint16_t unit_count)
{
	DMA2D->L[DMA2D_LAYER_FG].MAR = (uint32_t)source;
	DMA2D->L[DMA2D_LAYER_FG].PFCCR = unit_size;
	
	DMA2D->L[DMA2D_LAYER_OUT].MAR = (uint32_t)destin;
	DMA2D->L[DMA2D_LAYER_OUT].PFCCR = unit_size;
	
	DMA2D->NLR = ((unit_count - 1) << 0) | (8 << 23);
	
	DMA2D->CR &= ~(DMA2D_CR_MODE_Msk | DMA2D_CR_GPDMA_Msk);
	DMA2D->CR |= (1 << DMA2D_CR_GPDMA_Pos) |
				 (1 << DMA2D_CR_START_Pos);
}

/*******************************************************************************************************************************
* @brief	DMA2D interrupt enable
* @param	it is interrupt type, can be DMA2D_IT_DONE, DMA2D_IT_HALF, DMA2D_IT_ERROR and their '|' operation
* @return
*******************************************************************************************************************************/
void DMA2D_INTEn(uint32_t it)
{
	DMA2D->IE |= it;
}

/*******************************************************************************************************************************
* @brief	DMA2D interrupt disable
* @param	it is interrupt type, can be DMA2D_IT_DONE, DMA2D_IT_HALF, DMA2D_IT_ERROR and their '|' operation
* @return
*******************************************************************************************************************************/
void DMA2D_INTDis(uint32_t it)
{
	DMA2D->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	DMA2D interrupt flag clear
* @param	it is interrupt type, can be DMA2D_IT_DONE, DMA2D_IT_HALF, DMA2D_IT_ERROR and their '|' operation
* @return
*******************************************************************************************************************************/
void DMA2D_INTClr(uint32_t it)
{
	DMA2D->IF = it;
}

/*******************************************************************************************************************************
* @brief	DMA2D interrupt state query
* @param	it is interrupt type, can be DMA2D_IT_DONE, DMA2D_IT_HALF, DMA2D_IT_ERROR and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t DMA2D_INTStat(uint32_t it)
{
	return (DMA2D->IF & it) ? 1 : 0;
}
