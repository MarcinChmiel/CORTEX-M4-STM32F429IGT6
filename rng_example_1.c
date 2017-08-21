//Example shows how to set Random Number Generator
//Random Number Generator uses PLL

#include "stm32f4xx.h"
void PLL_HSE_init(void);

int main(void)
{
	//IO Configuration
	RCC->AHB1ENR |= (1<<0);					//GPIOA clock enabled
	GPIOA->MODER |= (1<<0);					//PA0 output
	GPIOA->MODER |= (1<<2);					//PA1 output
	GPIOA->ODR = 0;
	
	
	//Random Number Generator Configuration
	RCC->AHB2ENR |= (1<<6);					//RNG clock enabled
	RNG->CR |= (1<<2);					//Random Number Generator enabled
	
	
	//PLL Configuration
	PLL_HSE_init();


	uint32_t r_number = 0;
	int i;
	
	while(1)
	{
		if(RNG->SR & (1<<1))				//The PLL48CLK was not correctly detected
		GPIOA->ODR |= (1<<1);	
		while(!(RNG->SR & (1<<0)));			//Wait until Data register will contain
								//generated random number
		if(r_number != RNG->DR)				//Check RNG working
		{
		r_number = RNG->DR;
		GPIOA->ODR ^= (1<<0);				//If RNG works correctly switch LED at PA0 pin
		}
		for(i=0; i<5000000;i++);			//Delay
	}
}

void PLL_HSE_init(void)						//With PLL for RNG
{
	FLASH->ACR |= (5<<0);					//Five wait states FLASH memory LATENCY
	RCC->CR |= (1<<16);					//HSE oscillator ON
	while(!(RCC->CR & (1<<17)));				//Wait for HSE READY
	RCC->PLLCFGR = 0x20000000;
	RCC->PLLCFGR |= (8<<24);				//DivF. for RNG clock = 7 ( 360/7 < 48 MHz)
	RCC->PLLCFGR |= (360<<6);				//PLLN = 360
	RCC->PLLCFGR |= (8<<0);					//PLLM = 8	
	RCC->PLLCFGR |= (1<<22);				//HSE as PLL clock entry
	RCC->CFGR |= (1<<15);					//APB2 clock divided by 2
	RCC->CFGR |= (1<<12) | (1<<10);				//APB1 clock divided by 4
	RCC->CR |= (1<<24);					//PLL enabled
	while(!(RCC->CR & (1<<25)));				//Wait for PLL READY
	RCC->CFGR |= (1<<1);					//PLL selected as system clock
}
