/*******************************************************************************************************************************
* @brief	SRA driver
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
#include <math.h>
#include <stdlib.h>
#include "SWM330.h"
#include "SWM330_sra.h"


/*******************************************************************************************************************************
* @brief	SRA init
* @param	initStruct is data used to init the SRA
* @return
*******************************************************************************************************************************/
void SRA_Init(SRA_InitStructure * initStruct)
{
	SYS->CLKEN0 |= (1 << SYS_CLKEN0_SRA_Pos);
	
	SRA->IFE   = 0x0F;
	SRA->IDIS |= 0xF0;
	SRA->IF    = 0x0F;		// Interrupt Clear
	SRA->IDIS &= ~(initStruct->IntEOTEn << SRA_IDIS_FRAME_Pos);
	
	if(initStruct->IntEOTEn)
		NVIC_EnableIRQ(SRA_IRQn);
}


#define PI 3.141592653589793
static double deg2rad(double degree)
{
	return degree / 180 * PI;
}

/*******************************************************************************************************************************
* @brief	SRA scaling and rotation transform parameters configure, and then you can use SRA_Start() to perform the transform
* @param	transParam is the scaling and rotation transform parameters to set
* @param	dstWidth is the width of the transformed image
* @param	dstHeight is the height of the transformed image
* @return	true: parameter valid, false: parameter error
*******************************************************************************************************************************/
bool SRA_Transform(SRA_TransformParam * transParam, uint16_t *dstWidth, uint16_t *dstHeight)
{
	int degree = transParam->RotateAngle;
	int mirror = transParam->RotateAngle - 360;
	if((mirror == 1) || (mirror == 2) || (mirror == 3))		// mirror
		degree = 0;
	else													// rotation
		mirror = 0;
	
	double real_factor_x = transParam->ScaleWidth  * 1.0 / transParam->ImageWidth,
		   real_factor_y = transParam->ScaleHeight * 1.0 / transParam->ImageHeight;
	if((real_factor_x > 4) | (real_factor_x < 0.5) |
	   (real_factor_y > 4) | (real_factor_y < 0.5))
		return false;
	
	int src_width_m1  = transParam->ImageWidth  - 1,
		src_height_m1 = transParam->ImageHeight - 1,
		scl_width_m1  = transParam->ScaleWidth  - 1,
		scl_height_m1 = transParam->ScaleHeight - 1;
	
	double scl_factor_x = src_width_m1  * 1.0 / scl_width_m1,
		   scl_factor_y = src_height_m1 * 1.0 / scl_height_m1;
	int    scl_factor_x_para = round(scl_factor_x * pow(2, 13)),
		   scl_factor_y_para = round(scl_factor_y * pow(2, 13));
	
	double sra_sin = sin(deg2rad(degree)),
		   sra_cos = cos(deg2rad(degree));
	int    sra_sin_para = round(sra_sin * pow(2, 13)),
		   sra_cos_para = round(sra_cos * pow(2, 13));
	double sra_sin_fp = sra_sin_para / pow(2, 13),
		   sra_cos_fp = sra_cos_para / pow(2, 13);
	
	double sra_sin_xfact = sra_sin_para * scl_factor_x_para,
		   sra_sin_yfact = sra_sin_para * scl_factor_y_para,
		   sra_cos_xfact = sra_cos_para * scl_factor_x_para,
		   sra_cos_yfact = sra_cos_para * scl_factor_y_para;
	int    sra_sin_xfact_para = floor(sra_sin_xfact / pow(2, 13)),
		   sra_sin_yfact_para = floor(sra_sin_yfact / pow(2, 13)),
		   sra_cos_xfact_para = floor(sra_cos_xfact / pow(2, 13)),
		   sra_cos_yfact_para = floor(sra_cos_yfact / pow(2, 13));
	double sra_sin_xfact_fp = sra_sin_xfact_para / pow(2, 13),
		   sra_sin_yfact_fp = sra_sin_yfact_para / pow(2, 13),
		   sra_cos_xfact_fp = sra_cos_xfact_para / pow(2, 13),
		   sra_cos_yfact_fp = sra_cos_yfact_para / pow(2, 13);
	
	if(sra_sin < 0)
	{
		sra_sin_xfact_para = pow(2, 16) + sra_sin_xfact_para;
		sra_sin_yfact_para = pow(2, 16) + sra_sin_yfact_para;
	}
	if(sra_cos < 0)
	{
		sra_cos_xfact_para = pow(2, 16) + sra_cos_xfact_para;
		sra_cos_yfact_para = pow(2, 16) + sra_cos_yfact_para;
	}

	double dst_offset_x,
		   dst_offset_y,
		   dst_tile_offset_x,
		   dst_tile_offset_y;
	if(mirror == 1)			// x-mirror
	{
		dst_offset_x      = scl_width_m1;
		dst_offset_y      = 0;
		dst_tile_offset_x = 7 * sra_cos_xfact_fp;
		dst_tile_offset_y = 0;
	}
	else if(mirror == 2)	// y-mirror
	{
		dst_offset_x      = 0;
		dst_offset_y      = scl_height_m1;
		dst_tile_offset_x = 0;
		dst_tile_offset_y = 7 * sra_cos_yfact_fp;
	}
	else if(mirror == 3)	// xy-mirror
	{
		dst_offset_x      = scl_width_m1;
		dst_offset_y      = scl_height_m1;
		dst_tile_offset_x = 7 * sra_cos_xfact_fp;
		dst_tile_offset_y = 7 * sra_cos_yfact_fp;
	}
	else
	{
		if((degree >= 0) && (degree < 90))
		{
			dst_offset_x      = - sra_sin_fp * scl_height_m1;
			dst_offset_y      = 0;
			dst_tile_offset_x = 0;
			dst_tile_offset_y = 7 * sra_sin_yfact_fp;
		}
		else if((degree >= 90) && (degree < 180))
		{
			dst_offset_x      = sra_cos_fp * scl_width_m1 - sra_sin_fp * scl_height_m1;
			dst_offset_y      = sra_cos_fp * scl_height_m1;
			dst_tile_offset_x = -7 * sra_cos_xfact_fp;
			dst_tile_offset_y = -7 * sra_cos_yfact_fp + 7 * sra_sin_yfact_fp;
		}
		else if((degree >= 180) && (degree < 270))
		{
			dst_offset_x      = sra_cos_fp * scl_width_m1;
			dst_offset_y      = sra_cos_fp * scl_height_m1 + sra_sin_fp * scl_width_m1;
			dst_tile_offset_x = -7 * sra_cos_xfact_fp - 7 * sra_sin_xfact_fp;
			dst_tile_offset_y = -7 * sra_cos_yfact_fp;
		}
		else
		{
			dst_offset_x      = 0;
			dst_offset_y      = sra_sin_fp * scl_width_m1;
			dst_tile_offset_x = -7 * sra_sin_xfact_fp;
			dst_tile_offset_y = 0;
		}
	}
	
	double dst_tile_remap_stx,
		   dst_tile_remap_sty;
	if(mirror == 1)			// x-mirror
	{
		dst_tile_remap_stx = (-7 + dst_offset_x) * sra_cos_xfact_fp; 	// (t,0)
		dst_tile_remap_sty = ( 0 + dst_offset_y) * sra_cos_yfact_fp; 	// (t,0)
	}
	else if(mirror == 2)	// y-mirror 
	{
		dst_tile_remap_stx = ( 0 + dst_offset_x) * sra_cos_xfact_fp; 	// (0,t)
		dst_tile_remap_sty = (-7 + dst_offset_y) * sra_cos_yfact_fp; 	// (0,t)
	}
	else if(mirror == 3)	// xy-mirror 
	{
		dst_tile_remap_stx = (-7 + dst_offset_x) * sra_cos_xfact_fp; 	// (t,0)
		dst_tile_remap_sty = (-7 + dst_offset_y) * sra_cos_yfact_fp; 	// (0,t)
	}
	else
	{
		if((degree >= 0) && (degree < 90))
		{
			dst_tile_remap_stx = (0 + dst_offset_x) * (+sra_cos_xfact_fp) +(0 + dst_offset_y) * sra_sin_xfact_fp; 	// (0,0)
			dst_tile_remap_sty = (7 + dst_offset_x) * (-sra_sin_yfact_fp) +(0 + dst_offset_y) * sra_cos_yfact_fp; 	// (t,0)
		}
		else if((degree >= 90) && (degree < 180))
		{
			dst_tile_remap_stx = (7 + dst_offset_x) * (+sra_cos_xfact_fp) +(0 + dst_offset_y) * sra_sin_xfact_fp;	// (t,0)
			dst_tile_remap_sty = (7 + dst_offset_x) * (-sra_sin_yfact_fp) +(7 + dst_offset_y) * sra_cos_yfact_fp;	// (t,t)
		}
		else if((degree >= 180) && (degree < 270))
		{
			dst_tile_remap_stx = (7 + dst_offset_x) * (+sra_cos_xfact_fp) +(7 + dst_offset_y) * sra_sin_xfact_fp;	// (t,t)
			dst_tile_remap_sty = (0 + dst_offset_x) * (-sra_sin_yfact_fp) +(7 + dst_offset_y) * sra_cos_yfact_fp;	// (0,t)
		}
		else
		{
			dst_tile_remap_stx = (0 + dst_offset_x) * (+sra_cos_xfact_fp) +(7 + dst_offset_y) * sra_sin_xfact_fp;	// (0,t)
			dst_tile_remap_sty = (0 + dst_offset_x) * (-sra_sin_yfact_fp) +(0 + dst_offset_y) * sra_cos_yfact_fp;	// (0,0)
		}
	}
	
	// SRC first tile start position
	double src_tile_stx    = dst_tile_remap_stx,
		   src_tile_sty    = dst_tile_remap_sty;
	double src_tile_stx_fp = round(src_tile_stx * pow(2, 13)) / pow(2, 13),
		   src_tile_sty_fp = round(src_tile_sty * pow(2, 13)) / pow(2, 13);
	int    src_tile_stx_para,
		   src_tile_sty_para;
	
#define IMG_SIZE_W 16
	if(src_tile_stx < 0)
		src_tile_stx_para = round(src_tile_stx * pow(2, 13)) + pow(2, IMG_SIZE_W+13+1);
	else
		src_tile_stx_para = round(src_tile_stx * pow(2, 13));
	
	if(src_tile_sty < 0)
		src_tile_sty_para = round(src_tile_sty * pow(2, 13)) + pow(2, IMG_SIZE_W+13+1);
	else
		src_tile_sty_para = round(src_tile_sty * pow(2, 13));
	
	// DST tile first pixel offset
	int dst_tile_offset_x_para,
		dst_tile_offset_y_para;
	
	if(dst_tile_offset_x < 0)
		return false;
	else
		dst_tile_offset_x_para = round(dst_tile_offset_x * pow(2, 13));

	if(dst_tile_offset_y < 0)
		return false;
	else
		dst_tile_offset_y_para = round(dst_tile_offset_y * pow(2, 13));

	int dst_width  = ceil(scl_width_m1 * fabs(sra_cos_fp) + scl_height_m1 * fabs(sra_sin_fp)) + 1,
	    dst_height = ceil(scl_width_m1 * fabs(sra_sin_fp) + scl_height_m1 * fabs(sra_cos_fp)) + 1;

	int src_stride, dst_stride;
	switch(transParam->SrcFormat & 7)
	{
	case SRA_FMT_MONO8b:
		src_stride = ceil(transParam->ImageWidth / 4.0);
		break;
	
	case SRA_FMT_RGB565:
		src_stride = ceil(transParam->ImageWidth / 2.0);
		break;
	
	case SRA_FMT_RGB888:
	case SRA_FMT_ARGB8565:
		src_stride = ceil(transParam->ImageWidth * 3.0 / 4);
		break;
	
	case SRA_FMT_XRGB8888:
	case SRA_FMT_ARGB8888:
	case SRA_FMT_XARGB88565:
		src_stride = transParam->ImageWidth;
		break;
	}
	
	switch(transParam->DstFormat & 7)
	{
	case SRA_FMT_MONO8b:
		dst_stride = ceil(transParam->OutputWidth / 4.0);
		dst_width  = ceil(dst_width / 4.0) * 4;
		break;
	
	case SRA_FMT_RGB565:
		dst_stride = ceil(transParam->OutputWidth / 2.0);
		dst_width  = ceil(dst_width / 2.0) * 2;
		break;
	
	case SRA_FMT_RGB888:
	case SRA_FMT_ARGB8565:
		dst_stride = ceil(transParam->OutputWidth * 3.0 / 4);
		dst_width  = dst_width;
		break;
	
	case SRA_FMT_XRGB8888:
	case SRA_FMT_ARGB8888:
	case SRA_FMT_XARGB88565:
		dst_stride = transParam->OutputWidth;
		dst_width  = dst_width;
		break;
	}
	
	SRA->CFG = (transParam->SrcFormat << SRA_CFG_SRCFMT_Pos) |
			   (transParam->DstFormat << SRA_CFG_DSTFMT_Pos) |
			   (mirror			   	  << SRA_CFG_ROTTYP_Pos) |
			   (1				   	  << SRA_CFG_ROTAAEN_Pos) |
			   (1					  << SRA_CFG_DUMMYWB_Pos);
	
	SRA->SINP = (sra_sin_xfact_para << SRA_SINP_SINX_Pos) |
				(sra_sin_yfact_para << SRA_SINP_SINY_Pos);
	SRA->COSP = (sra_cos_xfact_para << SRA_COSP_COSX_Pos) |
				(sra_cos_yfact_para << SRA_COSP_COSY_Pos);
	
	SRA->SRCBASE = transParam->ImageAddr;
	SRA->SRCSIZE = ( src_width_m1    << SRA_SRCSIZE_WIDTH_Pos) |
				   ( src_height_m1   << SRA_SRCSIZE_HEIGHT_Pos);
	SRA->SRCSTRIDE = src_stride;
	
	SRA->DSTBASE = transParam->OutputAddr;
	SRA->DSTSIZE = ((dst_width  - 1) << SRA_DSTSIZE_WIDTH_Pos) |
				   ((dst_height - 1) << SRA_DSTSIZE_HEIGHT_Pos);
	SRA->DSTSTRIDE = dst_stride;
	
	SRA->SRC_TILE_POSX = src_tile_stx_para;
	SRA->SRC_TILE_POSY = src_tile_sty_para;
	SRA->DST_TILE_OFFX = dst_tile_offset_x_para;
	SRA->DST_TILE_OFFY = dst_tile_offset_y_para;
	
	*dstWidth  = dst_width;
	*dstHeight = dst_height;
	
	if(dst_width > transParam->OutputWidth)
		return false;
	
	return true;
}

