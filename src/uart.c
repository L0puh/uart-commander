#include "uart.h"

static QueueHandle_t tx_queue;
static QueueHandle_t rx_queue;

void telemetry_send(const char *s){
   while (*s){
      xQueueSend(tx_queue, s, portMAX_DELAY);
      s++;
   }
}

void serial_rx_task(void *arg __attribute__ ((unused))) {
   for (;;){
      if (usart_get_flag(USART1, USART_SR_RXNE)){
         char c = usart_recv(USART1);
         xQueueSendToBack(rx_queue, &c, 0);
      }
      taskYIELD();
   }

}

void serial_tx_task(void *arg __attribute__ ((unused))) {
   char c;
   for (;;) {
      if (xQueueReceive(tx_queue, &c, 500) == pdPASS){
         if (usart_get_flag(USART1, USART_SR_TXE)) {
            usart_send(USART1, c);
            gpio_toggle(LED_PORT, LED_PIN);
            return;
         }
         
         taskYIELD();
      }

   }
}


void setup_uart(void){
   rcc_periph_clock_enable(RCC_USART1);

   gpio_set_mode(LED_PORT, 
                 GPIO_MODE_OUTPUT_50_MHZ,
                 GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                 GPIO_USART1_TX); //PA9
   
   gpio_set_mode(LED_PORT, 
                 GPIO_MODE_OUTPUT_50_MHZ,
                 GPIO_CNF_INPUT_FLOAT,
                 GPIO_USART1_RX); //PA10


   usart_set_baudrate(USART1, BAURATE);
   usart_set_databits(USART1, 8);
   usart_set_stopbits(USART1, USART_STOPBITS_1);
   usart_set_mode(USART1, USART_MODE_TX_RX);
   usart_set_parity(USART1, USART_PARITY_NONE);
   usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
   usart_enable(USART1);

   tx_queue = xQueueCreate(256, sizeof(char));

}


