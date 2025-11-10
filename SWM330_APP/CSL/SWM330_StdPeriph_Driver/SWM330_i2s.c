/****************************************************************************************************************************************** 
* 文件名称:	SWM350_i2c.c
* 功能说明:	SWM350单片机的I2C串行接口功能驱动库
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期:	V1.0.0		2016年1月30日
* 升级记录:  
*
*
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIES AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIEE. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIES ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology
*******************************************************************************************************************************************/
#include "SWM330.h"
#include "SWM330_i2s.h"


/****************************************************************************************************************************************** 
* 函数名称:	I2S_Init()
* 功能说明:	I2S音频串行接口初始化，包括帧格式、数据长度、时钟频率、中断设定
* 输    入: I2S_TypeDef * I2Sx		指定要被设置的I2S，有效值包括I2S0、I2S1
*			I2S_InitStructure * initStruct	包含I2S相关设定值的结构体
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void I2S_Init(I2S_TypeDef * I2Sx, I2S_InitStructure * initStruct)
{
	switch((uint32_t)I2Sx)
	{
	case ((uint32_t)I2S0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_I2S0_Pos);
		break;
	}
	
	I2S_Close(I2Sx);	//一些关键寄存器只能在I2S关闭时设置
	
	I2Sx->CR1 = (0 << I2S_CR1_NSYNC_Pos) |
				(0 << I2S_CR1_FILTE_Pos);
	
	I2Sx->CR2 = (initStruct->Mode						  << I2S_CR2_MODE_Pos)  |
				(initStruct->ClkPolarity				  << I2S_CR2_CKPOL_Pos) |
				((initStruct->FrameFormat & 0x03)  		  << I2S_CR2_FFMT_Pos)  |
				(initStruct->DataLen               		  << I2S_CR2_DLEN_Pos)  |
				(initStruct->ChannelLen 			  	  << I2S_CR2_CHLEN_Pos) |
				((initStruct->FrameFormat & 0x04 ? 1 : 0) << I2S_CR2_PCMSYNW_Pos);
	
	I2Sx->CR3 = (SystemCoreClock / initStruct->ClkFreq / 2 - 1) << I2S_CR3_SCLKDIV_Pos;
	
	I2S_INTClr(I2Sx, initStruct->INTEn);
	I2S_INTEn(I2Sx, initStruct->INTEn);
	
	if(initStruct->INTEn)
	{
		switch((uint32_t)I2Sx)
		{
		case ((uint32_t)I2S0):  NVIC_EnableIRQ(I2S0_IRQn);	break;
		}
	}
}

/****************************************************************************************************************************************** 
* 函数名称:	I2S_Open()
* 功能说明:	I2S打开，允许收发
* 输    入: I2S_TypeDef * I2Sx		指定要被设置的I2S，有效值包括I2S0、I2S1
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void I2S_Open(I2S_TypeDef * I2Sx)
{
	I2Sx->CR2 |= I2S_CR2_EN_Msk;
}

/****************************************************************************************************************************************** 
* 函数名称:	I2S_Close()
* 功能说明:	I2S关闭，禁止收发
* 输    入: I2S_TypeDef * I2Sx		指定要被设置的I2S，有效值包括I2S0、I2S1
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void I2S_Close(I2S_TypeDef * I2Sx)
{
	I2Sx->CR2 &= ~I2S_CR2_EN_Msk;
}


/****************************************************************************************************************************************** 
* 函数名称:	I2S_INTEn()
* 功能说明:	中断使能
* 输    入: I2S_TypeDef * I2Sx		指定要被设置的I2S，有效值包括I2S0、I2S1
* 			uint32_t it				interrupt type，有效值I2S_IT_SLV_TX_UNR、I2S_IT_RX_OVR、I2S_IT_RX_NOT_EMPTY、I2S_IT_TX_EMPTY 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void I2S_INTEn(I2S_TypeDef * I2Sx, uint32_t it)
{
	I2Sx->IE |= it;
}

/****************************************************************************************************************************************** 
* 函数名称:	I2S_INTDis()
* 功能说明:	中断禁止
* 输    入: I2S_TypeDef * I2Sx		指定要被设置的I2S，有效值包括I2S0、I2S1
* 			uint32_t it				interrupt type，有效值I2S_IT_SLV_TX_UNR、I2S_IT_RX_OVR、I2S_IT_RX_NOT_EMPTY、I2S_IT_TX_EMPTY 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void I2S_INTDis(I2S_TypeDef * I2Sx, uint32_t it)
{
	I2Sx->IE &= ~it;
}

/****************************************************************************************************************************************** 
* 函数名称:	I2S_INTClr()
* 功能说明:	中断标志清除
* 输    入: I2S_TypeDef * I2Sx		指定要被设置的I2S，有效值包括I2S0、I2S1
* 			uint32_t it				interrupt type，有效值I2S_IT_SLV_TX_UNR、I2S_IT_RX_OVR、I2S_IT_RX_NOT_EMPTY、I2S_IT_TX_EMPTY 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void I2S_INTClr(I2S_TypeDef * I2Sx, uint32_t it)
{
	I2Sx->SR = it;
}

/****************************************************************************************************************************************** 
* 函数名称:	I2S_INTStat()
* 功能说明:	中断状态查询
* 输    入: I2S_TypeDef * I2Sx		指定要被设置的I2S，有效值包括I2S0、I2S1
* 			uint32_t it				interrupt type，有效值I2S_IT_SLV_TX_UNR、I2S_IT_RX_OVR、I2S_IT_RX_NOT_EMPTY、I2S_IT_TX_EMPTY 及其“或”
* 输    出: bool					true 中断发生    false 中断未发生	
* 注意事项: 无
******************************************************************************************************************************************/
bool I2S_INTStat(I2S_TypeDef * I2Sx, uint32_t it)
{
	return I2Sx->SR & it;
}
