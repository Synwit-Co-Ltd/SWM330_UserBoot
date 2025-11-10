/*******************************************************************************************************************************
* @brief	QSPI driver
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
#include "SWM330_qspi.h"


static uint8_t AddressSize;


/*******************************************************************************************************************************
* @brief	QSPI init
* @param	QSPIx is the QSPI to init
* @param	initStruct is data used to init the QSPI
* @return
*******************************************************************************************************************************/
void QSPI_Init(QSPI_TypeDef * QSPIx, QSPI_InitStructure * initStruct)
{
	switch((uint32_t)QSPIx)
	{
	case ((uint32_t)QSPI0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_QSPI0_Pos);
		break;
	
	case ((uint32_t)QSPI1):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_QSPI1_Pos);
		break;
	}
	
	QSPI_Close(QSPIx);
	
	QSPIx->CR = (1						<< QSPI_CR_TOEN_Pos)   |
				(0						<< QSPI_CR_SSHIFT_Pos) |
				(0						<< QSPI_CR_DUAL_Pos)   |
				(8						<< QSPI_CR_FFTHR_Pos)  |
				(initStruct->IntEn		<< QSPI_CR_ERRIE_Pos)  |
				((initStruct->ClkDiv-1)	<< QSPI_CR_CLKDIV_Pos);
	
	QSPIx->DCR = (initStruct->ClkMode 	<< QSPI_DCR_CLKMOD_Pos) |
				 (3						<< QSPI_DCR_CSHIGH_Pos) |
				 (initStruct->Size		<< QSPI_DCR_FLSIZE_Pos);
	
	AddressSize = initStruct->Size / 8;
	
	QSPIx->SSHIFT = initStruct->SampleShift & 0x0F;
	
	QSPIx->CS0TO = 100;
	
	QSPIx->FCR = 0x1B;
	if(initStruct->IntEn)
	{
		switch((uint32_t)QSPIx)
		{
		case ((uint32_t)QSPI0):	NVIC_EnableIRQ(QSPI0_IRQn); break;
		case ((uint32_t)QSPI1):	NVIC_EnableIRQ(QSPI1_IRQn); break;
		}
	}
}


/*******************************************************************************************************************************
* @brief	QSPI open
* @param	QSPIx is the QSPI to open
* @return
*******************************************************************************************************************************/
void QSPI_Open(QSPI_TypeDef * QSPIx)
{
	QSPIx->CR |= QSPI_CR_EN_Msk;
}


/*******************************************************************************************************************************
* @brief	QSPI close
* @param	QSPIx is the QSPI to close
* @param	initStruct is data used to init the QSPI
* @return
*******************************************************************************************************************************/
void QSPI_Close(QSPI_TypeDef * QSPIx)
{
	QSPIx->CR &= ~QSPI_CR_EN_Msk;
}


/*******************************************************************************************************************************
* @brief	QSPI_CmdStructure instance clear
* @param	cmdStruct is the QSPI_CmdStructure instance to clear
* @return
*******************************************************************************************************************************/
void QSPI_CmdStructClear(QSPI_CmdStructure * cmdStruct)
{
	cmdStruct->Instruction		  = 0;
	cmdStruct->InstructionMode	  = 0;
	cmdStruct->Address 			  = 0;
	cmdStruct->AddressMode		  = 0;
	cmdStruct->AddressSize		  = 0;
	cmdStruct->AlternateBytes 	  = 0;
	cmdStruct->AlternateBytesMode = 0;
	cmdStruct->AlternateBytesSize = 0;
	cmdStruct->DummyCycles		  = 0;
	cmdStruct->DataMode			  = 0;
	cmdStruct->DataCount		  = 0;
	cmdStruct->SendInstOnlyOnce   = 0;
}


