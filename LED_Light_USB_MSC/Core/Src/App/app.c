#include "main.h"
#include "app.h"
#include "animation.h"
#include "button.h"
#include "usb.h"
#include "led.h"

///PART OF EXAMPLE ANIMATION FROM HERE
uint32_t example_run = 0;
uint32_t example_current_animation = 0;
uint32_t example_current_animation_played_times = 0;
uint32_t example_current_animation_image = 0;

void Example_Animation_Play(uint32_t ntimes, uint32_t from_image, uint32_t length, uint32_t delay){
	if(example_current_animation_played_times < ntimes){
		if(example_current_animation_image < length){
			Animation_Display_Image(from_image + example_current_animation_image);
			example_current_animation_image++;
			HAL_Delay(delay);
		}else{
			example_current_animation_image = 0;
			example_current_animation_played_times++;
		}
	}else{
		example_current_animation_played_times = 0;
		example_current_animation++;
	}
}
///PART OF EXAMPLE ANIMATION TO HERE

void App_Loop(){
	if(!USB_Enabled() && USB_Det_State()){
		///PART OF EXAMPLE ANIMATION FROM HERE
		example_run = 0;
		example_current_animation = 0;
		example_current_animation_played_times = 0;
		example_current_animation_image = 0;
		///PART OF EXAMPLE ANIMATION TO HERE

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

			///PART OF EXAMPLE ANIMATION FROM HERE
			if(!Button_State()){
				//If button pressed, allow animation start or stop
				example_run = 1 - example_run;
				//If animation should be stopped, reset frame counter and display black color (all LEDs off)
				if(!example_run){
					example_run = 0;
					example_current_animation = 0;
					example_current_animation_played_times = 0;
					example_current_animation_image = 0;
					Animation_Display_Color(0);
				}
				//Wait until button released
				while(!Button_State());
				//Prevent bouncing
				HAL_Delay(200);
			}

			//If animation should be played and if we have images required for running this example (35)
			if(example_run && (animation_data_config.images == 35)){

				//In this example we have 6 different animations with different frame rates and play times
				switch(example_current_animation){
					case 0:
						//First animation has 8 frames and starts from 0th frame, we play it 8 times
						Example_Animation_Play(8, 0, 8, 50);
					break;
					case 1:
						//Second animation starts from 8th frame and also has 8 frames total
						Example_Animation_Play(8, 8, 8, 50);
					break;
					case 2:
						//Third animation starts from 16th frame and has 2 frames, we play it 4 times
						Example_Animation_Play(4, 16, 2, 1000);
					break;
					case 3:
						//Fourth animation starts from 18th frame and has 2 frames, we play it 2 times
						Example_Animation_Play(2, 18, 2, 1000);
					break;
					case 4:
						//Fifth animation starts from 20th frame and has 11 frames, we play it 1 times
						Example_Animation_Play(1, 20, 11, 50);
						//Stay still for some time after heart animation
						if(example_current_animation == 5){
							HAL_Delay(1000);
						}
					break;
					case 5:
						//Sixth animation starts from 31th frame and has 4 frames, we play it 6 times
						Example_Animation_Play(6, 31, 4, 250);
					break;
					case 6:
						//All animations played, start over
						example_current_animation = 0;
					break;

				}
			}
			///PART OF EXAMPLE ANIMATION TO HERE
		}
	}
}
void App_Init(){
	LED_Set(LED_ON);
	USB_Init();
	Button_Init();
	Animation_Init();
	LED_Set(LED_OFF);
}
