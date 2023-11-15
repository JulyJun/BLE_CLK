/*
 * flash.c
 *
 *  Created on: Nov 7, 2023
 *      Author: 박병준
 */


#include "flash.h"

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
uint32_t FirstSector, NbOfSectors;

void initFlash(target_flashRange_t* target, uint32_t startAddr, uint32_t endAddr)
{
	target->USER_START_ADDR = startAddr;
	target->USER_END_ADDR = endAddr;
	target->USER_TARGET_ADDR = target->USER_START_ADDR;
}

uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
      sector = FLASH_SECTOR_0;
    }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
      sector = FLASH_SECTOR_1;
    }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
      sector = FLASH_SECTOR_2;
    }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
      sector = FLASH_SECTOR_3;
    }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
      sector = FLASH_SECTOR_4;
    }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
      sector = FLASH_SECTOR_5;
    }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
      sector = FLASH_SECTOR_6;
    }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
      sector = FLASH_SECTOR_7;
    }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
      sector = FLASH_SECTOR_8;
    }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
      sector = FLASH_SECTOR_9;
    }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
      sector = FLASH_SECTOR_10;
    }
  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
    {
      sector = FLASH_SECTOR_11;
    }
  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
    {
      sector = FLASH_SECTOR_12;
    }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
    {
      sector = FLASH_SECTOR_13;
    }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
    {
      sector = FLASH_SECTOR_14;
    }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
    {
      sector = FLASH_SECTOR_15;
    }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
    {
      sector = FLASH_SECTOR_16;
    }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
    {
      sector = FLASH_SECTOR_17;
    }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
    {
      sector = FLASH_SECTOR_18;
    }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
    {
      sector = FLASH_SECTOR_19;
    }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
    {
      sector = FLASH_SECTOR_20;
    }
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
    {
      sector = FLASH_SECTOR_21;
    }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
    {
      sector = FLASH_SECTOR_22;
    }
  else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23) */
    {
      sector = FLASH_SECTOR_23;
    }
  return sector;
}

/**
 * @brief  Gets sector Size
 * @param  None
 * @retval The size of a given sector
 */
uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;
  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) ||\
      (Sector == FLASH_SECTOR_3) || (Sector == FLASH_SECTOR_12) || (Sector == FLASH_SECTOR_13) ||\
      (Sector == FLASH_SECTOR_14) || (Sector == FLASH_SECTOR_15))
    {
      sectorsize = 16 * 1024;
    }
  else if((Sector == FLASH_SECTOR_4) || (Sector == FLASH_SECTOR_16))
    {
      sectorsize = 64 * 1024;
    }
  else
    {
      sectorsize = 128 * 1024;
    }
  return sectorsize;
}

Status_flashRW readFlash(uint32_t StartADDR)
{
  unsigned int value = *(unsigned int*)StartADDR;
  printf("addr[0x%08x] = %08x\r\n", StartADDR, value);
  return RW_OK;
}

Status_flashRW overwriteFlash(target_flashRange_t* target, uint32_t DATA)
{
	HAL_FLASH_Unlock();
	uint32_t Address = target->USER_TARGET_ADDR;
	if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA) == HAL_OK)
	{
		printf("Changed: ");
		readFlash(Address);
	}
	else
	{
		return RW_ERROR;
	}

	HAL_FLASH_Lock();
	return RW_OK;
}

Status_flashRW eraseFlash(target_flashRange_t* target)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  //uint32_t Address = 0;
  uint32_t PageError;

  HAL_FLASH_Unlock();
  FirstSector = GetSector(target->USER_START_ADDR);
  NbOfSectors = GetSector(target->USER_END_ADDR) - FirstSector + 1;
  EraseInitStruct.TypeErase 		= FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector        		= FirstSector;
  EraseInitStruct.NbSectors     	= NbOfSectors;
  HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
#if 0
  Address = target->USER_START_ADDR;

  while(Address < target->USER_END_ADDR)
    {
      if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
      {
    	  printf("Changed: ");
    	  readFlash(Address);
    	  Address = Address + 4;
      }
      else
      {
    	  printf("error occurred");
    	  return RW_ERROR;
      }
    }
#endif


  HAL_FLASH_Lock();
  return RW_OK;
}
