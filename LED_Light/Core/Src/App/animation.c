#include "main.h"
#include "animation.h"

extern TIM_HandleTypeDef htim1;

static uint8_t animation_busy = 0;
static uint8_t animation_mode = ANIMATION_MODE_COLOR;
static uint32_t animation_color = 0;
static uint16_t animation_image = 0;
static uint32_t animation_i = 0;
extern uint8_t animation_data[ANIMATION_DATA_IMAGES][ANIMATION_DATA_IMAGE_SIZE];


void Animation_Timer_Error_Interrupt(){
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
	animation_i = 0;
	animation_busy = 0;
}
void Animation_PWM_Pulse_Interrupt(){
	if(animation_i < ((ANIMATION_DATA_IMAGE_SIZE * 8) + ANIMATION_DATA_STOP_PULSES)){
		if((animation_i + 1) >= ((ANIMATION_DATA_IMAGE_SIZE * 8) + 1)){
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		}else{
			switch(animation_mode){
				case ANIMATION_MODE_COLOR:
					Animation_Set_Bit((uint8_t)(((uint32_t)animation_color >> ((uint32_t)animation_i % 24)) & (uint32_t)1));
				break;
				case ANIMATION_MODE_IMAGE:
					Animation_Set_Bit((animation_data[animation_image][animation_i / 8] >> (animation_i % 8)) & 1);
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
	return animation_busy;
}
void Animation_Set_Bit(uint8_t bit){
	if(bit){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ANIMATION_DATA_BIT_1);
	}else{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ANIMATION_DATA_BIT_0);
	}
}
void Animation_Stop(){
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
	animation_i = 0;
	animation_busy = 0;
}
void Animation_Display_Color(uint32_t color_grb){
	if(ANIMATION_DATA_IMAGE_SIZE){
		uint8_t color_g = (uint8_t)((uint32_t)color_grb >> 16);
		color_g = (color_g & 0xF0) >> 4 | (color_g & 0x0F) << 4;
		color_g = (color_g & 0xCC) >> 2 | (color_g & 0x33) << 2;
		color_g = (color_g & 0xAA) >> 1 | (color_g & 0x55) << 1;
		uint8_t color_r = (uint8_t)((uint32_t)color_grb >> 8);
		color_r = (color_r & 0xF0) >> 4 | (color_r & 0x0F) << 4;
		color_r = (color_r & 0xCC) >> 2 | (color_r & 0x33) << 2;
		color_r = (color_r & 0xAA) >> 1 | (color_r & 0x55) << 1;
		uint8_t color_b = (uint8_t)((uint32_t)color_grb);
		color_b = (color_b & 0xF0) >> 4 | (color_b & 0x0F) << 4;
		color_b = (color_b & 0xCC) >> 2 | (color_b & 0x33) << 2;
		color_b = (color_b & 0xAA) >> 1 | (color_b & 0x55) << 1;

		animation_busy = 1;
		animation_i = 0;
		animation_color = ((uint32_t)color_g <<16) | ((uint32_t)color_r <<8) | (uint32_t)color_b;
		animation_mode = ANIMATION_MODE_COLOR;

		HAL_TIM_PWM_PulseFinishedCallback(&htim1);
		HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
		while(Animation_Busy());
	}
}
void Animation_Display_Image(uint16_t image){
	if((image < ANIMATION_DATA_IMAGES) && ANIMATION_DATA_IMAGE_SIZE){
		animation_busy = 1;
		animation_image = image;
		animation_i = 0;
		animation_mode = ANIMATION_MODE_IMAGE;

		HAL_TIM_PWM_PulseFinishedCallback(&htim1);
		HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
		while(Animation_Busy());
	}
}
void Animation_Init(){
	htim1.Init.Prescaler = ANIMATION_DATA_CLKDIV;
	htim1.Init.Period = ANIMATION_DATA_PULSE;
	if(HAL_TIM_Base_Init(&htim1) != HAL_OK){Error_Handler();}

	Animation_Display_Color(0);
	while(animation_busy);
}
