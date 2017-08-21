//Example shows how to change from internal oscillator to external


#include "stm32f4xx.h"
void HSE_init(void);

int main(void)
{
	RCC->AHB1ENR |= (1<<0);
	GPIOA->MODER |= (1<<0);
	GPIOA->ODR = 0;
	
	HSE_init();
	
	while(1)
	{
		if((RCC->CFGR & (1<<2)))		//Check which oscillator is selected as system clock
			GPIOA->ODR |= (1<<0);
	}
	
}

void HSE_init(void)
{
	RCC->CR |= (1<<16);				//HSE oscillator ON
	while(!(RCC->CR & (1<<17)));			//Wait for HSE READY
	RCC->CFGR |= (1<<0);				//Select HSE as system clock
	RCC->CR &= ~(1<<0);				//HSI oscillator OFF
}
