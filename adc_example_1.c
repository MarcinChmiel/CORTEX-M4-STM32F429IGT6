//Example shows how to set ADC without interrupts.
//When analog voltage is greater than 2V - turn on LED

//4096	-	VREF
//ADC	-	Vin
//Vin = ADC * VREF / 4096

#include "stm32f4xx.h"

#define VREF 3.3

int main(void)
{
	//IO Configuration
	RCC->AHB1ENR |= (1<<0);						//GPIOA Clock Enable
	GPIOA->MODER |= (1<<2);						//GPIOA PA1 Output
	GPIOA->ODR = 0;							//GPIOA initial value
	GPIOA->MODER |= (1<<1) | (1<<0);				//GPIOA PA0 Analog mode
	
	//ADC Configuration
	RCC->APB2ENR |= (1<<8);						//ADC1 Clock Enable
	ADC1->CR2 |= (1<<1);						//Continuous conversion mode															//1 Conversion, Channel 0 ( PA0 )
	ADC1->CR2 |= (1<<0);						//ADC Enable
	ADC1->CR2 |= (1<<30);						//Start conversion
	float result;
	
	while(1)
	{
		
		while(!(ADC1->SR & (1<<1)));				//Wait for end of conversion ( until EOC = 1 )
		result = (ADC1->DR  & 0xFFF) * VREF / 4096;
		if(result > 2)
			GPIOA->ODR |= (1<<1);
		else
			GPIOA->ODR &= ~(1<<1);
	}
}
