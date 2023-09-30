#include "stm32f4xx.h" // Device header
void delay();
int main()
{

RCC->APB1ENR |= 100; // Enabling Timer 4.
RCC->APB1ENR |= 4;   // set clock for APB1 where TIM4 is located

RCC->AHB1ENR |= 011; //Enabling the clock signals to the GPIOB and GPIOA peripherals. B is for output, A is for input.
	

GPIOB->MODER |=0x10000000;	//Configuring the GPIOB port pin 14(PB.14) as output pin. Moder is 32 bits, out of which 28th bit will be for pin14. So, enabling it to 1.

GPIOB->ODR^=0x4000;	//set that pin as high, so that led blinks when powered on. for this, enable pin 14 of output data register (16 bits).


//done with configuring Timer4 and output port to be gpioB.


						
TIM4->ARR=64000;  // setting ARR to 64000. 									
TIM4->PSC=500;	// if ARR=64000, and delay is 2 seconds, I calculated PSC, and It turned out to be 500. This will reduce clock frequency from 16MHZ to 32KHZ. 														
//done with ARR,PSC assignments.


TIM4->CNT=0;   // clearing the timer 4 counter
TIM4->CR1=1;   // start timer 4 


while(1)
{
		int count=0;
		if(GPIOA->IDR & 0X2000) // using GPIOA as input data register. push button is conected to 13th pin. IDR is 16bit register, so 0x2000 is 13th pin. checking is push button is on or not.
			{	
				while((GPIOC->IDR & 0X2000)!= 0X00);// wait if button pressed.
				while(count<=5) // loop till 5 times 
				{

					GPIOB->ODR ^=0x4000; // blink led, setting the output pin as high.
								delay(); //2 seconds delay.
					GPIOB->ODR ^=0x4000;// again blink led.
								delay(); //2 seconds delay.
					count=count+1;		// increment the count.	
				}
			}
	}
	return 1;
}

GPIOB->ODR ^=0x0000;// again blink led.


void delay()
{
	TIM4->SR=0;// setting status register to zero, so that no ISR is served in the mean time.
	TIM4->CR1 |=1; // starting the timer 4.

	while((TIM4->SR & TIM_SR_UIF)==0); //Wait until ARR value becomes zero.
}