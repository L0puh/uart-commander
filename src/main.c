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
#define LED_PIN GPIO10

static QueueHandle_t tx_queue;

static void uart_send(const char *s){
   while (*s){
      xQueueSend(tx_queue, s, portMAX_DELAY);
      s++;
   }

}

static void test_task(void *arg __attribute__ ((unused))) {
   for (;;){
      uart_send("Test function to send telemetry\n\r");
      uart_send("                send via queues\n\n\r");
      vTaskDelay(pdMS_TO_TICKS(1000));
   }

}

static void serial_task(void *arg __attribute__ ((unused))) {
   char c;
   for (;;) {
      if (xQueueReceive(tx_queue, &c, 500) == pdPASS){
         while (!usart_get_flag(USART1, USART_SR_TXE)) 
            taskYIELD();
         usart_send(USART1, c);
      }
      gpio_toggle(LED_PORT, LED_PIN);

   }
}


void setup_uart(void){
   rcc_periph_clock_enable(RCC_USART1);

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

   tx_queue = xQueueCreate(256, sizeof(char));

}

void setup_gpio(void){
   rcc_clock_setup_in_hse_8mhz_out_72mhz();

   rcc_periph_clock_enable(LED_RCC_PORT);
   gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ,
                 GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);

}


int main(void) {
   setup_gpio();
   setup_uart();

   xTaskCreate(serial_task, "serial_task", 100, NULL, configMAX_PRIORITIES-1, NULL);
   xTaskCreate(test_task, "test_task", 100, NULL, configMAX_PRIORITIES-2, NULL);
   vTaskStartScheduler();

   for (;;);
   return 0;
}
