//Example shows how to set 180MHz frequency with PLL & HSE oscillator.

#include "stm32f4xx.h"
void PLL_HSE_init(void);

int main(void)
{
	int i;
	
	//IO Configuration
	RCC->AHB1ENR |= (1<<0);
	GPIOA->MODER |= (1<<0);
	GPIOA->ODR = 0;
	
	//PLL clocked by HSE
	PLL_HSE_init();
	 	
				
	while(1)
	{
		for(i=0;i<2000000;i++);			//Test
		GPIOA->ODR ^= (1<<0);
	}
	
}

void PLL_HSE_init(void)
{
	FLASH->ACR |= (5<<0);				//Five wait states FLASH memory LATENCY
	RCC->CR |= (1<<16);				//HSE oscillator ON
	while(!(RCC->CR & (1<<17)));			//Wait for HSE READY
	RCC->PLLCFGR = 0x20000000;
	RCC->PLLCFGR |= (360<<6);			//PLLN = 360
	RCC->PLLCFGR |= (8<<0);				//PLLM = 8	
	RCC->PLLCFGR |= (1<<22);			//HSE as PLL clock entry
	RCC->CFGR |= (1<<15);				//APB2 clock divided by 2
	RCC->CFGR |= (1<<12) | (1<<10);			//APB1 clock divided by 4
	RCC->CR |= (1<<24);				//PLL ON
	while(!(RCC->CR & (1<<25)));			//Wait for PLL READY
	RCC->CFGR |= (1<<1);				//PLL selected as system clock
}
