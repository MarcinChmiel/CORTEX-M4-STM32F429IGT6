//Example shows how to use PLL with internal oscillator

#include "stm32f4xx.h"
void HSE_init(void);

int main(void)
{
	RCC->AHB1ENR |= (1<<0);
	GPIOA->MODER |= (1<<0);
	GPIOA->ODR = 0;
	int i;
	
	PLL_HSI_init();	
	
	while(1)
	{
		for(i=0;i<2000000;i++);	
		GPIOA->ODR ^= (1<<0);
	}
	
}

void PLL_HSI_init(void)
{
	FLASH->ACR |= (1<<1);				//Two wait states FLASH memory LATENCY
	RCC->CR |= (1<<24);				//PLL ON
	while(!(RCC->CR & (1<<25)));			//Wait for PLL READY
	RCC->CFGR |= (1<<1);				//PLL selected as system clock	
}
