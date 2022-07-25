#include "main.h"
#include "animation_data.h"

extern CRC_HandleTypeDef hcrc;

struct animation_data_config_s animation_data_config;

static uint8_t animation_data[ANIMATION_DATA_BUFFER_SIZE];
static uint32_t animation_data_buffered = 0;
static uint8_t animation_data_buffering_rq = 0;
static uint16_t animation_data_buffering_image = 0;

void Animation_Data_Prefetch(uint16_t image){
	uint32_t data_size = 0;
	if(animation_data_config.image_size < ANIMATION_DATA_BUFFER_SIZE){
		data_size = animation_data_config.image_size;
	}else{
		data_size = ANIMATION_DATA_BUFFER_SIZE;
	}
	W25Q_Read(animation_data, ANIMATION_DATA_HEADER_SIZE + (animation_data_config.image_size * image), data_size);
	animation_data_buffered = data_size;
	animation_data_buffering_rq = 0;
}
void Animation_Data_Read(uint16_t image){
	uint32_t data_size = 0;
	if((animation_data_config.image_size - animation_data_buffered) < ANIMATION_DATA_BUFFER_SIZE_HALF){
		data_size = animation_data_config.image_size - animation_data_buffered;
	}else{
		data_size = ANIMATION_DATA_BUFFER_SIZE_HALF;
	}

	if(!(animation_data_buffered % ANIMATION_DATA_BUFFER_SIZE)){
		//If requested byte 0 of buffer
		W25Q_Read_DMA(&animation_data[0], data_size, ANIMATION_DATA_HEADER_SIZE + (animation_data_config.image_size * image) + animation_data_buffered);
	}else{
		//If requested middle right byte of buffer
		W25Q_Read_DMA(&animation_data[ANIMATION_DATA_BUFFER_SIZE_HALF], data_size, ANIMATION_DATA_HEADER_SIZE + (animation_data_config.image_size * image) + animation_data_buffered);
	}

	animation_data_buffered += data_size;
}
uint8_t Animation_Data_Get_Bit(uint16_t image, uint32_t byte, uint8_t bit){
	if(byte && !(byte % ANIMATION_DATA_BUFFER_SIZE_HALF) && !bit){
		//If requested first bit of first byte from first or second half of buffer
		if(animation_data_buffered < animation_data_config.image_size){
			animation_data_buffering_image = image;
			animation_data_buffering_rq = 1;
		}
	}

	return (animation_data[byte % ANIMATION_DATA_BUFFER_SIZE] >> bit) & 1;
}
void Animation_Data_Init(){
	uint8_t data_error = 0;
	//Check size
	uint32_t data_size = 0;
	W25Q_Read((uint8_t *)&data_size, 0, 4);
	if((data_size > ANIMATION_DATA_HEADER_SIZE) && (data_size <= ANIMATION_DATA_MAX_SIZE)){
		//Read CRC
		uint32_t data_crc = 0;
		W25Q_Read((uint8_t *)&data_crc, data_size - 4, 4);

		//Subtract CRC length
		data_size -= 4;
		//Calculate actual CRC
		uint32_t i = 0;
		uint32_t data_crc_actual = 0;
		if(data_size < ANIMATION_DATA_BUFFER_SIZE){
			W25Q_Read(&animation_data[0], 0, data_size);
			data_crc_actual = HAL_CRC_Calculate(&hcrc, (uint32_t *)&animation_data[0], data_size / 4);
			i += data_size;
		}else{
			W25Q_Read(&animation_data[0], i, ANIMATION_DATA_BUFFER_SIZE);
			data_crc_actual = HAL_CRC_Calculate(&hcrc, (uint32_t *)&animation_data[0], ANIMATION_DATA_BUFFER_SIZE / 4);
			i += ANIMATION_DATA_BUFFER_SIZE;
		}
		while(i != data_size){
			if((data_size - i) < ANIMATION_DATA_BUFFER_SIZE){
				W25Q_Read(&animation_data[0], i, data_size - i);
				data_crc_actual = HAL_CRC_Accumulate(&hcrc, (uint32_t *)&animation_data[0], (data_size - i) / 4);
				i += data_size - i;
			}else{
				W25Q_Read(&animation_data[0], i, ANIMATION_DATA_BUFFER_SIZE);
				data_crc_actual = HAL_CRC_Accumulate(&hcrc, (uint32_t *)&animation_data[0], ANIMATION_DATA_BUFFER_SIZE / 4);
				i += ANIMATION_DATA_BUFFER_SIZE;
			}
		}

		//Check CRC
		if(data_crc_actual == data_crc){
			//Read animation data configuration
			uint32_t data = 0;
			W25Q_Read((uint8_t *)&data, 4, 2);
			animation_data_config.clkdiv = *(uint16_t *)&data;
			W25Q_Read((uint8_t *)&data, 6, 1);
			animation_data_config.pulse = *(uint8_t *)&data;
			W25Q_Read((uint8_t *)&data, 7, 1);
			animation_data_config.bit_0 = *(uint8_t *)&data;
			W25Q_Read((uint8_t *)&data, 8, 1);
			animation_data_config.bit_1 = *(uint8_t *)&data;
			W25Q_Read((uint8_t *)&data, 9, 2);
			animation_data_config.stop_pulses = *(uint16_t *)&data;
			W25Q_Read((uint8_t *)&data, 11, 4);
			animation_data_config.images = *(uint32_t *)&data;
			W25Q_Read((uint8_t *)&data, 15, 4);
			animation_data_config.image_size = *(uint32_t *)&data;
		}else{
			data_error = 1;
		}
	}else{
		data_error = 1;
	}

	//Reading correct data failed
	if(data_error){
		animation_data_config.clkdiv = 0;
		animation_data_config.pulse = 0;
		animation_data_config.bit_0 = 0;
		animation_data_config.bit_1 = 0;
		animation_data_config.stop_pulses = 0;
		animation_data_config.images = 0;
		animation_data_config.image_size = 0;
	}
}

void Animation_Data_Buffer(){
	if(animation_data_buffering_rq){
		animation_data_buffering_rq = 0;
		Animation_Data_Read(animation_data_buffering_image);
	}
}
