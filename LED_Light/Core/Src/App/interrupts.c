#include "main.h"
#include "button.h"
#include "animation.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == BUTTON_Pin){
		Button_Interrupt();
	}
}
void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM1){
		Animation_Timer_Error_Interrupt();
	}
}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM1){
		Animation_PWM_Pulse_Interrupt();
	}
}