/*******************************************************************************************************************************
* @brief	QSPI send command
* @param	QSPIx is the QSPI to use
* @param	cmdMode: QSPI_Mode_IndirectWrite, QSPI_Mode_IndirectRead, QSPI_Mode_AutoPolling, or QSPI_Mode_MemoryMapped
* @param	cmdStruct contains the command to be sent out
* @return
*******************************************************************************************************************************/
void QSPI_Command(QSPI_TypeDef * QSPIx, uint8_t cmdMode, QSPI_CmdStructure * cmdStruct)
{
	if(cmdStruct->AlternateBytesMode != QSPI_PhaseMode_None)
		QSPIx->ABR = cmdStruct->AlternateBytes;
	
	if(cmdStruct->DataMode != QSPI_PhaseMode_None)
		QSPIx->DLR = cmdStruct->DataCount - 1;
	
	QSPIx->CCR = (cmdStruct->Instruction		<< QSPI_CCR_CODE_Pos)   |
				 (cmdStruct->InstructionMode	<< QSPI_CCR_IMODE_Pos)  |
				 (cmdStruct->AddressMode		<< QSPI_CCR_AMODE_Pos)  |
				 (cmdStruct->AddressSize		<< QSPI_CCR_ASIZE_Pos)  |
				 (cmdStruct->AlternateBytesMode	<< QSPI_CCR_ABMODE_Pos) |
				 (cmdStruct->AlternateBytesSize	<< QSPI_CCR_ABSIZE_Pos) |
				 (cmdStruct->DummyCycles		<< QSPI_CCR_DUMMY_Pos)  |
				 (cmdStruct->DataMode			<< QSPI_CCR_DMODE_Pos)  |
				 (cmdMode						<< QSPI_CCR_MODE_Pos)   |
				 (cmdStruct->SendInstOnlyOnce	<< QSPI_CCR_SIOO_Pos);
	
	if(cmdStruct->AddressMode != QSPI_PhaseMode_None)
		QSPIx->AR = cmdStruct->Address;
	
	for(int i = 0; i < 3; i++) __NOP();
}


/*******************************************************************************************************************************
* @brief	SPI Flash erase
* @param	QSPIx is the QSPI to use
* @param	addr is the SPI Flash address to erase
* @param	block_size is block size (in kbytes) to erase, support 4 and 64
* @param	wait: 1 wait for erase operation done, 0 send out erase command, and then immediately return.
* @return
*******************************************************************************************************************************/
void QSPI_Erase_(QSPI_TypeDef * QSPIx, uint32_t addr, uint16_t block_size, uint8_t wait)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	uint8_t instruction;
	switch(block_size)
	{
	case 4:
		instruction = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_ERASE_SECTOR    : QSPI_CMD_ERASE_SECTOR;
		break;
	
	case 64:
		instruction = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_ERASE_BLOCK64KB : QSPI_CMD_ERASE_BLOCK64KB;
		break;
	}
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = instruction;
	cmdStruct.AddressMode 		 = QSPI_PhaseMode_1bit;
	cmdStruct.AddressSize		 = AddressSize;
	cmdStruct.Address			 = addr;
	cmdStruct.AlternateBytesMode = QSPI_PhaseMode_None;
	cmdStruct.DummyCycles 		 = 0;
	cmdStruct.DataMode 			 = QSPI_PhaseMode_None;
	
	QSPI_WriteEnable(QSPIx);
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectWrite, &cmdStruct);
	
	while(QSPI_Busy(QSPIx)) __NOP();
	
	if(wait)
		while(QSPI_FlashBusy(QSPIx)) __NOP();
}


