#ifndef UART_H
#define UART_H

#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <queue.h>
#include <task.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#define BAURATE 38400
#define LED_PORT GPIOA
#define LED_RCC_PORT RCC_GPIOA
#define LED_PIN GPIO11


void setup_uart(void);
void telemetry_send(const char *s);
//TODO: void recv_command(void);

void serial_rx_task(void *arg __attribute__ ((unused)));
void serial_tx_task(void *arg __attribute__ ((unused)));

//TODO: parse_command();

#endif

