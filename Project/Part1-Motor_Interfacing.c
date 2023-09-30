#include "stm32f4xx.h"
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
	while(1)
		{
						if(GPIOC->IDR & (1<<13)) //Senses Input from the USER Push BUtton
						{         
							 while(1)
												{
											 GPIOB->ODR^=(1<<5); //Turns On PB5
											 mill(200); // Delays 2000 ms or 2seconds
											 GPIOB->ODR^=(1<<5);
											 mill(2000-200);
									}
										}
		}
	}
