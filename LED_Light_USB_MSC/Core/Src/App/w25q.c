#include "main.h"
#include "w25q.h"

extern SPI_HandleTypeDef hspi1;
uint8_t w25q_dma_busy = 0;

//Hardware
void W25Q_SPI_Error_Interrupt(){
	W25Q_Set_CS(1);
	w25q_dma_busy = 0;
}
void W25Q_SPI_Receive_DMA_Interrupt(){
	W25Q_Set_CS(1);
	w25q_dma_busy = 0;
}
void W25Q_SPI_Receive_DMA(uint8_t *data, uint16_t length){
	HAL_SPI_Receive_DMA(&hspi1, data, length);
}
void W25Q_SPI_Receive(uint8_t *data, uint16_t length){
	HAL_SPI_Receive(&hspi1, data, length, W25Q_BLOCKING_TIMEOUT);
}
void W25Q_SPI_Transmit(uint8_t *data, uint16_t length){
	HAL_SPI_Transmit(&hspi1, data, length, W25Q_BLOCKING_TIMEOUT);
}
void W25Q_Set_CS(uint8_t cs){
	if(!cs){
		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
	}
}

//USB Side Commands
void W25Q_Write_Enable(uint8_t enable){
	W25Q_Set_CS(0);
	uint8_t cmd = W25Q_CMD_WRIRE_ENABLE;
	if(!enable){
		cmd = W25Q_CMD_WRITE_DISABLE;
	}
	W25Q_SPI_Transmit(&cmd, 1);
	W25Q_Set_CS(1);
}
uint8_t W25Q_Busy(){
	W25Q_Set_CS(0);
	uint8_t tmp = W25Q_CMD_READ_STATUS_R1;
	W25Q_SPI_Transmit(&tmp, 1);
	W25Q_SPI_Receive(&tmp, 1);
	W25Q_Set_CS(1);
	return (tmp & 1);
}
void W25Q_Erase_Sector(uint32_t sector){
	while(W25Q_Busy());
	W25Q_Write_Enable(1);
	while(W25Q_Busy());
	W25Q_Set_CS(0);
	uint8_t cmd = W25Q_CMD_SECTOR_ERASE;
	W25Q_SPI_Transmit(&cmd, 1);
	uint32_t address = sector * W25Q_SECTOR_SIZE;
	W25Q_SPI_Transmit((uint8_t *)&address, 3);
	W25Q_Set_CS(1);
	while(W25Q_Busy());
}
void W25Q_Write_Sector(uint8_t* data, uint16_t sector){
	uint8_t cmd = W25Q_CMD_PAGE_PROGRAM;
	uint32_t address = sector * W25Q_SECTOR_SIZE;
	uint32_t i = 0;
	while(i != W25Q_SECTOR_SIZE / W25Q_PAGE_SIZE){
		while(W25Q_Busy());
		W25Q_Write_Enable(1);
		while(W25Q_Busy());
		W25Q_Set_CS(0);
		W25Q_SPI_Transmit(&cmd, 1);
		W25Q_SPI_Transmit((uint8_t *)&address, 3);
		W25Q_SPI_Transmit(&data[W25Q_PAGE_SIZE * i], W25Q_PAGE_SIZE);
		W25Q_Set_CS(1);
		address += W25Q_PAGE_SIZE;
		i++;
	}
	while(W25Q_Busy());
}
void W25Q_Read_Sector(uint8_t *data, uint16_t sector){
	while(W25Q_Busy());
	W25Q_Set_CS(0);
	uint8_t cmd = W25Q_CMD_FAST_READ;
	W25Q_SPI_Transmit(&cmd, 1);

	uint32_t address = sector * W25Q_SECTOR_SIZE;
	address = ((address >> 16) & 0xff) | (address & 0xff00) | ((address << 16) & 0xff0000);
	W25Q_SPI_Transmit((uint8_t *)&address, 4);

	W25Q_SPI_Receive(data, W25Q_SECTOR_SIZE);
 	W25Q_Set_CS(1);
}
//Debug
void W25Q_Read_Dev_Info(uint8_t *info){
	W25Q_Set_CS(0);
	uint8_t cmd = W25Q_CMD_JEDEC_ID;
	W25Q_SPI_Transmit(&cmd, 1);
	W25Q_SPI_Receive(info, 3);
	W25Q_Set_CS(1);
}
void W25Q_Chip_Erase(){
	while(W25Q_Busy());
	W25Q_Write_Enable(1);
	while(W25Q_Busy());
	W25Q_Set_CS(0);
	uint8_t cmd = W25Q_CMD_CHIP_ERASE;
	W25Q_SPI_Transmit(&cmd, 1);
	W25Q_Set_CS(1);
	while(W25Q_Busy());
}

//App
void W25Q_Read_DMA(uint8_t *data, uint16_t length, uint32_t address){
	while(w25q_dma_busy);
	w25q_dma_busy = 1;
	W25Q_Set_CS(0);
	uint8_t cmd = W25Q_CMD_FAST_READ;
	W25Q_SPI_Transmit(&cmd, 1);

	address = ((address >> 16) & 0xff) | (address & 0xff00) | ((address << 16) & 0xff0000);
	W25Q_SPI_Transmit((uint8_t *)&address, 4);

	W25Q_SPI_Receive_DMA(data, length);
}
void W25Q_Read(uint8_t *data, uint32_t address, uint16_t length){
	while(W25Q_Busy());
	W25Q_Set_CS(0);
	uint8_t cmd = W25Q_CMD_FAST_READ;
	W25Q_SPI_Transmit(&cmd, 1);

	address = ((address >> 16) & 0xff) | (address & 0xff00) | ((address << 16) & 0xff0000);
	W25Q_SPI_Transmit((uint8_t *)&address, 4);

	W25Q_SPI_Receive(data, length);
 	W25Q_Set_CS(1);
}
