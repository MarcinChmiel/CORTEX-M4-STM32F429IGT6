//Example shows how to set I2C
//How to write & read data to/from BMP280 sensor register

#include "stm32f4xx.h"
void USART_send_byte(uint8_t);
void simple_delay(uint32_t);
uint8_t I2C_receive_byte(uint8_t, uint8_t);
void I2C_send_byte(uint8_t, uint8_t, uint8_t);

int main(void)
{
	uint8_t data_byte;
	
	//IO Clocks
	RCC->AHB1ENR |= (1<<0);								//GPIOA Clock enabled
	RCC->AHB1ENR |= (1<<1);								//GPIOB clock enabled
	//IO Configuration for USART
	GPIOA->MODER |= (1<<19);							//GPIOA PA9 Alternate function
	GPIOA->AFR[1] |= (7<<4);							//PA9 AF7 USART1 TX
	//IO Configuration for I2C
	GPIOB->MODER |= (1<<13);							//PB6 alternate function
	GPIOB->MODER |= (1<<15);							//PB7 alternate function
	GPIOB->AFR[0] |= (1<<26);							//PB6 as I2C1 SCL
	GPIOB->AFR[0] |= (1<<30);							//PB7 as I2C1 SDA
	GPIOB->OTYPER |= (3<<6);							//OPEN DRAIN
	//USART Configuration
	RCC->APB2ENR |= (1<<4);								//USART1 clock enabled
	USART1->CR1 |= (1<<13);								//USART1 enableb
											//BAUD RATE 9600, fPCLK = 16 MHz, oversampling by 16 (OVER8=0), BRR = 104.1875
											//Mantissa = 104
											//x/16 = 0.1875 -> x = 3
											//Fraction = 3
	USART1->BRR |= (3<<0);								//Fraction = 3
	USART1->BRR |= (13<<7);								//Mantissa = 104	
	USART1->CR1 |= (1<<3);								//Transmitter enabled
	//I2C Configuration
	RCC->APB1ENR |= (1<<21);							//I2C1 clock enabled
	I2C1->CR2 |= (16<<0);								//Peripheral clock frequency 16MHz
	I2C1->CCR |= (80<<0);								//Clock control register in Fast/Standard mode (Master mode)
	I2C1->TRISE |= (17<<0);								//Maximum rise time in Fast/Standard mode (Master mode)
	I2C1->CR1 |= (1<<0);								//Peripheral enabled
	
	
	
	while(1)
	{
		I2C_send_byte(0x76, 0xF4, 0x27);
		data_byte = I2C_receive_byte(0x76, 0xF4);
		USART_send_byte(data_byte);
		simple_delay(2000000);
	}
}

void USART_send_byte(uint8_t byte)
{
		while(!(USART1->SR & (1<<7)));						//Wait for empty trasmit data register
		USART1->DR = byte;							//Write data to data register
}

void simple_delay(uint32_t t)
{
		uint32_t i;
		for(i=0;i<t;i++);
}

uint8_t I2C_receive_byte(uint8_t slave_address, uint8_t register_address)
{
	  I2C1->CR1 |= (1<<10);								//Acknowledge enable
		//Firstly send slave address with WRITE LSB(0), because we'll write register address
		I2C1->CR1 |= (1<<8);							//Generate a Start condition
		while(!(I2C1->SR1 & (1<<0)));						//Wait for SB=1
											//Slave address 111011X, X=0 if SDO = GND, X=1 if SDO = Vddio
											//+ LSB = 0 if write, 1 if read
		I2C1->DR = (slave_address<<1);						//Send slave address: 11101100 (write, SDO = GND)
		while(!(I2C1->SR1 & (1<<1)));						//Wait for ADDR=1
		uint16_t waste = I2C1->SR1;						//Read SR1 & SR2 to clear ADDR bit
		waste = I2C1->SR2;							//Read SR1 & SR2 to clear ADDR bit
		while(!(I2C1->SR1 & (1<<7)));						//Wait for TXE=1
		I2C1->DR = register_address;						//Send byte with register address

		//Secondly send slave address with READ LSB(1), because we'll read data from register
		I2C1->CR1 |= (1<<8);							//Generate a Start condition
		while(!(I2C1->SR1 & (1<<0)));						//Wait for SB=1
											//Slave address 111011X, X=0 if SDO = GND, X=1 if SDO = Vddio
											//+ LSB = 0 if write, 1 if read
		I2C1->DR = ((slave_address<<1)+1);					//Send slave address: 11101101 (read, SDO = GND)
		while(!(I2C1->SR1 & (1<<1)));						//Wait for ADDR=1
		I2C1->CR1 &= ~(1<<10);							//Acknowledge disable = generate noacknowledge bit
		waste = I2C1->SR1;							//Read SR1 & SR2 to clear ADDR bit
		waste = I2C1->SR2;							//Read SR1 & SR2 to clear ADDR bit
		while(!(I2C1->SR1 & (1<<6)));						//Wait for RXNE=1
		I2C1->CR1 |= (1<<9);							//Stop generation
		return I2C1->DR;							//Read data byte
}

void I2C_send_byte(uint8_t slave_address, uint8_t register_address, uint8_t byte)
{
	  	I2C1->CR1 |= (1<<10);							//Acknowledge enable
		//Firstly send slave address with WRITE LSB(0), because we'll write register address
		I2C1->CR1 |= (1<<8);							//Generate a Start condition
		while(!(I2C1->SR1 & (1<<0)));						//Wait for SB=1
											//Slave address 111011X, X=0 if SDO = GND, X=1 if SDO = Vddio
											//+ LSB = 0 if write, 1 if read
		I2C1->DR = (slave_address<<1);						//Send slave address: 11101100 (write, SDO = GND)
		while(!(I2C1->SR1 & (1<<1)));						//Wait for ADDR=1
		uint16_t waste = I2C1->SR1;						//Read SR1 & SR2 to clear ADDR bit
		waste = I2C1->SR2;							//Read SR1 & SR2 to clear ADDR bit
		while(!(I2C1->SR1 & (1<<7)));						//Wait for TXE=1
		I2C1->DR = register_address;						//Send byte with register address
		while(!(I2C1->SR1 & (1<<2)));						//Wait for BTF=1
		I2C1->DR = byte;							//Send data byte to register
		I2C1->CR1 |= (1<<9);							//Stop generation
		I2C1->CR1 &= ~(1<<10);							//Acknowledge disable = generate noacknowledge bit
}
