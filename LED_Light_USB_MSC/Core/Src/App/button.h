#define BUTTON_DEBOUNCE_SAMPLES 10

uint8_t Button_Debounce_Read();
void Button_Interrupt();
uint8_t Button_State();
void Button_Init();