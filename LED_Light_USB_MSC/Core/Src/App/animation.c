#include "main.h"
#include "animation.h"

extern TIM_HandleTypeDef htim1;

static uint8_t animation_busy = 0;
static uint8_t animation_mode = ANIMATION_MODE_COLOR;
static uint32_t animation_color = 0;
static uint16_t animation_image = 0;
static uint32_t animation_i = 0;

void Animation_Timer_Error_Interrupt(){
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
	animation_i = 0;
	animation_busy = 0;
}
void Animation_PWM_Pulse_Interrupt(){
	if(animation_i < ((animation_data_config.image_size * 8) + animation_data_config.stop_pulses)){
		if((animation_i + 1) >= ((animation_data_config.image_size * 8) + 1)){
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		}else{
			switch(animation_mode){
				case ANIMATION_MODE_COLOR:
					Animation_Set_Bit((uint8_t)(((uint32_t)animation_color >> ((uint32_t)animation_i % 24)) & (uint32_t)1));
				break;
				case ANIMATION_MODE_IMAGE:
					Animation_Set_Bit(Animation_Data_Get_Bit(animation_image, animation_i / 8, animation_i % 8));
				break;
			}
		}
		animation_i++;
	}else{
		HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
		animation_busy = 0;
	}
}

uint8_t Animation_Busy(){
	Animation_Data_Buffer();
	return animation_busy;
}
void Animation_Set_Bit(uint8_t bit){
	if(bit){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, animation_data_config.bit_1);
	}else{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, animation_data_config.bit_0);
	}
}
void Animation_Stop(){
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
	animation_i = 0;
	animation_busy = 0;
}
void Animation_Display_Color(uint32_t color){
	if(animation_data_config.image_size){
		uint8_t color_a = (uint8_t)((uint32_t)color >> 16);
		color_a = (color_a & 0xF0) >> 4 | (color_a & 0x0F) << 4;
		color_a = (color_a & 0xCC) >> 2 | (color_a & 0x33) << 2;
		color_a = (color_a & 0xAA) >> 1 | (color_a & 0x55) << 1;
		uint8_t color_b = (uint8_t)((uint32_t)color >> 8);
		color_b = (color_b & 0xF0) >> 4 | (color_b & 0x0F) << 4;
		color_b = (color_b & 0xCC) >> 2 | (color_b & 0x33) << 2;
		color_b = (color_b & 0xAA) >> 1 | (color_b & 0x55) << 1;
		uint8_t color_c = (uint8_t)((uint32_t)color);
		color_c = (color_c & 0xF0) >> 4 | (color_c & 0x0F) << 4;
		color_c = (color_c & 0xCC) >> 2 | (color_c & 0x33) << 2;
		color_c = (color_c & 0xAA) >> 1 | (color_c & 0x55) << 1;

		animation_busy = 1;
		animation_i = 0;
		animation_color = ((uint32_t)color_a <<16) | ((uint32_t)color_b <<8) | (uint32_t)color_c;
		animation_mode = ANIMATION_MODE_COLOR;

		HAL_TIM_PWM_PulseFinishedCallback(&htim1);
		HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
		while(Animation_Busy());
	}
}
void Animation_Display_Image(uint16_t image){
	if((image < animation_data_config.images) && animation_data_config.image_size){
		animation_busy = 1;
		animation_image = image;
		animation_i = 0;
		animation_mode = ANIMATION_MODE_IMAGE;

		Animation_Data_Prefetch(image);
		HAL_TIM_PWM_PulseFinishedCallback(&htim1);
		HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
		while(Animation_Busy());
	}
}
void Animation_Init(){
	Animation_Data_Init();

	if(animation_data_config.images && animation_data_config.image_size){
		htim1.Init.Prescaler = animation_data_config.clkdiv;
		htim1.Init.Period = animation_data_config.pulse;
		if(HAL_TIM_Base_Init(&htim1) != HAL_OK){Error_Handler();}
		Animation_Display_Color(0);
	}
}
