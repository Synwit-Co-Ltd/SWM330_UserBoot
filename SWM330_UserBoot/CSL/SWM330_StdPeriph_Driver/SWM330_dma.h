#ifndef __SWM330_DMA_H__
#define __SWM330_DMA_H__


typedef struct {
	uint8_t  Mode;				// DMA_MODE_SINGLE, DMA_MODE_CIRCLE
	
	uint8_t  Unit;				// DMA_UNIT_BYTE, DMA_UNIT_HALFWORD, DMA_UNIT_WORD
	
	uint32_t Count;				// Number of transmission count. can be 1--0xFFFFFF
	
	uint32_t MemoryAddr;
	
	uint32_t PeripheralAddr;
	
	uint8_t  MemoryAddrInc;		// 0 Fixed address    1 Address increment
	
	uint8_t  PeripheralAddrInc;
	
	uint8_t  Handshake;			// handshake signal: DMA_HS_NO, DMA_CH0_UART0TX, DMA_CH0_SPI0TX, ...
	
	uint8_t  Priority;			// DMA_PRI_HIGHEST, DMA_PRI_VERY_HIGH, ..., DMA_PRI_LOWEST
	
	uint32_t  INTEn;			// interrupt enable, can be DMA_IT_DONE, DMA_IT_HALF, DMA_IT_ERROR and their '|' operation
} DMA_InitStructure;


#define DMA_CH0		0
#define DMA_CH1		1
#define DMA_CH2		2
#define DMA_CH3		3

#define DMA_MODE_SINGLE			0		// single mode, stop after the transmission is complete
#define DMA_MODE_CIRCLE			1		// circle mode, the next round of transmission is executed after the transmission is complete

#define DMA_UNIT_BYTE		0
#define DMA_UNIT_HALFWORD	1
#define DMA_UNIT_WORD		2

#define DMA_PRI_HIGHEST		0
#define DMA_PRI_VERY_HIGH	2
#define DMA_PRI_HIGH		4
#define DMA_PRI_LOW			11
#define DMA_PRI_VERY_LOW	13
#define DMA_PRI_LOWEST		15


#define DMA_HS_NO			(0 << 4)	// no handshake, used for memory copy, PeripheralAddr used as destination address
#define DMA_HS_MRD			(1 << 4)	// memory read  handshake, suitable for Memory to Peripheral transfers
#define DMA_HS_MWR			(2 << 4)	// memory write handshake, suitable for Peripheral to Memory transfers
#define DMA_HS_EXT			(4 << 4)	// external handshake
#define DMA_HS_MSK			(7 << 4)

// memory read handshake signal
#define DMA_CH0_UART0TX		(0 | DMA_HS_MRD)
#define DMA_CH0_QSPI0TX		(1 | DMA_HS_MRD)
#define DMA_CH0_SPI0TX		(2 | DMA_HS_MRD)

#define DMA_CH1_UART1TX		(0 | DMA_HS_MRD)
#define DMA_CH1_QSPI1TX		(1 | DMA_HS_MRD)
#define DMA_CH1_I2S0TX		(2 | DMA_HS_MRD)

#define DMA_CH2_UART2TX		(0 | DMA_HS_MRD)
#define DMA_CH2_QSPI0TX		(1 | DMA_HS_MWR)
#define DMA_CH2_I2S0TX		(2 | DMA_HS_MRD)

#define DMA_CH3_UART3TX		(0 | DMA_HS_MRD)
#define DMA_CH3_QSPI1TX		(1 | DMA_HS_MWR)
#define DMA_CH3_SPI0TX		(2 | DMA_HS_MRD)

// memory write handshake signal
#define DMA_CH0_UART1RX		(0 | DMA_HS_MWR)
#define DMA_CH0_QSPI1RX		(1 | DMA_HS_MWR)
#define DMA_CH0_I2S0RX		(2 | DMA_HS_MWR)
#define DMA_CH0_ADC0SEQ0	(3 | DMA_HS_MWR)

#define DMA_CH1_UART0RX		(0 | DMA_HS_MWR)
#define DMA_CH1_QSPI0RX		(1 | DMA_HS_MWR)
#define DMA_CH1_SPI0RX		(2 | DMA_HS_MWR)
#define DMA_CH1_ADC0SEQ1	(3 | DMA_HS_MWR)

