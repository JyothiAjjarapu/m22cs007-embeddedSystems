#include "stm32f4xx.h"

// Define USART2 and USART3 registers
#define USART2_BASE_ADDRESS   USART2
#define USART3_BASE_ADDRESS   USART3

// Function to initialize USART2
void USART2_Init(void) {
    // Enable USART2 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure GPIO pins for USART2
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;  // Alternate function mode
    GPIOA->AFR[0] |= (7 << (4 * 2)) | (7 << (4 * 3));  // Alternate function AF7 (USART2) for pins PA2 and PA3

    // Configure USART2
    USART2->BRR = 0x0683;  // Baud rate of 9600 (assuming system clock of 16MHz)
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE;  // Enable USART2 transmitter and USART2

    // Wait for USART2 to be ready
    while (!(USART2->SR & USART_SR_TC));
}

// Function to initialize USART3
void USART3_Init(void) {
    // Enable USART3 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    // Configure GPIO pins for USART3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;  // Alternate function mode
    GPIOB->AFR[1] |= (7 << (4 * 2)) | (7 << (4 * 3));  // Alternate function AF7 (USART3) for pins PB10 and PB11

    // Configure USART3
    USART3->BRR = 0x0683;  // Baud rate of 9600 (assuming system clock of 16MHz)
    USART3->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;  // Enable USART3 transmitter, receiver, and USART3
}


int main(void) {
    // Initialize USART2 and USART3
    USART2_Init();
    USART3_Init();

    // Send letter "c" from USART2 to USART3
    USART2->DR = 'l';
	  while (!(USART2->SR & USART_SR_TXE));  // Wait for transmit data register to be empty
    while ((USART3->SR & USART_SR_RXNE));  // Wait for receive data register to be non-empty
}
