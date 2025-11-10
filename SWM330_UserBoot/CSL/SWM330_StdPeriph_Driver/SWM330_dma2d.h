#ifndef __SWM330_DMA2D_H__
#define __SWM330_DMA2D_H__

#include <stdint.h>


typedef struct {
	uint8_t  BurstSize;			// RDMA_BURST_INC16, RDMA_BURST_INC8, RDMA_BURST_INC4, RDMA_BURST_SINGLE
	uint8_t  BlockSize;			// RDMA_BLOCK_16, RDMA_BLOCK_32, RDMA_BLOCK_64, RDMA_BLOCK_128, in unit of number of HREADY
	uint16_t Interval;			// data block transfer interval in unit of HCLK period, can be 1--65535
	
	uint8_t  IntEn;				// Interrupt Enable, can be DMA2D_IT_DONE, DMA2D_IT_HALF, DMA2D_IT_ERROR and their '|' operation
} DMA2D_InitStructure;

#define DMA2D_BURST_INC16	0
#define DMA2D_BURST_INC8	1
#define DMA2D_BURST_INC4	2
#define DMA2D_BURST_SINGLE	3

#define DMA2D_BLOCK_16		0
#define DMA2D_BLOCK_32		1
#define DMA2D_BLOCK_64		2
#define DMA2D_BLOCK_128		3


typedef struct {
	uint32_t Address;
	uint32_t LineOffset;		// added at the end of each line to determine the starting address of the next line
	uint8_t  ColorMode;			// DMA2D_FMT_ARGB888, DMA2D_FMT_RGB888, DMA2D_FMT_RGB565, ...
	uint8_t  AlphaMode;			// DMA2D_AMODE_PIXEL, DMA2D_AMODE_ALPHA, DMA2D_AMODE_PMULA, ...
	uint8_t  Alpha;				// used for DMA2D_AMODE_ALPHA mode, Assign a fixed Alpha value to the layer
	uint32_t AlhpaAddr;			// used for DMA2D_AMODE_EXTERN mode, only suitable for foreground layer, Alpha values are stored in a separate storage area specified by AlphaAddr
	
	/* only for DMA2D_LAYER_OUT, not for DMA2D_LAYER_FG and DMA2D_LAYER_BG */
	uint16_t LineCount;			// line per screen
	uint16_t LinePixel;			// pixel per line
} DMA2D_LayerSetting;


#define DMA2D_LAYER_FG		0	// Foreground layer
#define DMA2D_LAYER_BG		1	// Background layer
#define DMA2D_LAYER_OUT		2	// Output layer

/* Color Format */
#define DMA2D_FMT_ARGB888   (0 | (0 << 4))
#define DMA2D_FMT_RGB888    (1 | (0 << 4))
#define DMA2D_FMT_RGB565    (2 | (0 << 4))
#define DMA2D_FMT_ABGR888   (0 | (1 << 4))
#define DMA2D_FMT_BGR888    (1 | (1 << 4))
#define DMA2D_FMT_BGR565    (2 | (1 << 4))

/* Alpha Mode */
#define DMA2D_AMODE_PIXEL	(0 | (0 << 5))	// use the pixel's own Alpha value
#define DMA2D_AMODE_ALPHA	(0 | (1 << 5))	// use the Alpha value specified by software
#define DMA2D_AMODE_PMULA	(0 | (2 << 5))	// use the product of the pixel's own Alpha value and the Alpha value specified by software
#define DMA2D_AMODE_EXTERN	(0 | (3 << 5))	// only suitable for foreground layer, Alpha values are stored in a separate storage area specified by AlphaAddr


/* when GPDMA mode, transfer unit */
#define DMA2D_UNIT_BYTE		4
#define DMA2D_UNIT_HALF		5
#define DMA2D_UNIT_WORD		6


/* Interrupt Type */
#define DMA2D_IT_DONE		DMA2D_IF_DONE_Msk		// transfer done
#define DMA2D_IT_HALF		DMA2D_IF_PART_Msk		// GPDMA transfer half done
#define DMA2D_IT_ERROR		DMA2D_IF_ERROR_Msk		// Configration error


void DMA2D_Init(DMA2D_InitStructure * initStruct);
void DMA2D_PixelFill(DMA2D_LayerSetting * outLayer, uint32_t color);
void DMA2D_PixelMove(DMA2D_LayerSetting * fgLayer, DMA2D_LayerSetting * outLayer);
void DMA2D_PixelConvert(DMA2D_LayerSetting * fgLayer, DMA2D_LayerSetting * outLayer);
void DMA2D_PixelBlend(DMA2D_LayerSetting * fgLayer, DMA2D_LayerSetting * bgLayer, DMA2D_LayerSetting * outLayer);
uint32_t DMA2D_IsBusy(void);

void DMA2D_memcpy(void * destin, const void * source, uint8_t unit_size, uint16_t unit_count);

void DMA2D_INTEn(uint32_t it);
void DMA2D_INTDis(uint32_t it);
void DMA2D_INTClr(uint32_t it);
uint32_t DMA2D_INTStat(uint32_t it);


#endif
