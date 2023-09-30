#include "stm32f4xx.h"

// Define USART3 base address
#define USART3_BASE_ADDRESS   USART3

// Function to initialize USART3
void USART3_Init(void) {
    // Enable USART3 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    // Configure GPIO pins for USART3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;  // Alternate function mode
    GPIOB->AFR[1] |= (7 << (4 * 2)) | (7 << (4 * 3));  // Alternate function AF7 (USART3) for pins PB10 and PB11

    // Configure USART3
    USART3->BRR = 138;  // Baud rate of 115200 (assuming system clock of 16MHz)
    USART3->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;  // Enable USART3 transmitter, receiver, and USART3
}
void Timer(void)
	{
		RCC->AHB1ENR |= (1 << 1) | (1<<2) ; //Enable Clock for PORTB and PORTC
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //Enables Timer4 with frequency 16MHZ
		TIM4->PSC=15; //Prescaler to set clock frequency at 1Mhz
		TIM4->CR1 |=(1<<0); //Enable the Timer
	}
void leap(int get) //To count 1milli second as each clock pulse is 1us or Timer counts untill CNT is 1000
	{
		TIM4->CNT = 0;
		while(TIM4->CNT < get);
	}
void mill(int loop)
	{
		for (int j=0; j<loop; j++)
		{
			leap(10);
		}
	}
	
	
int main(void)
	{
		int dutycycle = 0;
	  // Initialize USART3
    USART3_Init();

    // Send initial command (5A 05 07 01 67)
    USART3->DR = 0x5A;  // Send byte 0x5A
    while (!(USART3->SR & USART_SR_TXE));  // Wait for transmit data register to be empty

    USART3->DR = 0x05;  // Send byte 0x05
    while (!(USART3->SR & USART_SR_TXE));

    USART3->DR = 0x07;  // Send byte 0x07
    while (!(USART3->SR & USART_SR_TXE));

    USART3->DR = 0x01;  // Send byte 0x01
    while (!(USART3->SR & USART_SR_TXE));

    USART3->DR = 0x67;  // Send byte 0x67
    while (!(USART3->SR & USART_SR_TXE));

    // Variables for distance measurement
    uint16_t distance = 0;	
		
	Timer();
	GPIOB->MODER|= (1<<10); //Select PB14 as Output
	int y=0;
	while(1)
	{
					if(GPIOC->IDR & (1<<13)) //Senses Input from the USER Push BUtton
					{     
							while(y<250)
											{
											GPIOB->ODR^=(1<<5); //Turns On Led Connected to PB5
											mill(100); // Delays 2000 ms or 2seconds
											GPIOB->ODR^=(1<<5);
											mill(1900);
													y=y+1;
										}
							break;}
	}   
	
	// main show starts from here
	while(1)
		{
			if(GPIOC->IDR & (1<<13)) //Senses Input from the USER Push BUtton
				{         
				 while(1)
						{
						if (USART3->SR & USART_SR_RXNE) {  // Check if data is available in the receiver data register
            uint8_t receivedData = USART3->DR;  // Read the received data

            // Process the received data
            static uint8_t state = 0;
            static uint16_t highByte = 0;

            switch (state) {
                case 0:
                    if (receivedData == 0x59) {
                        state = 1;
                    }
                    break;
                case 1:
                    if (receivedData == 0x59) {
                        state = 2;
                    } else {
                        state = 0;
                    }
                    break;
                case 2:
                    highByte = receivedData;
                    state = 3;
                    break;
                case 3:
                    distance = (highByte << 8) | receivedData;
                    state = 0;
                    break;
                default:
                    state = 0;
                    break;
            }
        }
							if(distance<=0x56A)
									dutycycle=100;
							else if(distance>0x56A && distance<=0x2AAA)
								dutycycle = 110;
							else if(distance>0x2AAA && distance<=0x6AAB)
								dutycycle = 130;
							else if(distance>0x6AAB && distance<=0x9555)
								dutycycle = 150;
							else if(distance>0x9555 && distance<=0xC000)
								dutycycle = 170;
							else 
								dutycycle = 200;						
							GPIOB->ODR^=(1<<5); //Turns On PB5				 
							mill(dutycycle); // Delays 2000 ms or 2seconds			 
							GPIOB->ODR^=(1<<5);
							mill(2000-dutycycle);

						}
			 }
		}
	}