#define DMA_CH2_UART3RX		(0 | DMA_HS_MWR)
#define DMA_CH2_QSPI1RX		(1 | DMA_HS_MWR)
#define DMA_CH2_SPI0RX		(2 | DMA_HS_MWR)
#define DMA_CH2_ADC0SEQ2	(3 | DMA_HS_MWR)

#define DMA_CH3_UART2RX		(0 | DMA_HS_MWR)
#define DMA_CH3_QSPI0RX		(1 | DMA_HS_MWR)
#define DMA_CH3_I2S0RX		(2 | DMA_HS_MWR)
#define DMA_CH3_ADC0SEQ3	(3 | DMA_HS_MWR)


// external handshake signal
#define DMA_EXMRD_TIMR0		(0 | DMA_HS_MRD | DMA_HS_EXT)
#define DMA_EXMRD_TIMR1		(1 | DMA_HS_MRD | DMA_HS_EXT)
#define DMA_EXMRD_BTIMR0	(2 | DMA_HS_MRD | DMA_HS_EXT)
#define DMA_EXMRD_BTIMR1	(3 | DMA_HS_MRD | DMA_HS_EXT)
#define DMA_EXMRD_BTIMR2	(4 | DMA_HS_MRD | DMA_HS_EXT)
#define DMA_EXMRD_BTIMR3	(5 | DMA_HS_MRD | DMA_HS_EXT)
#define DMA_EXMRD_TRIG0		(6 | DMA_HS_MRD | DMA_HS_EXT)	// DMA_TRIG0 pin
#define DMA_EXMRD_TRIG1		(7 | DMA_HS_MRD | DMA_HS_EXT)	// DMA_TRIG1 pin

#define DMA_EXMWR_TIMR0		(0 | DMA_HS_MWR | DMA_HS_EXT)
#define DMA_EXMWR_TIMR1		(1 | DMA_HS_MWR | DMA_HS_EXT)
#define DMA_EXMWR_BTIMR0	(2 | DMA_HS_MWR | DMA_HS_EXT)
#define DMA_EXMWR_BTIMR1	(3 | DMA_HS_MWR | DMA_HS_EXT)
#define DMA_EXMWR_BTIMR2	(4 | DMA_HS_MWR | DMA_HS_EXT)
#define DMA_EXMWR_BTIMR3	(5 | DMA_HS_MWR | DMA_HS_EXT)
#define DMA_EXMWR_TRIG0		(6 | DMA_HS_MWR | DMA_HS_EXT)
#define DMA_EXMWR_TRIG1		(7 | DMA_HS_MWR | DMA_HS_EXT)



/* Interrupt Type */
#define DMA_IT_DONE		(1 << 1)	//Transfer Done
#define DMA_IT_HALF		(1 << 2)	//Transfer Half
#define DMA_IT_ERROR	(1 << 3)	//Transfer Error



void DMA_CH_Init(uint32_t chn, DMA_InitStructure * initStruct);
void DMA_CH_Open(uint32_t chn);
void DMA_CH_Close(uint32_t chn);


static inline void DMA_CH_SetCount(uint32_t chn, uint32_t count)
{
	DMA->CH[chn].NDT = (count << DMA_NDT_LEN_Pos) |
					   (4	  << DMA_NDT_HALF_Pos);
}


static inline uint32_t DMA_CH_GetRemaining(uint32_t chn)
{
	return (DMA->CH[chn].NDT & DMA_NDT_LEN_Msk);
}


static inline void DMA_CH_SetAddrAndCount(uint32_t chn, uint32_t addr, uint32_t count)
{
	DMA->CH[chn].MAR = addr;
	
	DMA_CH_SetCount(chn, count);
}

void DMA_CH_INTEn(uint32_t chn, uint32_t it);
void DMA_CH_INTDis(uint32_t chn, uint32_t it);
void DMA_CH_INTClr(uint32_t chn, uint32_t it);
uint32_t DMA_CH_INTStat(uint32_t chn, uint32_t it);


#endif
