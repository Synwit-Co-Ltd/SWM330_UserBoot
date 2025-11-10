/*******************************************************************************************************************************
* @brief	LCD interface driver
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
#include "SWM330_lcd.h"

#include <string.h>


/*******************************************************************************************************************************
* @brief	LCD init
* @param	LCDx is the LCD to init
* @param	initStruct is data used to init the LCD
* @return
*******************************************************************************************************************************/
void LCD_Init(LCD_TypeDef * LCDx, LCD_InitStructure * initStruct)
{	
	switch((uint32_t)LCDx)
	{
	case ((uint32_t)LCD):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_LCD_Pos);
		break;
	}
	
	LCDx->CR  = ((initStruct->ClkDiv - 2)  << LCD_CR_CLKDIV_Pos)   |
				(initStruct->SampleEdge    << LCD_CR_CLKINV_Pos)   |
				(1                         << LCD_CR_CLKALW_Pos)   |
				((initStruct->Format & 1)  << LCD_CR_FORMAT_Pos)   |
			    ((initStruct->Format >> 1) << LCD_CR_MODE_Pos)     |
				(2						   << LCD_CR_PARTSIZE_Pos) |
				(3                         << LCD_CR_BURSTLEN_Pos) |
				((1-initStruct->IntEOTEn)  << LCD_CR_AUTORESTA_Pos);
	
	LCDx->CRH1 = ((initStruct->HnPixel - 1)    << LCD_CRH1_PIX_Pos);
	LCDx->CRH0 = ((initStruct->HsyncWidth - 1) << LCD_CRH0_HSW_Pos) |
				 ((initStruct->Hbp - 1)        << LCD_CRH0_HBP_Pos) |
				 ((initStruct->Hfp - 1)        << LCD_CRH0_HFP_Pos);
	
	LCDx->CRV1 = ((initStruct->VnPixel - 1)    << LCD_CRV1_PIX_Pos);
	LCDx->CRV0 = ((initStruct->VsyncWidth - 1) << LCD_CRV0_VSW_Pos) |
				 ((initStruct->Vbp - 1)        << LCD_CRV0_VBP_Pos) |
				 ((initStruct->Vfp - 1)        << LCD_CRV0_VFP_Pos);
	
	LCDx->BGC = initStruct->Background;
	
	LCDx->L[0].LCR = (1    << LCD_LCR_EN_Pos)   |
					 (0    << LCD_LCR_CKEN_Pos) |
					 (0xFF << LCD_LCR_ALPHA_Pos);
	
	LCDx->L[0].WHP = (0                         << LCD_WHP_STA_Pos) |
					 ((initStruct->HnPixel - 1) << LCD_WHP_STP_Pos);
	
	LCDx->L[0].WVP = (0                         << LCD_WVP_STA_Pos) |
					 ((initStruct->VnPixel - 1) << LCD_WVP_STP_Pos);
	
	LCDx->L[0].ADDR = initStruct->DataSource;
	
	LCDx->L[0].LLEN = (initStruct->HnPixel - 1);
	
	LCDx->IF = 1;	// clear interrupt flag
	LCDx->IE = initStruct->IntEOTEn;
	
	switch((uint32_t)LCDx)
	{
	case ((uint32_t)LCD):		
		if(initStruct->IntEOTEn) NVIC_EnableIRQ(LCD_IRQn);
		break;
	}
}

/*******************************************************************************************************************************
* @brief	LCD layer init
* @param	LCDx is the LCD to set
* @param	layerx is the LCD layer to init, can be LCD_LAYER_1
* @param	initStruct is data used to init the LCD layer
* @return
*******************************************************************************************************************************/
void LCD_LayerInit(LCD_TypeDef * LCDx, uint32_t layerx, LCD_LayerInitStructure * initStruct)
{
	LCDx->L[layerx].LCR = (1                 << LCD_LCR_EN_Pos)   |
					      (0                 << LCD_LCR_CKEN_Pos) |
					      (initStruct->Alpha << LCD_LCR_ALPHA_Pos);
	
	LCDx->L[layerx].WHP = (initStruct->HStart << LCD_WHP_STA_Pos) |
					      (initStruct->HStop  << LCD_WHP_STP_Pos);
	
	LCDx->L[layerx].WVP = (initStruct->VStart << LCD_WVP_STA_Pos) |
					      (initStruct->VStop  << LCD_WVP_STP_Pos);
	
	LCDx->L[layerx].ADDR = initStruct->DataSource;
	
	LCDx->L[layerx].LLEN = (initStruct->HStop - initStruct->HStart);
	
	LCD->CR |= (1 << LCD_CR_VBPRELOAD_Pos);
}

