#include "main.h"
#include "usb_device.h"
#include "usbd_core.h"
#include "usb.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

static uint8_t usb_det_state = 0;
static uint8_t usb_enabled = 1;

void USB_Det_Interrupt(){
	usb_det_state = USB_Det_Debounce_Read();
}

uint8_t USB_Det_Debounce_Read(){
	uint8_t level;
	uint8_t d = 0;
	uint8_t i = 0;
	while(d != 1){
		i = 0;
		level = HAL_GPIO_ReadPin(USB_DET_GPIO_Port, USB_DET_Pin);
		while(i != USB_DET_DEBOUNCE_SAMPLES){
			i++;
			if(HAL_GPIO_ReadPin(USB_DET_GPIO_Port, USB_DET_Pin) != level){
				break;
			}
			if(i == USB_DET_DEBOUNCE_SAMPLES){
				d = 1;
			}
		}
	}
	return level;
}

uint8_t USB_Det_State(){
	return usb_det_state;
}
uint8_t USB_Enabled(){
	return usb_enabled;
}
void USB_Enable(uint8_t enable){
	if(enable){
		if(USBD_Start(&hUsbDeviceFS) != USBD_OK){Error_Handler();}
	}else{
		if(USBD_Stop(&hUsbDeviceFS) != USBD_OK){Error_Handler();}
	}
	usb_enabled = enable;
}
void USB_Init(){
	USB_Det_Interrupt();
	USB_Enable(0);
}