#define USB_DET_DEBOUNCE_SAMPLES 10

void USB_Det_Interrupt();
uint8_t USB_Det_Debounce_Read();
uint8_t USB_Det_State();
uint8_t USB_Enabled();
void USB_Enable(uint8_t enable);
void USB_Init();