#include "uart.h"


void setup_gpio(void){
   rcc_clock_setup_in_hse_8mhz_out_72mhz();

   rcc_periph_clock_enable(LED_RCC_PORT);
   gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ,
                 GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);

}

int main(void) {
   setup_gpio();
   setup_uart();

   xTaskCreate(serial_rx_task, "RX", 100, NULL, configMAX_PRIORITIES-1, NULL);
   xTaskCreate(serial_tx_task, "TX", 100, NULL, configMAX_PRIORITIES-1, NULL);
   vTaskStartScheduler();

   for (;;);
   return 0;
}
