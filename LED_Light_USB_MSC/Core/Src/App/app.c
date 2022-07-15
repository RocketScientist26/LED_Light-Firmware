#include "main.h"
#include "app.h"
#include "animation.h"
#include "button.h"
#include "usb.h"
#include "led.h"

#define FRAME_DELAY 250

uint32_t i = 0;

void App_Init(){
	LED_Set(LED_ON);
	USB_Init();
	Button_Init();
	Animation_Init();
	LED_Set(LED_OFF);
}
void App_Loop(){
	if(!USB_Enabled() && USB_Det_State()){
		i = 0;
		Animation_Display_Color(0);
		USB_Enable(1);
	}else if(USB_Enabled() && !USB_Det_State()){
		USB_Enable(0);
		LED_Set(LED_ON);
		Animation_Init();
		LED_Set(LED_OFF);
	}
	
	if(!USB_Enabled()){
		if(!animation_data_config.images || !animation_data_config.image_size){
			LED_Set(1 - LED_State());
			HAL_Delay(300);
		}else{
			if(LED_State()){
				LED_Set(LED_OFF);
			}
			if(!Button_State()){
				if(i == animation_data_config.images){
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
}
