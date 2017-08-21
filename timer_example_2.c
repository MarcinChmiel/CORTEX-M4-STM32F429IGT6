//Timer TIM3 -	Count Up Mode - LED blnking every 1s
//PA0 - LED

#include "stm32f4xx.h"

int main(void)
{
	//IO Configuration
	RCC->AHB1ENR |= (1<<0);				//GPIOA Clock Enable
	GPIOA->MODER |= (1<<0);				//PA0 as output
	GPIOA->ODR = 0;					//Set GPIOA initial value to 0
	
	
	//Timer TIM2 Configuration
	RCC->APB1ENR |= (1<<1);				//TIM3 Clock Enable
	TIM3->CR1 |= (1<<7);				//TIMx_ARR register is buffered
	TIM3->PSC = 999;				//Prescaler 1000
	TIM3->ARR = 16000;				//16M / 1000 / 16000 = 1s
	TIM3->CR1 &= ~(1<<4);				//Bit 4 = 0 ( as default), Counter used as upcounter
	TIM3->CR1 |= (1<<2);				//Only counter overflow generates an interrupt
	TIM3->DIER |= (1<<0);				//Update interrupt enabled
	TIM3->CR1 |= (1<<0);				//Counter enabled
	
	
	//Interrupt Enable
	NVIC_EnableIRQ(TIM3_IRQn);
	while(1)
	{
		
	}
}

void TIM3_IRQHandler(void)
{
	TIM3->SR &= ~(1<<0);				//Clear interrupt flag
	GPIOA->ODR ^= (1<<0);				//Switch PA0 state ( LED )
}
