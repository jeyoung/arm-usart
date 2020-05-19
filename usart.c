#include "usart.h"

int main(void)
{
    // Enable GPIOA
    RCC_AHB1ENR |= (0x01 << 0x00);
    // Alternate function (AF) mode for PA9 and PA10
    GPIOA_MODER |= (0x02 << 0x12) | (0x02 << 0x14);
    // AF7 (USART TX and RX) for PA9 and PA10
    GPIOA_AFRH  |= (0x07 << 0x04) | (0x07 << 0x08); 

    // Configure PA5 for debugging output
    GPIOA_MODER |= (0x01 << 0x0A);

    // Enable USART1
    RCC_APB2ENR |= (0x01 << 0x04);
    // Configure baud rate based on APB2 clock (equals to HSI 16MHz by default)
    USART1_BRR   = 0x683;
    // Enable TX and RX, and USART overall
    USART1_CR1  |= (0x01 << 0x03) | (0x01 << 0x02) | (0x01 << 0x0D);

    while (1)
    {
        // Look for an RX
        if (USART1_SR & (0x01 << 0x05))
        {
            // Light up PA5 to confirm we're here
            GPIOA_ODR |= (0x01 << 0x05);

            char c = USART1_DR;
            USART1_DR = c;

            // Check for TC before continuing
            while (!(USART1_SR & (0x01 << 0x06)))
                ;
        }
    }

    return 0;
}
