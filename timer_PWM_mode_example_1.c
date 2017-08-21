//Example shows how to set TIM3 Timer in PWM mode

#include "stm32f4xx.h"

int main(void)
{
	//IO Configuration
	RCC->AHB1ENR |= (1<<0);				//GPIOA Clock Enable
	GPIOA->MODER |= (1<<0);				//GPIOA PA0 Output
	GPIOA->ODR = 0;					//GPIOA initial value
	GPIOA->MODER |= (1<<13);			//GPIOA PA6 Alternate function
	GPIOA->AFR[0] |= (1<<25);			//GPIOA PA6 Alternate function: AF2 = Timer3 Channel 1
	
	//Timer3 Configuration
	RCC->APB1ENR |= (1<<1);				//TIM3 Clock Enable
	TIM3->CR1 |= (1<<7);				//TIM3_ARR register is buffered
	TIM3->CCMR1 |= (1<<6) | (1<<5);			//PWM mode 1, channel 1
	TIM3->CCMR1 |= (1<<0);				//CC1 channel is configured as input
	TIM3->CCMR1 |= (1<<3);  			//Preload register on TIM3_CCR1 enabled	
	TIM3->CCMR1 &= ~(1<<0);				//CC1 channel is configured as output
							//Changing CH1 to input is forced by setting preload register
	TIM3->PSC = 0;					//Prescaler 1
	TIM3->ARR = 100;				//FREQ 160KHz
	TIM3->CCR1 = 50;				//Duty Cycle [%]
	TIM3->CCER |= (1<<0);				//OC1 signal is output on the corresponding output pin
	
	TIM3->EGR |= (1<<0);				//Re-initialize the counter and generates 													//registers update
	TIM3->CR1 |= (1<<0);				//Counter Enabled
	while(1)
	{
	}
}
