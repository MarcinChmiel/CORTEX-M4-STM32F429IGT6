//Example shows how to set USART without interrupts.
//Receiving and sending back char to PC via USART.

#include "stm32f4xx.h"

int main(void)
{
	//IO Configuration
	RCC->AHB1ENR |= (1<<0);					//GPIOA Clock enabled
	GPIOA->MODER |= (1<<19);				//GPIOA PA9 Alternate function
	GPIOA->MODER |= (1<<21);				//GPIOA PA10 Alternate function
	GPIOA->AFR[1] |= (1<<6) | (1<<5) | (1<<4);		//PA9 AF7 USART1 TX
	GPIOA->AFR[1] |= (1<<10) | (1<<9) | (1<<8);		//PA10 AF7 USART1 RX
	
	//USART Configuration
	RCC->APB2ENR |= (1<<4);					//USART1 clock enabled
	USART1->CR1 |= (1<<13);					//USART enableb
	//BAUD RATE 9600, fPCLK = 16 MHz, oversampling by 16 (OVER8=0), BRR = 104.1875
	//Mantissa = 104
	//x/16 = 0.1875 -> x = 3
	//Fraction = 3
	USART1->BRR |= (1<<1) | (1<<0);				//Fraction = 3
	USART1->BRR |= (1<<10) | (1<<9) | (1<<7);		//Mantissa = 104	
	USART1->CR1 |= (1<<3);					//Transmitter enabled
	USART1->CR1 |= (1<<2);					//Receiver enabled
	
	
	int i;
	char c = '\0';
	while(1)
	{
		while(!(USART1->SR & (1<<5)));			//Wait for data ready to be read
		c = USART1->DR;					//Read data from data register
		
		while(!(USART1->SR & (1<<7)));			//Wait for empty transmit data register
		USART1->DR = c;					//Write char 'a' to data register
	}
	
}
