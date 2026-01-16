#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK		0
#define configUSE_TICK_HOOK		0
#define configCPU_CLOCK_HZ		72000000
#define configTICK_RATE_HZ		1000		// 1ms tick
#define configMAX_PRIORITIES		5
#define configMINIMAL_STACK_SIZE	128
#define configTOTAL_HEAP_SIZE		(6 * 1024)	// 6KB
#define configMAX_TASK_NAME_LEN		16
#define configUSE_TRACE_FACILITY	0
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1
#define configUSE_MUTEXES		0
#define configCHECK_FOR_STACK_OVERFLOW	0

#define INCLUDE_vTaskDelay		1
#define INCLUDE_vTaskDelayUntil		1
#define INCLUDE_xTaskGetSchedulerState	1

/* Cortex-M specific */
#define configPRIO_BITS			4
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY		0xf
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5
#define configKERNEL_INTERRUPT_PRIORITY		(0xf << (8 - configPRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY	(5 << (8 - configPRIO_BITS))

/* Handler mappings - these might need adjustment based on your FreeRTOS version */
#define xPortPendSVHandler	pend_sv_handler
#define vPortSVCHandler		sv_call_handler
#define xPortSysTickHandler	sys_tick_handler

#endif /* FREERTOS_CONFIG_H */
