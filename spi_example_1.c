//Example shows how to set SPI and use BMP280 sensor (in simplification)
//It uses USART to send result to PC

#include "stm32f4xx.h"
uint8_t SPI_transmit(uint8_t, uint8_t);
void UART_send_byte(uint8_t);
uint32_t BMP280_t(uint8_t);

uint8_t data_byte = 0;
uint32_t temp = 0;
int i;
int main()
{
	
	
	//IO Configuration
	RCC->AHB1ENR |= (1<<1);						//GPIOB clock enabled
	GPIOB->MODER |= (1<<24);					//PB12 as output
	GPIOB->ODR |= (1<<12);						//Set PB12
	GPIOB->MODER |= (1<<27);					//PB13 alternate function
	GPIOB->MODER |= (1<<29);					//PB14 alternate function
	GPIOB->MODER |= (1U<<31);					//PB15 alternate function
	GPIOB->AFR[1] |= (5<<20);					//PB13 as SPI2 SCK
	GPIOB->AFR[1] |= (5<<24);					//PB14 as SPI2 MISO
	GPIOB->AFR[1] |= (5<<28);					//PB15 as SPI2 MOSI
	RCC->AHB1ENR |= (1<<0);						//GPIOA clock enabled
	GPIOA->MODER |= (1<<0);						//PA0 as output
	GPIOA->ODR = 0;							//GPIOA initial value
	GPIOA->MODER |= (1<<19);					//GPIOA PA9 Alternate function
	GPIOA->AFR[1] |= (1<<6) | (1<<5) | (1<<4);			//PA9 AF7 USART1 TX
	
	
	//SPI Configuration
	RCC->APB1ENR |= (1<<14);					//SPI2 clock enabled
	SPI2->CR2 |= (1<<2);						//SS output enable
	SPI2->CR1 |= (1<<2);						//Master configuration
	SPI2->CR1 |= (1<<6);						//SPI enabled
	
	
	//USART Configuration
	RCC->APB2ENR |= (1<<4);						//USART1 clock enabled
	USART1->CR1 |= (1<<13);						//USART enableb
	//BAUD RATE 9600, fPCLK = 16 MHz, oversampling by 16 (OVER8=0), BRR = 104.1875
	//Mantissa = 104
	//x/16 = 0.1875 -> x = 3
	//Fraction = 3
	USART1->BRR |= (1<<1) | (1<<0);					//Fraction = 3
	USART1->BRR |= (1<<10) | (1<<9) | (1<<7);			//Mantissa = 104	
	USART1->CR1 |= (1<<3);						//Transmitter enabled
	
	
	//BMP280 Configuration
	#define dig_T1 27965
	#define dig_T2 26715
	#define dig_T3 -1000
	SPI_transmit(0x74, 0x27);					//Set ctrl_meas register (001,001,11)
	SPI_transmit(0x75, 0xE0);					//Set config register (111,000,0,0)
	while(1)
	{
		
		data_byte = SPI_transmit(0xFA, 0x00);			//Retrieve data_byte from register
		temperature = BMP280_t(data_byte);
		UART_send_byte(temperature);
		for(i=0;i<2000000;i++);					//Delay
		
	}
}


uint8_t SPI_transmit(uint8_t address, uint8_t byte)
{
	GPIOB->ODR &= ~(1<<12);						//Transmission start
	while(!(SPI2->SR & (1<<1)));					//Wait until TX buffer will be empty
	SPI2->DR = address;						//Send register address
	while(!(SPI2->SR & (1<<0)));					//Wait until RX buffer will contain any data
	uint8_t data = SPI2->DR;					//Clear RX buffer
	while(!(SPI2->SR & (1<<1)));					//Wait until TX buffer will be empty
	SPI2->DR = byte;						//Send data byte
	while(!(SPI2->SR & (1<<0)));					//Wait until RX buffer will contain any data
	GPIOB->ODR |= (1<<12);						//Transmission end
	data = SPI2->DR;						//Clear RX buffer
	return data;							//Return data byte
}


void UART_send_byte(uint8_t byte)
{
		while(!(USART1->SR & (1<<7)));				//Wait for empty trasmit data register
		USART1->DR = byte;					//Write char 'a' to data register
}

uint32_t BMP280_t(uint8_t data)
{
	int32_t var1 = (((data>>3) - ((uint32_t)dig_T1<<1)) * (uint32_t)dig_T2)>>11;
	int32_t var2 = (((((data>>4) - (uint32_t)dig_T1) * ((data>>4) - (uint32_t)dig_T2))>>12) * (uint32_t)dig_T3)>>14;
	int32_t T = ((var1 + var2) * 5 + 128) >> 8;
	return T;
}
