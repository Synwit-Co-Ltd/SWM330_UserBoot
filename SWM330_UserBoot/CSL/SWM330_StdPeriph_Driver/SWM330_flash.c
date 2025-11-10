/*******************************************************************************************************************************
* @brief	Flash erase/write driver
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
#include "SWM330_flash.h"


const IAP_Flash_Erase_t IAP_Flash_Erase = (IAP_Flash_Erase_t)0x01000401;
const IAP_Flash_Write_t IAP_Flash_Write = (IAP_Flash_Write_t)0x01000481;
const IAP_Flash_Param_t IAP_Flash_Param = (IAP_Flash_Param_t)0x01000521;
const IAP_Flash_Param_t IAP_Flash_ParamTAC = (IAP_Flash_Param_t)0x01000561;
const IAP_Cache_Config_t IAP_Cache_Config = (IAP_Cache_Config_t)0x01000621;


/*******************************************************************************************************************************
* @brief	Flash sector erase
* @param	addr is address of sector to erase, must be 1KB aligned, i.e. addr % 1024 == 0
* @return	FLASH_RES_OK, FLASH_RES_TO, FLASH_RES_ERR
*******************************************************************************************************************************/
uint32_t FLASH_Erase(uint32_t addr)
{
	if(addr % 1024 != 0) return FLASH_RES_ERR;
	
	__disable_irq();
	
	IAP_Flash_Erase(addr/1024, 0x0B11FFAC);
	
	FLASH_CacheClear();
	
	__enable_irq();
	
	return FLASH_RES_OK;
}


/*******************************************************************************************************************************
* @brief	Flash data write
* @param	addr is address of Flash to write, must be 4-word aligned, i.e. addr % 16 == 0
* @param	buff is the data to write
* @param	count is the number of data to write, in unit of word, and must be a multiple of 4
* @return	FLASH_RES_OK, FLASH_RES_TO, FLASH_RES_ERR
*******************************************************************************************************************************/
uint32_t FLASH_Write(uint32_t addr, uint32_t buff[], uint32_t count)
{
	if(addr % 16 != 0) return FLASH_RES_ERR;
	if(count % 4 != 0) return FLASH_RES_ERR;
	
	__disable_irq();
	
	IAP_Flash_Write(addr, (uint32_t)buff, count/4, 0x0B11FFAC);
	
	FLASH_CacheClear();
	
	__enable_irq();
	
	return FLASH_RES_OK;
}


/*******************************************************************************************************************************
* @brief	set Flash parameter to the parameter required for running at specified MHz.
* @param	xMHz == SystemCoreClock / 1000000
* @return
*******************************************************************************************************************************/
void Flash_Param_at_xMHz(uint32_t xMHz)
{
	__disable_irq();
	
	IAP_Flash_ParamTAC(5, 0x0B11FFAC);
	
	IAP_Flash_Param(1000 / xMHz, 0x0B11FFAC);
	
	if(xMHz < 48)
		IAP_Flash_ParamTAC(0, 0x0B11FFAC);
	else if(xMHz < 76)
		IAP_Flash_ParamTAC(1, 0x0B11FFAC);
	else if(xMHz < 102)
		IAP_Flash_ParamTAC(2, 0x0B11FFAC);
	else if(xMHz < 128)
		IAP_Flash_ParamTAC(3, 0x0B11FFAC);
	else if(xMHz < 152)
		IAP_Flash_ParamTAC(4, 0x0B11FFAC);
	
	__enable_irq();
}
