#include "main.h"
#include "led.h"

static uint8_t led_state = LED_OFF;

void LED_Set(uint8_t state){
	led_state = state;
	if(state){
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	}
}
uint8_t LED_State(){
	return led_state;
}