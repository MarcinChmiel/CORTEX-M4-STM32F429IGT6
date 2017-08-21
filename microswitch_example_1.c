//Example shows how to program microswitch
//Every time You press BUTTON You switch LED
//PA0 - LED
//PA1 - BUTTON 

#include "stm32f4xx.h"

int main(void)
{

	//Pin Configuration
	RCC->AHB1ENR |= (1<<0);						//PORTA Clock Enable
	GPIOA->MODER |= (1<<0);						//PA0 as Output
	GPIOA->ODR = 0;							//PORTA initial value
	
	uint16_t last_button_state = 0;
	int i;
	
	
	while(1)
	{
		uint16_t button_state = (GPIOA->IDR & (1<<1));
		if(button_state != last_button_state)
		{
			for(i=0; i<800000; i++);			//Delay for debouncing
			if(button_state)
			{
				GPIOA->ODR ^= (1<<0);			//SWITCH LED state
			}
		}
		last_button_state = button_state;
	}
}
