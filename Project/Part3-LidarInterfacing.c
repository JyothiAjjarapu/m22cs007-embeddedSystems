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

int main(void) {
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

    // Receive data from USART3
    while (1) {
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
			}
		}

        // Perform other operations or delay as needed