/*******************************************************************************************************************************
* @brief	SPI Flash write
* @param	QSPIx is the QSPI to use
* @param	addr is the SPI Flash address to write
* @param	buff is the data to be written to SPI Flash
* @param	count is the number of byte to write, max to 256
* @param	data_width is number of data line to use in data phase, can be 1 or 4
* @param	data_phase indicates whether do data phase in the function, or outside the function (by DMA for example)
* @return
*******************************************************************************************************************************/
void QSPI_Write_(QSPI_TypeDef * QSPIx, uint32_t addr, uint8_t buff[], uint32_t count, uint8_t data_width, uint8_t data_phase)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	uint8_t instruction, dataMode;
	switch(data_width)
	{
	case 1:
		instruction = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_PAGE_PROGRAM      : QSPI_CMD_PAGE_PROGRAM;
		dataMode 	= QSPI_PhaseMode_1bit;
		break;
	
	case 4:
		instruction = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_PAGE_PROGRAM_4bit : QSPI_CMD_PAGE_PROGRAM_4bit;
		dataMode 	= QSPI_PhaseMode_4bit;
		break;
	}
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = instruction;
	cmdStruct.AddressMode 		 = QSPI_PhaseMode_1bit;
	cmdStruct.AddressSize 		 = AddressSize;
	cmdStruct.Address 			 = addr;
	cmdStruct.AlternateBytesMode = QSPI_PhaseMode_None;
	cmdStruct.DummyCycles 		 = 0;
	cmdStruct.DataMode 			 = dataMode;
	cmdStruct.DataCount 		 = count;
	
	QSPI_WriteEnable(QSPIx);
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectWrite, &cmdStruct);
	
	if(data_phase == 0)
		return;
	
	uint32_t n_word = count / 4;
	
	for(int i = 0; i < n_word; i++)
	{
		uint32_t * p_word = (uint32_t *)buff;
		
		while(QSPI_FIFOSpace(QSPIx) < 4) __NOP();
		
		QSPIx->DRW = p_word[i];
	}
	
	if((count % 4) / 2)
	{
		uint16_t * p_half = (uint16_t *)&buff[n_word * 4];
		
		while(QSPI_FIFOSpace(QSPIx) < 2) __NOP();
		
		QSPIx->DRH = p_half[0];
	}
	
	if(count % 2)
	{
		while(QSPI_FIFOSpace(QSPIx) < 1) __NOP();
		
		QSPIx->DRB = buff[count - 1];
	}
	
	while(QSPI_Busy(QSPIx)) __NOP();
	
	while(QSPI_FlashBusy(QSPIx)) __NOP();
}


/*******************************************************************************************************************************
* @brief	SPI Flash read
* @param	QSPIx is the QSPI to use
* @param	addr is the SPI Flash address to read
* @param	buff is the buffer used to save read data
* @param	count is the number of byte to read
* @param	addr_width is number of data line to use in address phase, can be 1, 2 or 4
* @param	data_width is number of data line to use in data phase, can be 1, 2 or 4
* @param	data_phase indicates whether do data phase in the function, or outside the function (by DMA for example)
* @return
*******************************************************************************************************************************/
void QSPI_Read_(QSPI_TypeDef * QSPIx, uint32_t addr, uint8_t buff[], uint32_t count, uint8_t addr_width, uint8_t data_width, uint8_t data_phase)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	uint8_t instruction, addressMode, dataMode, dummyCycles;
	uint8_t alternateBytesMode, alternateBytesSize, alternateBytes;
	switch((addr_width << 4) | data_width)
	{
	case 0x11:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ        : QSPI_CMD_FAST_READ;
		addressMode 	   = QSPI_PhaseMode_1bit;
		alternateBytesMode = QSPI_PhaseMode_None;
		alternateBytesSize = 0;
		dummyCycles        = 8;
		dataMode 		   = QSPI_PhaseMode_1bit;
		break;
	
	case 0x12:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_2bit   : QSPI_CMD_FAST_READ_2bit;
		addressMode 	   = QSPI_PhaseMode_1bit;
		alternateBytesMode = QSPI_PhaseMode_None;
		alternateBytesSize = 0;
		dummyCycles        = 8;
		dataMode 		   = QSPI_PhaseMode_2bit;
		break;
	
	case 0x22:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_IO2bit : QSPI_CMD_FAST_READ_IO2bit;
		addressMode 	   = QSPI_PhaseMode_2bit;
		alternateBytesMode = QSPI_PhaseMode_2bit;
		alternateBytesSize = QSPI_PhaseSize_8bit;
		alternateBytes     = 0xFF;
		dummyCycles        = 0;
		dataMode 		   = QSPI_PhaseMode_2bit;
		break;
	
	case 0x14:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_4bit   : QSPI_CMD_FAST_READ_4bit;
		addressMode 	   = QSPI_PhaseMode_1bit;
		alternateBytesMode = QSPI_PhaseMode_None;
		alternateBytesSize = 0;
		dummyCycles        = 8;
		dataMode 		   = QSPI_PhaseMode_4bit;
		break;
	
	case 0x44:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_IO4bit : QSPI_CMD_FAST_READ_IO4bit;
		addressMode 	   = QSPI_PhaseMode_4bit;
		alternateBytesMode = QSPI_PhaseMode_4bit;
		alternateBytesSize = QSPI_PhaseSize_8bit;
		alternateBytes     = 0xFF;
		dummyCycles        = 4;
		dataMode 		   = QSPI_PhaseMode_4bit;
		break;
	}
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = instruction;
	cmdStruct.AddressMode 		 = addressMode;
	cmdStruct.AddressSize 		 = AddressSize;
	cmdStruct.Address 			 = addr;
	cmdStruct.AlternateBytesMode = alternateBytesMode;
	cmdStruct.AlternateBytesSize = alternateBytesSize;
	cmdStruct.AlternateBytes 	 = alternateBytes;
	cmdStruct.DummyCycles 		 = dummyCycles;
	cmdStruct.DataMode 			 = dataMode;
	cmdStruct.DataCount 		 = count;
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectRead, &cmdStruct);
	
	if(data_phase == 0)
		return;
	
	uint32_t n_word = count / 4;
	
	for(int i = 0; i < n_word; i++)
	{
		uint32_t * p_word = (uint32_t *)buff;
		
		while(QSPI_FIFOCount(QSPIx) < 4) __NOP();
		
		p_word[i] = QSPIx->DRW;
	}
	
	if((count % 4) / 2)
	{
		uint16_t * p_half = (uint16_t *)&buff[n_word * 4];
		
		while(QSPI_FIFOCount(QSPIx) < 2) __NOP();
		
		p_half[0] = QSPIx->DRH;
	}
	
	if(count % 2)
	{
		while(QSPI_FIFOCount(QSPIx) < 1) __NOP();
		
		buff[count - 1] = QSPIx->DRB;
	}
}


