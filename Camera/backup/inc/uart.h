#ifndef UART_H
#define UART_H

#include "stdint.h"
#include "math.h"
#include "stm32f4xx.h"

// APB frequencies
#define APB1_FREQ 42000000.0
#define APB2_FREQ 84000000.0

// GPIO pin mode
#define GPIO_Mode_AF 0x02
// GPIO speeds
#define GPIO_Speed_2MHz 0x00, // Low speed
#define GPIO_Speed_25MHz 0x01 // Medium speed
#define GPIO_Speed_50MHz 0x02 // Fast speed 
#define GPIO_Speed_100MHz 0x03  // High speed
// GPIO pull up/pull down
#define GPIO_PuPd_UP 0x01
// GPIO AF mappings
#define GPIO_AF_USART123 0x07 // USART1, USART2, USART3
#define GPIO_AF_UART456  0x08  // UART4, UART5, UART6

void initUart(GPIO_TypeDef * GPIOx, USART_TypeDef * USARTx, uint8_t pin_rx, uint8_t pin_tx, uint32_t baudrate);
void sendByte(USART_TypeDef * USARTx, uint8_t data);
uint8_t readByte(USART_TypeDef * USARTx);
uint8_t flagStatus(USART_TypeDef * USARTx, uint16_t flag);

#endif