/*******************************************************************************************************************************
* @brief	LCD layer position set
* @param	LCDx is the LCD to set
* @param	layerx is the LCD layer to set, can be LCD_LAYER_1
* @param	hstart is horizontal starting position
* @param	hstop is horizontal termination position (included)
* @param	vstart is vertical starting position
* @param	vstop is vertical termination position (included)
* @return
*******************************************************************************************************************************/
void LCD_SetLayerPos(LCD_TypeDef * LCDx, uint32_t layerx, uint16_t hstart, uint16_t hstop, uint16_t vstart, uint16_t vstop)
{
	LCDx->L[layerx].WHP = (hstart << LCD_WHP_STA_Pos) |
					      (hstop  << LCD_WHP_STP_Pos);
	
	LCDx->L[layerx].WVP = (vstart << LCD_WVP_STA_Pos) |
					      (vstop  << LCD_WVP_STP_Pos);
	
	LCD->CR |= (1 << LCD_CR_VBPRELOAD_Pos);
}

/*******************************************************************************************************************************
* @brief	LCD start
* @param	LCDx is the LCD to start
* @return
*******************************************************************************************************************************/
void LCD_Start(LCD_TypeDef * LCDx)
{
	LCDx->START |= (1 << LCD_START_GO_Pos);
}

static uint32_t LCD_AutoRestart = 0;
/*******************************************************************************************************************************
* @brief	LCD stop
* @param	LCDx is the LCD to stop
* @return
*******************************************************************************************************************************/
void LCD_Stop(LCD_TypeDef * LCDx)
{
	LCD_AutoRestart = LCDx->CR & LCD_CR_AUTORESTA_Msk;
	
	if(LCD_AutoRestart)
		LCDx->CR &= ~LCD_CR_AUTORESTA_Msk;
	else
		NVIC_DisableIRQ(LCD_IRQn);
}

/*******************************************************************************************************************************
* @brief	LCD restart
* @param	LCDx is the LCD to restart
* @return
* @note		can only call once after LCD_Stop() call.
*******************************************************************************************************************************/
void LCD_ReStart(LCD_TypeDef * LCDx)
{
	if(LCD_AutoRestart)
	{
		LCD_Start(LCDx);
		
		LCDx->CR |=  LCD_CR_AUTORESTA_Msk;
	}
	else
		NVIC_EnableIRQ(LCD_IRQn);
}