/*******************************************************************************************************************************
* @brief	SPI Flash memory map, then you can access SPI Flash content by directly reading address [0x70000000, 0x78000000)
* @param	QSPIx is the QSPI to use
* @param	addr_width is number of data line to use in address phase, can be 1, 2 or 4
* @param	data_width is number of data line to use in data phase, can be 1, 2 or 4
* @return
*******************************************************************************************************************************/
void QSPI_MemoryMap(QSPI_TypeDef * QSPIx, uint8_t addr_width, uint8_t data_width)
{
	while(QSPI_Busy(QSPIx)) __NOP();
	
	QSPIx->CACHE |= QSPI_CACHE_CLR_Msk;
	for(int i = 0; i < CyclesPerUs; i++) __NOP();
	
	uint8_t instruction, addressMode, dataMode, dummyCycles;
	uint8_t alternateBytesMode, alternateBytesSize, alternateBytes;
	switch((addr_width << 4) | data_width)
	{
	case 0x11:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ        : QSPI_CMD_FAST_READ;
		addressMode 	   = QSPI_PhaseMode_1bit;
		alternateBytesMode = QSPI_PhaseMode_None;
		alternateBytesSize = 0;
		dummyCycles        = 8;
		dataMode 		   = QSPI_PhaseMode_1bit;
		break;
	
	case 0x12:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_2bit   : QSPI_CMD_FAST_READ_2bit;
		addressMode 	   = QSPI_PhaseMode_1bit;
		alternateBytesMode = QSPI_PhaseMode_None;
		alternateBytesSize = 0;
		dummyCycles        = 8;
		dataMode 		   = QSPI_PhaseMode_2bit;
		break;
	
	case 0x22:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_IO2bit : QSPI_CMD_FAST_READ_IO2bit;
		addressMode 	   = QSPI_PhaseMode_2bit;
		alternateBytesMode = QSPI_PhaseMode_2bit;
		alternateBytesSize = QSPI_PhaseSize_8bit;
		alternateBytes     = 0xFF;
		dummyCycles        = 0;
		dataMode 		   = QSPI_PhaseMode_2bit;
		break;
	
	case 0x14:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_4bit   : QSPI_CMD_FAST_READ_4bit;
		addressMode 	   = QSPI_PhaseMode_1bit;
		alternateBytesMode = QSPI_PhaseMode_None;
		alternateBytesSize = 0;
		dummyCycles        = 8;
		dataMode 		   = QSPI_PhaseMode_4bit;
		break;
	
	case 0x44:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_IO4bit : QSPI_CMD_FAST_READ_IO4bit;
		addressMode 	   = QSPI_PhaseMode_4bit;
		alternateBytesMode = QSPI_PhaseMode_4bit;
		alternateBytesSize = QSPI_PhaseSize_8bit;
		alternateBytes     = 0xFF;
		dummyCycles        = 4;
		dataMode 		   = QSPI_PhaseMode_4bit;
		break;
	}
	
	if(alternateBytesMode != QSPI_PhaseMode_None)
		QSPIx->ABR = alternateBytes;
		
	QSPIx->CCR = (instruction				<< QSPI_CCR_CODE_Pos)   |
				 (QSPI_PhaseMode_1bit		<< QSPI_CCR_IMODE_Pos)  |
				 (addressMode				<< QSPI_CCR_AMODE_Pos)  |
				 (AddressSize				<< QSPI_CCR_ASIZE_Pos)  |
				 (alternateBytesMode		<< QSPI_CCR_ABMODE_Pos) |
				 (alternateBytesSize		<< QSPI_CCR_ABSIZE_Pos) |
				 (dummyCycles				<< QSPI_CCR_DUMMY_Pos)  |
				 (dataMode					<< QSPI_CCR_DMODE_Pos)  |
				 (QSPI_Mode_MemoryMapped	<< QSPI_CCR_MODE_Pos)   |
				 (0							<< QSPI_CCR_SIOO_Pos);
}


