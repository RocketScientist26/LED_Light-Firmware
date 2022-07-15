#include "animation_data.h"

enum{
	ANIMATION_MODE_COLOR,
	ANIMATION_MODE_IMAGE
};

void Animation_Timer_Error_Interrupt();
void Animation_PWM_Pulse_Interrupt();

uint8_t Animation_Busy();
void Animation_Set_Bit(uint8_t bit);
void Animation_Stop();
void Animation_Display_Color(uint32_t color_grb);
void Animation_Display_Image(uint16_t image);
void Animation_Init();
