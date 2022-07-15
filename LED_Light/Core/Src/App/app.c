#include "main.h"
#include "app.h"
#include "animation.h"
#include "button.h"
#include "led.h"

#define FRAME_DELAY 250

uint32_t i = 0;

void App_Init(){
	LED_Set(LED_ON);
	Button_Init();
	Animation_Init();
	LED_Set(LED_OFF);
}
void App_Loop(){
	if(!ANIMATION_DATA_IMAGES || !ANIMATION_DATA_IMAGE_SIZE){
		LED_Set(1 - LED_State());
		HAL_Delay(300);
	}else{
		if(LED_State()){
			LED_Set(LED_OFF);
		}
		if(!Button_State()){
			if(i == ANIMATION_DATA_IMAGES){
				i = 0;
			}
			Animation_Display_Image(i);
			HAL_Delay(FRAME_DELAY);
			i++;
		}else{
			i = 0;
			Animation_Display_Color(0);
			HAL_Delay(100);
		}
	}
}
