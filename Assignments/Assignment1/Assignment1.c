#include "stm32f4xx.h"
#include<stdio.h>

int odd_sum(int arr[], int size) 
{
    int sum = 0; // initializing sum to 0
	
	  int i=0; // initializing the loop variable to 0
	
    for (i = 0; i < size; i++) // for loop to run the loop till the array elements are done.. so using size 
	  {
        if (arr[i] % 2 != 0) //checking if the element is odd or even.
				{
            sum += arr[i]; // if its odd, then only add to sum.
        }
    } //done with the elements
		
    return sum; // return the sum to the main function.
}

int main(void) 
	{
		int x = __get_CONTROL();		// get the output sum to display the results.
   
		int sum=0; //initializing sum to 0
		
    int c[] = {11, 20, 35, 47, 51, 6, 70, 82, 91, 10}; // declaring array "c" with 10 elements
		
    int size = sizeof(c) / sizeof(c[0]);  //declaring size to be the size of array.
		
    sum = odd_sum(c, size); // calling the odd_sum function, where the actual code is written.
		
		printf("Sum of odd elements: %d\n", sum);
		
    
    __set_CONTROL(x);  // set the output sum to display
    
    RCC->AHB1ENR |= 1; // enabling the clock for GPIOA peripheral.
    
    GPIOA->MODER |= 0x5555; // configuring the GPIOA as output
   
		GPIOA->ODR = 0x00;     
   
    
		GPIOA->ODR = sum; // set the value of the output data register of the GPIOA peripheral to the value of the variable "output"
   
    return 0;
}

