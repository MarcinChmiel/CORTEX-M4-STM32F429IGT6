//Example shows how to switch PA0 state with SysTick Timer

#include "stm32f4xx.h"

int main(void)
{
	RCC->AHB1ENR |= (1<<0);				//IO port A clock enabled
	GPIOA->MODER |= (1<<0);				//Set PA0 as output
	GPIOA->ODR &= ~(1<<0);				//Set PA0 as HIGH
	SysTick_Config(16000000);			//SysTick Timer Init
	while(1)
	{
		
	}
}

void SysTick_Handler(void)
{
	GPIOA->ODR ^= (1<<0);				//Change PA0 state
}
