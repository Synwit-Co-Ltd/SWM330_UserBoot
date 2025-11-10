#ifndef __SWM330_RDMA_H__
#define __SWM330_RDMA_H__


typedef struct {
	uint8_t  BurstSize;		// RDMA_BURST_INC16, RDMA_BURST_INC8, RDMA_BURST_INC4, RDMA_BURST_SINGLE
	uint8_t  BlockSize;		// RDMA_BLOCK_16, RDMA_BLOCK_32, RDMA_BLOCK_64, RDMA_BLOCK_128, in unit of number of HREADY
	uint16_t Interval;		// data block transfer interval in unit of HCLK period, can be 1--65535
	
	uint32_t INTEn;			// interrupt enable, can be RDMA_IT_DONE, RDMA_IT_HALF, RDMA_IT_ERROR and their '|' operation
} RDMA_InitStructure;


#define RDMA_BURST_INC16	0
#define RDMA_BURST_INC8		1
#define RDMA_BURST_INC4		2
#define RDMA_BURST_SINGLE	3

#define RDMA_BLOCK_16		0
#define RDMA_BLOCK_32		1
#define RDMA_BLOCK_64		2
#define RDMA_BLOCK_128		3

#define RDMA_UNIT_BYTE		0
#define RDMA_UNIT_HALFWORD	1
#define RDMA_UNIT_WORD		2



/* Interrupt Type */
#define RDMA_IT_DONE	(1 << 0)	// Transfer Done
#define RDMA_IT_HALF	(1 << 1)	// Transfer Half
#define RDMA_IT_ERROR	(1 << 2)	// Transfer Error



void RDMA_Init(RDMA_InitStructure * initStruct);
void RDMA_memcpy(void * destin, const void * source, uint8_t unit_size, uint32_t unit_count);


static inline uint32_t RDMA_Remaining(void)
{
	return (RDMA->LEN & RDMA_LEN_SIZE_Msk);
}


void RDMA_INTEn(uint32_t it);
void RDMA_INTDis(uint32_t it);
void RDMA_INTClr(uint32_t it);
uint32_t RDMA_INTStat(uint32_t it);


#endif