/*******************************************************************************************************************************
* @brief	start transform
* @param
* @return
*******************************************************************************************************************************/
void SRA_Start(void)
{
	SRA->CR = SRA_CR_START_Msk;
}

/*******************************************************************************************************************************
* @brief	abort transform
* @param
* @return
*******************************************************************************************************************************/
void SRA_Abort(void)
{
	SRA->CR = SRA_CR_ABORT_Msk;
	
	while(SRA->SR & SRA_SR_ABORT_Msk) __NOP();
}

/*******************************************************************************************************************************
* @brief	SRA busy query
* @param
* @return	1: SRA busy for doing transform, 0: SRA idle
*******************************************************************************************************************************/
uint32_t SRA_Busy(void)
{
	return (SRA->SR & SRA_SR_BUSY_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	SRA interrupt enable
* @param	it is interrupt type, can be SRA_IT_FRAME, SRA_IT_8LINE, SRA_IT_ROTERR, SRA_IT_BUSERR and their '|' operation
* @return
*******************************************************************************************************************************/
void SRA_INTEn(uint32_t it)
{
	SRA->IDIS &= ~(it << 4);
}

/*******************************************************************************************************************************
* @brief	SRA interrupt disable
* @param	it is interrupt type, can be SRA_IT_FRAME, SRA_IT_8LINE, SRA_IT_ROTERR, SRA_IT_BUSERR and their '|' operation
* @return
*******************************************************************************************************************************/
void SRA_INTDis(uint32_t it)
{
	SRA->IDIS |= it << 4;
}

/*******************************************************************************************************************************
* @brief	SRA interrupt flag clear
* @param	it is interrupt type, can be SRA_IT_FRAME, SRA_IT_8LINE, SRA_IT_ROTERR, SRA_IT_BUSERR and their '|' operation
* @return
*******************************************************************************************************************************/
void SRA_INTClr(uint32_t it)
{
	SRA->IF = it;
}

/*******************************************************************************************************************************
* @brief	SRA interrupt state query
* @param	it is interrupt type, can be SRA_IT_FRAME, SRA_IT_8LINE, SRA_IT_ROTERR, SRA_IT_BUSERR and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t SRA_INTStat(uint32_t it)
{
	return (SRA->IF & it) ? 1 : 0;
}
