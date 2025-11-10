#ifndef __SWM330_DVP_H__
#define __SWM330_DVP_H__


typedef struct {
	uint16_t InFormat;			// DVP_INFMT_RAW_8b, DVP_INFMT_RAW_10b, ...
	uint16_t OutFormat;			// DVP_OUTFMT_RAW, DVP_OUTFMT_YUV422, DVP_OUTFMT_YUV420
	
	/* position and size of output window */
	uint16_t StartLine;			// start from 1
	uint16_t LineCount;
	uint16_t StartPixel;		// must be multiple of 2, start from 1
	uint16_t PixelCount;		// must be multiple of 4
	
	uint16_t LineStride;		// must be multiple of 8, line stride in pixel
	
	uint8_t  SampleEdge;		// DVP_PCKPolarity_Rising, DVP_PCKPolarity_Falling
	
	/* for RAW/RGB output */
	uint32_t RGBAddr;			// must be word aligned
	
	/* for YUV output */
	uint32_t YAddr;				// must be word aligned
	uint32_t UVAddr;			// must be word aligned
	
	uint8_t  IntEn;				// DVP_IT_DONE, DVP_IT_FRAME, ..., DVP_IT_UPDATE and their '|' operation
} DVP_InitStructure;


#define DVP_INFMT_RAW_8b		((0 << 11) | (0 << 6) | (0 << 4) | 0)
#define DVP_INFMT_RAW_10b		((1 << 11) | (0 << 6) | (0 << 4) | 0)
#define DVP_INFMT_RAW_12b		((2 << 11) | (0 << 6) | (0 << 4) | 0)
#define DVP_INFMT_RAW_14b		((3 << 11) | (0 << 6) | (0 << 4) | 0)
#define DVP_INFMT_RGB565		((0 << 11) | (0 << 6) | (0 << 4) | 8)
#define DVP_INFMT_RGB565_SWAP	((0 << 11) | (1 << 6) | (0 << 4) | 8)	// odd/even byte swap
#define DVP_INFMT_YUV422_YUYV	((0 << 11) | (0 << 6) | (0 << 4) | 1)
#define DVP_INFMT_YUV422_YVYU	((0 << 11) | (0 << 6) | (1 << 4) | 1)
#define DVP_INFMT_YUV422_UYVY	((0 << 11) | (0 << 6) | (2 << 4) | 1)
#define DVP_INFMT_YUV422_VYUY	((0 << 11) | (0 << 6) | (3 << 4) | 1)
#define DVP_INFMT_BT656_YUYV	((0 << 11) | (0 << 6) | (0 << 4) | 2)
#define DVP_INFMT_BT656_YVYU	((0 << 11) | (0 << 6) | (1 << 4) | 2)
#define DVP_INFMT_BT656_UYVY	((0 << 11) | (0 << 6) | (2 << 4) | 2)
#define DVP_INFMT_BT656_VYUY	((0 << 11) | (0 << 6) | (3 << 4) | 2)

#define DVP_OUTFMT_RAW			0	// no processing, save as input
#define DVP_OUTFMT_YUV422		1	// NV16
#define DVP_OUTFMT_YUV420		2	// NV12
#define DVP_OUTFMT_RGB565		3

#define DVP_PCKPolarity_Rising		0		// sample on rising edge
#define DVP_PCKPolarity_Falling		1		// sample on falling edge

#define DVP_HSPolarity_High			0		// DVP_HREF active high
#define DVP_HSPolarity_Low			1		// DVP_HREF active low

#define DVP_VSPolarity_High			0		// DVP_VSYNC active high
#define DVP_VSPolarity_Low			1		// DVP_VSYNC active low


/* Interrupt Type */
#define DVP_IT_DONE		DVP_IE_DONE_Msk		// all frames capture done
#define DVP_IT_FRAME	DVP_IE_FRAME_Msk	// one frame capture done
#define DVP_IT_LINES	DVP_IE_LINES_Msk	// half of frame capture done
#define DVP_IT_BUFULL	DVP_IE_BUFULL_Msk	// buffer full
#define DVP_IT_HSZCHG	DVP_IE_HSZCHG_Msk	// input horizontal size changed
#define DVP_IT_VSZCHG	DVP_IE_VSZCHG_Msk	// input vertical size changed
#define DVP_IT_CODERR	DVP_IE_CODERR_Msk	// XY code error
#define DVP_IT_UPDATE	DVP_IE_UPDATE_Msk	// output address update done



void DVP_Init(DVP_TypeDef * DVPx, DVP_InitStructure * initStruct);

void DVP_Start(DVP_TypeDef * DVPx, uint16_t start_frame, uint16_t frame_count);
void DVP_Stop(DVP_TypeDef * DVPx, uint32_t immediate);
bool DVP_Busy(DVP_TypeDef * DVPx);


void DVP_INTEn(DVP_TypeDef * DVPx, uint32_t it);
void DVP_INTDis(DVP_TypeDef * DVPx, uint32_t it);
void DVP_INTClr(DVP_TypeDef * DVPx, uint32_t it);
uint32_t DVP_INTStat(DVP_TypeDef * DVPx, uint32_t it);


#endif
