//Timer TIM4 - Count Up Mode - Realisation of delay_ms() function with LED blinking

#include "stm32f4xx.h"

void delay_ms(uint16_t);
volatile uint16_t counter = 0;

int main(void)
{
	//Pin configuration
	RCC->AHB1ENR |= (1<<0);					//GPIOA Clock Enable
	GPIOA->MODER |= (1<<0);					//GPIOA PA0 as output
	GPIOA->ODR = 0;						//GPIOA initial value 0
	
	
	//Timer Configuration
	RCC->APB1ENR |= (1<<2);					//Timer4 Clock Enable
	TIM4->CR1 |= (1<<7);					//TIM4_ARR register is buffered
	TIM4->CR1 |= (1<<2);					//Only counter overflow generates an interrupt
	TIM4->DIER |= (1<<0);					//Update interrupt enabled
	TIM4->PSC = 999;					//Prescaler 1000
	TIM4->ARR = 15;						//ARR value 16
								//16*1000=16k, 16k/16M = 1ms
	TIM4->CR1 |= (1<<0);					//Counter Enable
	
	
	//Interrupt enable
	NVIC_EnableIRQ(TIM4_IRQn);
	
	while(1)
	{
		delay_ms(1000);
		GPIOA->ODR ^= (1<<0);				//Change PA0 state
	}
}
void TIM4_IRQHandler(void)
{
	TIM4->SR &= ~(1<<0);					//Clear interrupt flag
	counter++;
}

void delay_ms(uint16_t x)
{
	counter = 0;
	while(1)
	{
		if(counter == x)
			break;
	}
}
