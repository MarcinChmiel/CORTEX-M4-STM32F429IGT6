//Example shows how to set USART without interrupts
//Sending char every some delay


#include "stm32f4xx.h"

int main(void)
{
	//IO Configuration
	RCC->AHB1ENR |= (1<<0);				//GPIOA Clock enabled
	GPIOA->MODER |= (1<<19);			//GPIOA PA9 Alternate function
	GPIOA->AFR[1] |= (1<<6) | (1<<5) | (1<<4);	//PA9 AF7 USART1 TX
	
	//USART Configuration
	RCC->APB2ENR |= (1<<4);				//USART1 clock enabled
	USART1->CR1 |= (1<<13);				//USART enableb
	//BAUD RATE 9600, fPCLK = 16 MHz, oversampling by 16 (OVER8=0), BRR = 104.1875
	//Mantissa = 104
	//x/16 = 0.1875 -> x = 3
	//Fraction = 3
	USART1->BRR |= (1<<1) | (1<<0);			//Fraction = 3
	USART1->BRR |= (1<<10) | (1<<9) | (1<<7);	//Mantissa = 104	
	USART1->CR1 |= (1<<3);				//Transmitter enabled
	
	
	int i;
	while(1)
	{
		for(i=0;i<1600000;i++);			//Delay
		while(!(USART1->SR & (1<<7)));		//Wait for empty trasmit data register
		USART1->DR = 'a';			//Write char 'a' to data register
	}
	
}