/*******************************************************************************************************************************
* @brief	LCD busy query
* @param	LCDx is the LCD to query
* @return	1 LCD busy, 0 LCD idle
*******************************************************************************************************************************/
uint32_t LCD_IsBusy(LCD_TypeDef * LCDx)
{
	return (LCDx->START & LCD_START_GO_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	LCD interrupt enable
* @param	LCDx is the LCD to set
* @param	it is interrupt type, can be LCD_IT_DONE, LCD_IT_PART and their '|' operation
* @return
*******************************************************************************************************************************/
void LCD_INTEn(LCD_TypeDef * LCDx, uint32_t it)
{
	LCDx->IE |= it;
}

/*******************************************************************************************************************************
* @brief	LCD interrupt disable
* @param	LCDx is the LCD to set
* @param	it is interrupt type, can be LCD_IT_DONE, LCD_IT_PART and their '|' operation
* @return
*******************************************************************************************************************************/
void LCD_INTDis(LCD_TypeDef * LCDx, uint32_t it)
{
	LCDx->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	LCD interrupt flag clear
* @param	LCDx is the LCD to clear
* @param	it is interrupt type, can be LCD_IT_DONE, LCD_IT_PART and their '|' operation
* @return
*******************************************************************************************************************************/
void LCD_INTClr(LCD_TypeDef * LCDx, uint32_t it)
{
	LCDx->IF = it;
}

/*******************************************************************************************************************************
* @brief	LCD interrupt state query
* @param	LCDx is the LCD to query

* @return	0 interrupt not happen, !=0 interrupt happened
*******************************************************************************************************************************/
uint32_t LCD_INTStat(LCD_TypeDef * LCDx, uint32_t it)
{
	return (LCDx->IF & it);
}


/*******************************************************************************************************************************
* @brief	LCD init, LCD working in MPU mode
* @param	LCDx is the LCD to init
* @param	initStruct is data used to init the LCD
* @return
*******************************************************************************************************************************/
void MPULCD_Init(LCD_TypeDef * LCDx, MPULCD_InitStructure * initStruct)
{
	switch((uint32_t)LCDx)
	{
	case ((uint32_t)LCD):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_LCD_Pos);
		__NOP();__NOP();__NOP();
		break;
	}
	
	LCDx->CR  = (LCD_FMT_RGB565				 << LCD_CR_FORMAT_Pos)  |	// in MPU mode, only support RGB565
				((initStruct->BusWidth  & 3) << LCD_CR_MODE_Pos)    |
				((initStruct->BusWidth >> 2) << LCD_CR_MPU8PIN_Pos) |
				(initStruct->ByteOrder		 << LCD_CR_MPU8MSB_Pos) |
				(0							 << LCD_CR_MPUDMAMSB_Pos);	
	
	LCDx->L[0].LCR |= (1 << LCD_LCR_EN_Pos);
	
	LCDx->MPUCR = ((initStruct->RDHoldTime    - 1) << LCD_MPUCR_RDHOLD_Pos) |
				  ((initStruct->WRHoldTime    - 1) << LCD_MPUCR_WRHOLD_Pos) |
				  ((initStruct->CSFall_WRFall - 1) << LCD_MPUCR_CS0WR0_Pos) |
				  ((initStruct->WRRise_CSRise - 1) << LCD_MPUCR_WR1CS1_Pos) |
				  ((initStruct->RDCSRise_Fall - 1) << LCD_MPUCR_RCS1_0_Pos) |
				  ((initStruct->WRCSRise_Fall - 1) << LCD_MPUCR_WCS1_0_Pos);
}


void MPULCD_WR_REG8(LCD_TypeDef * LCDx, uint8_t reg)
{
	LCDx->MPUIRB = reg;
	__NOP(); __NOP(); __NOP();
	while(LCD_IsBusy(LCDx)) __NOP();
}

void MPULCD_WR_DATA8(LCD_TypeDef * LCDx, uint8_t val)
{
	LCDx->MPUDRB = val;
	__NOP(); __NOP(); __NOP();
	while(LCD_IsBusy(LCDx)) __NOP();
}

void MPULCD_WR_REG16(LCD_TypeDef * LCDx, uint16_t reg)
{
	LCDx->MPUIRH = reg;
	__NOP(); __NOP(); __NOP();
	while(LCD_IsBusy(LCDx)) __NOP();
}

void MPULCD_WR_DATA16(LCD_TypeDef * LCDx, uint16_t val)
{
	LCDx->MPUDRH = val;
	__NOP(); __NOP(); __NOP();
	while(LCD_IsBusy(LCDx)) __NOP();
}

void MPULCD_WriteReg8(LCD_TypeDef * LCDx, uint8_t reg, uint8_t val)
{
	MPULCD_WR_REG8(LCDx, reg);
	
	MPULCD_WR_DATA8(LCDx, val);
}

uint8_t MPULCD_ReadReg8(LCD_TypeDef * LCDx, uint8_t reg)
{
	MPULCD_WR_REG8(LCDx, reg);
	
	return LCDx->MPUDRB;
}

void MPULCD_WriteReg16(LCD_TypeDef * LCDx, uint16_t reg, uint16_t val)
{
	MPULCD_WR_REG16(LCDx, reg);
	
	MPULCD_WR_DATA16(LCDx, val);
}

uint16_t MPULCD_ReadReg16(LCD_TypeDef * LCDx, uint16_t reg)
{
	MPULCD_WR_REG16(LCDx, reg);
	
	return LCDx->MPUDRH;
}


/*******************************************************************************************************************************
* @brief	Transfer display data to MPU screen by DMA
* @param	LCDx is the LCD to use
* @param	buff is data to send to MPU screen
* @param	hpix is number of pixels per line, must be even
* @param	vpix is number of lines per screen
* @return
*******************************************************************************************************************************/
void MPULCD_DMAStart(LCD_TypeDef * LCDx, uint32_t * buff, uint16_t hpix, uint16_t vpix)
{
	LCDx->L[0].ADDR = (uint32_t)buff;
	
	LCDx->L[0].LLEN = hpix - 1;
	
	LCDx->L[0].WHP = (0			 << LCD_WHP_STA_Pos) |
				     ((hpix - 1) << LCD_WHP_STP_Pos);
	
	LCDx->L[0].WVP = (0			 << LCD_WVP_STA_Pos) |
				     ((vpix - 1) << LCD_WVP_STP_Pos);
	
	LCDx->START |= (1 << LCD_START_GO_Pos);
}

/*******************************************************************************************************************************
* @brief	Data transfer busy query
* @param	LCDx is the LCD to query
* @return	1 transferring, 0 transfer done
*******************************************************************************************************************************/
uint32_t MPULCD_DMABusy(LCD_TypeDef * LCDx)
{
	return (LCDx->START & LCD_START_GO_Msk) ? 1 : 0;
}
