//Example shows how to use SysTick Timer

#include "stm32f4xx.h"

int main(void)
{
	
	SysTick_Config(SystemCoreClock);			//SysTick Timer Configuration
								//System Core Clock = FCPU

	while(1)
	{

	}
}


void SysTick_Handler(void)					//Interrupt function for SysTick Timer
{

}
