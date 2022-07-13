#include "main.h"
#include "button.h"
#include "app.h"

static uint8_t button_state = 0;

void Button_Interrupt(){
	button_state = Button_Debounce_Read();
}

uint8_t Button_Debounce_Read(){
	uint8_t level;
	uint8_t d = 0;
	uint8_t i = 0;
	while(d != 1){
		i = 0;
		level = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
		while(i != BUTTON_DEBOUNCE_SAMPLES){
			i++;
			if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) != level){
				break;
			}
			if(i == BUTTON_DEBOUNCE_SAMPLES){
				d = 1;
			}
		}
	}
	return level;
}
uint8_t Button_State(){
	return button_state;
}
void Button_Init(){
	Button_Interrupt();
}