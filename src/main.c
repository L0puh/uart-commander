#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#define BAURATE 38400
#define LED_PORT GPIOA
#define LED_RCC_PORT RCC_GPIOA
#define LED_PIN GPIO10

static void uart_putc(char c){
   usart_send_blocking(USART1, c);
}

static void serial_task(void *arg __attribute__ ((unused))) {
   int c = '0'-1;
   for (;;) {
      gpio_toggle(LED_PORT, LED_PIN);
      vTaskDelay(pdMS_TO_TICKS(500));
      if ( ++c >= 'Z') {
         uart_putc(c);
         uart_putc('\r');
         uart_putc('\n');
         c = '0'-1;
      } else {
         uart_putc(c);
      }

   }
}


void setup_uart(void){
   rcc_periph_clock_enable(RCC_USART1);
   // UART - connected to PA9
   gpio_set_mode(LED_PORT, 
                 GPIO_MODE_OUTPUT_50_MHZ,
                 GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                 GPIO_USART1_TX);


   usart_set_baudrate(USART1, BAURATE);
   usart_set_databits(USART1, 8);
   usart_set_stopbits(USART1, USART_STOPBITS_1);
   usart_set_mode(USART1, USART_MODE_TX);
   usart_set_parity(USART1, USART_PARITY_NONE);
   usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
   usart_enable(USART1);

}

void setup_gpio(void){
   rcc_clock_setup_in_hse_8mhz_out_72mhz();

   // PA10 - connected to LED 
   rcc_periph_clock_enable(LED_RCC_PORT);
   gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ,
                 GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);

}


int main(void) {
   setup_gpio();
   setup_uart();

   xTaskCreate(serial_task, "serial_task", 100, NULL, configMAX_PRIORITIES-1, NULL);
   vTaskStartScheduler();

   for (;;);
   return 0;
}