/*******************************************************************************************************************************
* @brief	SPI Flash memory map close
* @param	QSPIx is the QSPI to use
* @return
*******************************************************************************************************************************/
void QSPI_MemoryMapClose(QSPI_TypeDef * QSPIx)
{
	while(QSPI_Busy(QSPIx)) __NOP();
	
	QSPI_FlashBusy(QSPIx);
}


/*******************************************************************************************************************************
* @brief	SPI Flash busy query
* @param	QSPIx is the QSPI to query
* @return	1 SPI Flash is busy for internal erase or write, 0 SPI Flash internal operation done
*******************************************************************************************************************************/
bool QSPI_FlashBusy(QSPI_TypeDef * QSPIx)
{
	uint16_t reg = QSPI_ReadReg(QSPIx, QSPI_CMD_READ_STATUS_REG1, 1);
	
	return reg & (1 << QSPI_STATUS_REG1_BUSY_Pos);
}


/*******************************************************************************************************************************
* @brief	SPI Flash quad state query
* @param	QSPIx is the QSPI to query
* @return	QE bit value
*******************************************************************************************************************************/
uint8_t QSPI_QuadState(QSPI_TypeDef * QSPIx)
{
	uint8_t reg = QSPI_ReadReg(QSPIx, QSPI_CMD_READ_STATUS_REG2, 1);
	
	return (reg & (1 << QSPI_STATUS_REG2_QUAD_Pos)) ? 1 : 0;
}


/*******************************************************************************************************************************
* @brief	SPI Flash quad mode switch
* @param	QSPIx is the QSPI to use
* @param	on/off
* @return
*******************************************************************************************************************************/
void QSPI_QuadSwitch(QSPI_TypeDef * QSPIx, uint8_t on)
{
	uint8_t reg = QSPI_ReadReg(QSPIx, QSPI_CMD_READ_STATUS_REG2, 1);
	
	if(on)
		reg |=  (1 << QSPI_STATUS_REG2_QUAD_Pos);
	else
		reg &= ~(1 << QSPI_STATUS_REG2_QUAD_Pos);
	
	QSPI_WriteEnable(QSPIx);
	
	QSPI_WriteReg(QSPIx, QSPI_CMD_WRITE_STATUS_REG2, reg, 1);
	
	while(QSPI_FlashBusy(QSPIx)) __NOP();
}


