#include "main.h"
#include "stm32f4xx_hal.h"

//_____ External Definitions _______________________________________________________________________
// Pin Mappings
#define Flash_Select()				HAL_GPIO_WritePin(FLASH_nCS_GPIO_Port,  FLASH_nCS_Pin, 	RESET)
#define Flash_Deselect()			HAL_GPIO_WritePin(FLASH_nCS_GPIO_Port,  FLASH_nCS_Pin, 	SET)	
#define Flash_Hold()					HAL_GPIO_WritePin(FLASH_HOLD_GPIO_Port, FLASH_HOLD_Pin, RESET)
#define Flash_Release()				HAL_GPIO_WritePin(FLASH_HOLD_GPIO_Port, FLASH_HOLD_Pin, SET)
#define Flash_WP_Disable()		HAL_GPIO_WritePin(FLASH_nWP_GPIO_Port,  FLASH_nWP_Pin, 	RESET)
#define Flash_WP_Enable()			HAL_GPIO_WritePin(FLASH_nWP_GPIO_Port,  FLASH_nWP_Pin, 	SET)

// Instruction Commands
#define INST_WRITE_STATREG			0x01
#define INST_PAGE_PROGRAM				0x02
#define INST_READ_EN						0x03
#define INST_WRITE_DIS					0x04
#define INST_READ_STATREG_1			0x05
#define INST_WRITE_EN						0x06
#define INST_READ_STATREG_3			0x33


#define INST_READ_STATREG_2			0x35
#define INST_WRITE_EN_VSTATREG	0x50
#define INST_BURSTWRAP_SET			0x77

#define INST_ERASE_SECTOR				0x20
#define INST_ERASE_BLOCK				0xD8
#define INST_ERASE_CHIP					0xC7

//_____ Prototypes _______________________________________________________________________
unsigned char Flash_readLoc(unsigned long address);
void Flash_writeLoc(unsigned long address, unsigned char data);
void Flash_readString(unsigned long address, unsigned char *rxBuffer, unsigned char length);
void Flash_writeString(unsigned long address, unsigned char *string);

unsigned char Flash_readStatusReg (unsigned char reg);
void Flash_writeStatusReg (unsigned char reg, unsigned char data);
void Flash_S25FL_init(SPI_HandleTypeDef *spiconfig);
void Flash_S25FL_Test(void);

void Flash_BulkErase (void);
void Flash_SectorErase (unsigned char index);
void Flash_SubSectorErase (unsigned char index);

void _Flash_writeEN (void);
void _Flash_writeDIS (void);