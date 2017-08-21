//Timer TIM7 -	Counter Mode - LED blnking every 1s
//PA0 - LED


#include "stm32f4xx.h"

int main(void)
{
	//IO Init
	RCC->AHB1ENR |= (1<<0);				//GPIOA Clock Enable
	GPIOA->MODER |= (1<<0);				//Set PA0 as output
	GPIOA->ODR = 0;					//Set PORTA initial value
	
	
	//Timer TIM7 Init
	RCC->APB1ENR |= (1<<5);				//TIM7 Clock Enable
	TIM7->CR1 |= (1<<7);				//TIMx_ARR register is buffered
	TIM7->CR1 |= (1<<2);				//Only Overflow generates update interrupt
	TIM7->PSC = 999;				//Prescaler value = PSC + 1;
	TIM7->ARR = 16000;				//ARR * (PSC+1) = 16M = 1s
	TIM7->DIER |= (1<<0);				//Update interrupt enabled
	TIM7->CR1 |= (1<<0);				//Counter enabled
	
	//Interrupt Enable
	NVIC_EnableIRQ(TIM7_IRQn);			//Permission for TIM7 interrupt
	
	
	while(1)
	{

	}
}

void TIM7_IRQHandler(void)
{
	TIM7->SR &= ~(1<<0);				//Clear interrupt flag
	GPIOA->ODR ^= (1<<0);				//Change PA0 state ( LED )
}
