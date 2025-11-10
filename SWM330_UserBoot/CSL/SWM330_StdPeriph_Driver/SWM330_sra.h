#ifndef __SWM330_SRA_H__
#define __SWM330_SRA_H__


typedef struct {
	uint8_t  IntEOTEn;		// End of Transform
} SRA_InitStructure;


#define SRA_FMT_MONO8b		(0 | (0 << 3) | (0 << 4))	// Monochrome 8-bit
#define SRA_FMT_RGB565		(1 | (0 << 3) | (0 << 4))
#define SRA_FMT_BGR565		(1 | (1 << 3) | (0 << 4))
#define SRA_FMT_RGB888		(2 | (0 << 3) | (0 << 4))
#define SRA_FMT_BGR888		(2 | (1 << 3) | (0 << 4))
#define SRA_FMT_XRGB8888	(3 | (0 << 3) | (0 << 4))
#define SRA_FMT_XBGR8888	(3 | (1 << 3) | (0 << 4))
#define SRA_FMT_RGBX8888	(3 | (0 << 3) | (1 << 4))
#define SRA_FMT_BGRX8888	(3 | (1 << 3) | (1 << 4))
#define SRA_FMT_XARGB88565	(5 | (0 << 3) | (0 << 4))
#define SRA_FMT_XABGR88565	(5 | (1 << 3) | (0 << 4))
#define SRA_FMT_ARGBX85658	(5 | (0 << 3) | (1 << 4))
#define SRA_FMT_ABGRX85658	(5 | (1 << 3) | (1 << 4))
#define SRA_FMT_ARGB8565	(6 | (0 << 3) | (0 << 4))
#define SRA_FMT_ABGR8565	(6 | (1 << 3) | (0 << 4))
#define SRA_FMT_ARGB8888	(7 | (0 << 3) | (0 << 4))
#define SRA_FMT_ABGR8888	(7 | (1 << 3) | (0 << 4))
#define SRA_FMT_RGBA8888	(7 | (0 << 3) | (1 << 4))
#define SRA_FMT_BGRA8888	(7 | (1 << 3) | (1 << 4))


typedef struct {
	uint16_t SrcFormat;			// SRA_FMT_MONO8b, SRA_FMT_RGB565, SRA_FMT_BGR565, ...
	uint16_t DstFormat;			// SRA_FMT_MONO8b, SRA_FMT_RGB565, SRA_FMT_BGR565, ...
	uint32_t ImageAddr;
	uint16_t ImageWidth;
	uint16_t ImageHeight;
	uint16_t ScaleWidth;		// Image width after scaling:  0.5 <= ScaleWidth/ImageWidth   <= 4
	uint16_t ScaleHeight;		// Image height after scaling: 0.5 <= ScaleHeight/ImageHeight <= 4
	uint16_t RotateAngle;		// Rotation angle, can be 0-359, SRA_MIRROR_X, SRA_MIRROR_Y and SRA_MIRROR_XY
	uint32_t OutputAddr;
	uint32_t OutputWidth;
} SRA_TransformParam;


#define SRA_MIRROR_X		361	// Mirror Horizontally
#define SRA_MIRROR_Y		362	// Mirror Vertically 
#define SRA_MIRROR_XY		363	// Horizontal and vertical mirroring


#define SRA_IT_FRAME		(1 << 0)
#define SRA_IT_8LINE		(1 << 1)
#define SRA_IT_ROTERR		(1 << 2)		// Rotate Error
#define SRA_IT_BUSERR		(1 << 3)



void SRA_Init(SRA_InitStructure * initStruct);
bool SRA_Transform(SRA_TransformParam * transParam, uint16_t *dstWidth, uint16_t *dstHeight);

void SRA_Start(void);
void SRA_Abort(void);
uint32_t SRA_Busy(void);

void SRA_INTEn(uint32_t it);
void SRA_INTDis(uint32_t it);
void SRA_INTClr(uint32_t it);
uint32_t SRA_INTStat(uint32_t it);


#endif
