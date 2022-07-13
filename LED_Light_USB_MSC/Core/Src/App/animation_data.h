#ifndef ANIMATION_DATA_H_
#define ANIMATION_DATA_H_

#include "w25q.h"

#define ANIMATION_DATA_BUFFER_SIZE_QUARTER 1024
#define ANIMATION_DATA_BUFFER_SIZE_HALF (ANIMATION_DATA_BUFFER_SIZE_QUARTER * 2)
#define ANIMATION_DATA_BUFFER_SIZE (ANIMATION_DATA_BUFFER_SIZE_QUARTER * 4)
#define ANIMATION_DATA_HEADER_SIZE 19
#define ANIMATION_DATA_MAX_SIZE (W25Q_SECTORS * W25Q_SECTOR_SIZE)

struct animation_data_config_s{
	uint16_t clkdiv;
	uint8_t pulse;
	uint8_t bit_0;
	uint8_t bit_1;
	uint16_t stop_pulses;
	uint32_t images;
	uint32_t image_size;
};

extern struct animation_data_config_s animation_data_config;

void Animation_Data_Prefetch(uint16_t image);
void Animation_Data_Read(uint16_t image);
uint8_t Animation_Data_Get_Bit(uint16_t image, uint32_t byte, uint8_t bit);
void Animation_Data_Init();
void Animation_Data_Buffer();

#endif
