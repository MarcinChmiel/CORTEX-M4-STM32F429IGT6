//Example shows how to program microswitch
//Every time You press BUTTON You switch LED mode (using 1 bit)
//PA0 - LED
//PA1 - BUTTON

#include "stm32f4xx.h"

volatile uint8_t LED_mode = 0;

int main(void)
{
	//SysTick Timer Configuration
	SysTick_Config(SystemCoreClock/4);				//Interrupt every 250ms
	
	
	//Pin Configuration
	RCC->AHB1ENR |= (1<<0);						//GPIOA Clock Enable
	GPIOA->MODER |= (1<<0);						//PA0 as output ( LED )
	GPIOA->ODR = 0;							//GPIOA initial value

	
	uint16_t last_button_state = 0;
	int i;
	
	while(1)
	{
		uint16_t button_state = (GPIOA->IDR & (1<<1));
		if(button_state != last_button_state)
		{
			for(i=0; i<800000; i++);			//Delay for debouncing
			if(button_state)
				LED_mode ^= (1<<0);
		}
		last_button_state = button_state;
	}
}

void SysTick_Handler(void)
{
	switch(LED_mode & (1<<0))
	{
		case 0:
						GPIOA->ODR |= (1<<0);	//Turn on LED
						break;
		case 1:
						GPIOA->ODR ^= (1<<0);	//Switch LED
						break;
	}
}
