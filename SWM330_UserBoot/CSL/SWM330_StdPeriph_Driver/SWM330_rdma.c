/*******************************************************************************************************************************
* @brief	Rapid DMA driver
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
#include "SWM330_rdma.h"


/*******************************************************************************************************************************
* @brief	RDMA init
* @param	initStruct is data used to init the RDMA
* @return
*******************************************************************************************************************************/
void RDMA_Init(RDMA_InitStructure * initStruct)
{
	SYS->CLKEN0 |= SYS_CLKEN0_RDMA_Msk;
	__ISB(); __NOP(); __NOP(); __NOP();
	
	RDMA->CR = (initStruct->BurstSize << RDMA_CR_BURST_Pos) |
			   (initStruct->BlockSize << RDMA_CR_BLKSZ_Pos) |
			   (initStruct->Interval  << RDMA_CR_WAIT_Pos);
	
	RDMA_INTClr(initStruct->INTEn);
	RDMA_INTEn(initStruct->INTEn);
	
	if(initStruct->INTEn) NVIC_EnableIRQ(RDMA_IRQn);
}

/*******************************************************************************************************************************
* @brief	RDMA memcpy
* @param	destin is destination address, must be half-word/word aligned when unit_size == DMA2D_UNIT_HALF/DMA2D_UNIT_WORD
* @param	source is source address, must be half-word/word aligned when unit_size == DMA2D_UNIT_HALF/DMA2D_UNIT_WORD
* @param	unit_size: RDMA_UNIT_BYTE, RDMA_UNIT_HALFWORD, RDMA_UNIT_WORD
* @param	unit_count is unit count to transfer from source to destin
* @return
*******************************************************************************************************************************/
void RDMA_memcpy(void * destin, const void * source, uint8_t unit_size, uint32_t unit_count)
{
	RDMA->SRC = (uint32_t)source;
	RDMA->DES = (uint32_t)destin;
	RDMA->LEN = ((unit_count-1) << RDMA_LEN_SIZE_Pos) |
				(8				<< RDMA_LEN_PART_Pos);		// half
	
	uint8_t rd_hs_en = 0, wr_hs_en = 0, hs_sel = 0;
	if(((uint32_t)source == (uint32_t)&QSPI0->DRW) || ((uint32_t)source == (uint32_t)&QSPI1->DRW))
		rd_hs_en = 1;
	if(((uint32_t)destin == (uint32_t)&QSPI0->DRW) || ((uint32_t)destin == (uint32_t)&QSPI1->DRW))
		wr_hs_en = 1;
	if(((uint32_t)source == (uint32_t)&QSPI1->DRW) || ((uint32_t)destin == (uint32_t)&QSPI1->DRW))
		hs_sel = 1;
	
	RDMA->CR &= ~(RDMA_CR_RSIZE_Msk | RDMA_CR_WSIZE_Msk);
	RDMA->CR |= (unit_size << RDMA_CR_RSIZE_Pos) |
				(unit_size << RDMA_CR_WSIZE_Pos) | 
				(rd_hs_en  << RDMA_CR_RHSEN_Pos) |
				(wr_hs_en  << RDMA_CR_WHSEN_Pos) |
				(hs_sel	   << RDMA_CR_HSSEL_Pos) | RDMA_CR_START_Msk;
}

/*******************************************************************************************************************************
* @brief	RDMA interrupt enable
* @param	it is interrupt type, can be RDMA_IT_DONE, RDMA_IT_HALF, RDMA_IT_ERROR and their '|' operation
* @return
*******************************************************************************************************************************/
void RDMA_INTEn(uint32_t it)
{
	RDMA->IE |= it;
}

/*******************************************************************************************************************************
* @brief	RDMA interrupt disable
* @param	it is interrupt type, can be RDMA_IT_DONE, RDMA_IT_HALF, RDMA_IT_ERROR and their '|' operation
* @return
*******************************************************************************************************************************/
void RDMA_INTDis(uint32_t it)
{
	RDMA->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	RDMA interrupt flag clear
* @param	it is interrupt type, can be RDMA_IT_DONE, RDMA_IT_HALF, RDMA_IT_ERROR and their '|' operation
* @return
*******************************************************************************************************************************/
void RDMA_INTClr(uint32_t it)
{
	RDMA->IF = it;
}

/*******************************************************************************************************************************
* @brief	RDMA interrupt state query
* @param	it is interrupt type, can be RDMA_IT_DONE, RDMA_IT_HALF, RDMA_IT_ERROR and their '|' operation
* @return	!=0 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t RDMA_INTStat(uint32_t it)
{
	return RDMA->IF & it;
}