/*******************************************************************************************************************************
* @brief	SPI Flash register read
* @param	QSPIx is the QSPI to use
* @param	cmd is the command used to read SPI Flash register
* @param	n_bytes is the number of byte to read, can be 1, 2, 3, or 4
* @return	the data read from SPI Flash, MSB
*******************************************************************************************************************************/
uint32_t QSPI_ReadReg(QSPI_TypeDef * QSPIx, uint8_t cmd, uint8_t n_bytes)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = cmd;
	cmdStruct.AddressMode 		 = QSPI_PhaseMode_None;
	cmdStruct.AlternateBytesMode = QSPI_PhaseMode_None;
	cmdStruct.DummyCycles 		 = 0;
	cmdStruct.DataMode 			 = QSPI_PhaseMode_1bit;
	cmdStruct.DataCount 		 = n_bytes;
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectRead, &cmdStruct);
	
	while(QSPI_FIFOCount(QSPIx) < n_bytes) __NOP();
	
	uint32_t data = 0;
	for(int i = n_bytes; i > 0; i--)
		((uint8_t *)&data)[i-1] = QSPIx->DRB;
	
	return data;
}


/*******************************************************************************************************************************
* @brief	SPI Flash register write
* @param	QSPIx is the QSPI to use
* @param	cmd is the command used to write SPI Flash register
* @param	data is the data to be written to SPI Flash register, MSB
* @param	nbyte is the number of byte to write, can be 1, 2, 3, 4, or 0 (execute instruction without data)
* @return
*******************************************************************************************************************************/
void QSPI_WriteReg(QSPI_TypeDef * QSPIx, uint8_t cmd, uint32_t data, uint8_t n_bytes)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = cmd;
	cmdStruct.AddressMode 		 = QSPI_PhaseMode_None;
	cmdStruct.AlternateBytesMode = QSPI_PhaseMode_None;
	cmdStruct.DummyCycles 		 = 0;
	cmdStruct.DataMode 			 = n_bytes ? QSPI_PhaseMode_1bit : QSPI_PhaseMode_None;
	cmdStruct.DataCount 		 = n_bytes;
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectWrite, &cmdStruct);
	
	for(int i = n_bytes; i > 0; i--)
		QSPIx->DRB = ((uint8_t *)&data)[i-1];
	
	while(QSPI_Busy(QSPIx)) __NOP();
}


/*******************************************************************************************************************************
* @brief	QSPI interrupt enable
* @param	QSPIx is the QSPI to set
* @param	it is interrupt type, can be QSPI_IT_ERR, QSPI_IT_DONE, QSPI_IT_FFTHR, QSPI_IT_PSMAT, QSPI_IT_TO and their '|' operation
* @return
*******************************************************************************************************************************/
void QSPI_INTEn(QSPI_TypeDef * QSPIx, uint32_t it)
{
	QSPIx->CR |=  (it << 16);
}

/*******************************************************************************************************************************
* @brief	QSPI interrupt disable
* @param	QSPIx is the QSPI to set
* @param	it is interrupt type, can be QSPI_IT_ERR, QSPI_IT_DONE, QSPI_IT_FFTHR, QSPI_IT_PSMAT, QSPI_IT_TO and their '|' operation
* @return
*******************************************************************************************************************************/
void QSPI_INTDis(QSPI_TypeDef * QSPIx, uint32_t it)
{
	QSPIx->CR &= ~(it << 16);
}

/*******************************************************************************************************************************
* @brief	QSPI interrupt flag clear
* @param	QSPIx is the QSPI to set
* @param	it is interrupt type, can be QSPI_IT_ERR, QSPI_IT_DONE, QSPI_IT_FFTHR, QSPI_IT_PSMAT, QSPI_IT_TO and their '|' operation
* @return
*******************************************************************************************************************************/
void QSPI_INTClr(QSPI_TypeDef * QSPIx, uint32_t it)
{
	QSPIx->FCR = it;
}

/*******************************************************************************************************************************
* @brief	QSPI interrupt state query
* @param	QSPIx is the QSPI to query
* @param	it is interrupt type, can be QSPI_IT_ERR, QSPI_IT_DONE, QSPI_IT_FFTHR, QSPI_IT_PSMAT, QSPI_IT_TO and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t QSPI_INTStat(QSPI_TypeDef * QSPIx, uint32_t it)
{
	return QSPIx->SR & it;
}
