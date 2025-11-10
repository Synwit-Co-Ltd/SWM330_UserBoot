/*******************************************************************************************************************************
* @brief	DVP driver
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
#include "SWM330_dvp.h"


/*******************************************************************************************************************************
* @brief	DVP init
* @param	DVPx is the DVP to init
* @param	initStruct is data used to init the DVP
* @return
*******************************************************************************************************************************/
void DVP_Init(DVP_TypeDef * DVPx, DVP_InitStructure * initStruct)
{
	uint8_t clk_per_pixel  = (initStruct->InFormat & 0xF) == 0 ? 1 : 2;
	uint8_t byte_per_pixel =  initStruct->InFormat		  == 0 ? 1 : 2;
	
	SYS->CLKEN0 |= SYS_CLKEN0_DVP_Msk;
	
	DVPx->CR = (1					  << DVP_CR_DROPEN_Pos) |
			   (0					  << DVP_CR_CONTEN_Pos) |
			   (initStruct->InFormat  << DVP_CR_INFMT_Pos)  |
			   (initStruct->OutFormat << DVP_CR_OUTFMT_Pos);
	
	DVPx->INPOL = (initStruct->SampleEdge << DVP_INPOL_PCLK_Pos)  |
				  (0					  << DVP_INPOL_HREF_Pos)  |
				  (0					  << DVP_INPOL_VSYNC_Pos) |
				  (0					  << DVP_INPOL_FIELD_Pos);
	
	DVPx->OUTHSZ = ((initStruct->StartPixel - 1) * clk_per_pixel << DVP_OUTHSZ_BEGIN_Pos) |
				   ((initStruct->PixelCount * clk_per_pixel - 1) << DVP_OUTHSZ_COUNT_Pos);
	
	DVPx->OUTVSZ = ((initStruct->StartLine  - 1) << DVP_OUTVSZ_BEGIN_Pos) |
				   ((initStruct->LineCount  - 1) << DVP_OUTVSZ_COUNT_Pos);
	
	if((initStruct->OutFormat == DVP_OUTFMT_RAW) ||
	   (initStruct->OutFormat == DVP_OUTFMT_RGB565))
	{
		DVPx->BASE_YRAW = initStruct->RGBAddr;
		
		DVPx->STEP_YRAW = initStruct->LineStride * byte_per_pixel;
	}
	else
	{
		DVPx->BASE_YRAW = initStruct->YAddr;
		DVPx->BASE_UV   = initStruct->UVAddr;
		
		DVPx->STEP_YRAW = initStruct->LineStride;
		
		if(initStruct->OutFormat == DVP_OUTFMT_YUV422)
			DVPx->STEP_UV   = initStruct->LineStride;
		else
			DVPx->STEP_UV   = initStruct->LineStride / 2;
	}
	
	DVPx->UPDATE = 1;
	
	DVPx->CR |= DVP_CR_ENA_Msk;
	
	DVPx->ICFG = (initStruct->LineCount / 2 << DVP_ICFG_LINES_Pos);
	
	DVP_INTClr(DVPx, initStruct->IntEn);
	DVP_INTEn(DVPx, initStruct->IntEn);
	
	if(initStruct->IntEn)
		NVIC_EnableIRQ(DVP_IRQn);
}


/*******************************************************************************************************************************
* @brief	DVP capture start
* @param	DVPx is the DVP to start
* @param	start_frame is the start frame to be captured, start from 1
* @param	frame_count is the number of frame to capture
* @return
*******************************************************************************************************************************/
void DVP_Start(DVP_TypeDef * DVPx, uint16_t start_frame, uint16_t frame_count)
{
	DVPx->OUTCFG = (0				  << DVP_OUTCFG_FIELD_Pos) |
				   ((start_frame - 1) << DVP_OUTCFG_FRAME_Pos) |
				   ((frame_count - 1) << DVP_OUTCFG_COUNT_Pos);
	
	DVPx->OUTCR = DVP_OUTCR_CAPON_Msk;
}


/*******************************************************************************************************************************
* @brief	DVP capture stop
* @param	DVPx is the DVP to stop
* @param	immediate = 1: stop capture immediately, 0: stop capture after current frame
* @return
*******************************************************************************************************************************/
void DVP_Stop(DVP_TypeDef * DVPx, uint32_t immediate)
{
	DVPx->OUTCR = (0		 << DVP_OUTCR_CAPON_Pos) |
				  (immediate << DVP_OUTCR_OFFMODE_Pos);
}

/*******************************************************************************************************************************
* @brief	DVP busy query
* @param	DVPx is the DVP to query
* @return	true: DVP is doing frame capture, false: capture done, DVP is idle
*******************************************************************************************************************************/
bool DVP_Busy(DVP_TypeDef * DVPx)
{
	return DVPx->OUTCR & DVP_OUTCR_CAPON_Msk;
}


/*******************************************************************************************************************************
* @brief	DVP interrupt enable
* @param	DVPx is the DVP to set
* @param	it is interrupt type, can be DVP_IT_DONE, DVP_IT_FRAME, ..., DVP_IT_UPDATE and their '|' operation
* @return
*******************************************************************************************************************************/
void DVP_INTEn(DVP_TypeDef * DVPx, uint32_t it)
{
	DVPx->IE |= it;
}

/*******************************************************************************************************************************
* @brief	DVP interrupt disable
* @param	DVPx is the DVP to set
* @param	it is interrupt type, can be DVP_IT_DONE, DVP_IT_FRAME, ..., DVP_IT_UPDATE and their '|' operation
* @return
*******************************************************************************************************************************/
void DVP_INTDis(DVP_TypeDef * DVPx, uint32_t it)
{
	DVPx->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	DVP interrupt flag clear
* @param	DVPx is the DVP to set
* @param	it is interrupt type, can be DVP_IT_DONE, DVP_IT_FRAME, ..., DVP_IT_UPDATE and their '|' operation
* @return
*******************************************************************************************************************************/
void DVP_INTClr(DVP_TypeDef * DVPx, uint32_t it)
{
	DVPx->IF = it;
}

/*******************************************************************************************************************************
* @brief	DVP interrupt state query
* @param	DVPx is the DVP to query
* @param	it is interrupt type, can be DVP_IT_DONE, DVP_IT_FRAME, ..., DVP_IT_UPDATE and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t DVP_INTStat(DVP_TypeDef * DVPx, uint32_t it)
{
	return DVPx->IF & it ? 1 : 0;
}
