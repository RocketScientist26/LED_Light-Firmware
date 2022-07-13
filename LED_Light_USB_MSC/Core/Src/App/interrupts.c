#include "main.h"
#include "button.h"
#include "animation.h"
#include "w25q.h"
#include "usb.h"

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi){
	if(hspi->Instance == SPI1){
		W25Q_SPI_Error_Interrupt();
	}
}
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi){
	if(hspi->Instance == SPI1){
		W25Q_SPI_Receive_DMA_Interrupt();
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == BUTTON_Pin){
		Button_Interrupt();
	}
	else if(GPIO_Pin == USB_DET_Pin){
		USB_Det_Interrupt();
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
