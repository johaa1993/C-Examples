/*
Write a program using freeRTOS that blink the LEDs at dierent inter-valls, LED1 = 1 s, LED2 = 2 s etc.
Pressing a button (1-3) should makethe corresp onding LED (1-3) stay lit for 10 seconds while the others keepblinking.
Make the various task output their actions to the serial port.
*/
#include <stdio.h>
#include <stdarg.h>
#include <FreeRTOS.h>
#include <task.h>
#include "board.h"
#include "compiler.h"
#include "gpio.h"
#include "pm.h"
#include "usart.h"
#include "semphr.h"

 #define configDBG 1
 #define configDBG_USART (&AVR32_USART0)
 #define configDBG_USART_RX_PIN AVR32_USART0_RXD_0_0_PIN
 #define configDBG_USART_RX_FUNCTION AVR32_USART0_RXD_0_0_FUNCTION
 #define configDBG_USART_TX_PIN AVR32_USART0_TXD_0_0_PIN
 #define configDBG_USART_TX_FUNCTION AVR32_USART0_TXD_0_0_FUNCTION
 #define configDBG_USART_BAUDRATE 57600
 #define serialPORT_USART (&AVR32_USART0)
 #define serialPORT_USART_RX_PIN AVR32_USART0_RXD_0_0_PIN
 #define serialPORT_USART_RX_FUNCTION AVR32_USART0_RXD_0_0_FUNCTION
 #define serialPORT_USART_TX_PIN AVR32_USART0_TXD_0_0_PIN
 #define serialPORT_USART_TX_FUNCTION AVR32_USART0_TXD_0_0_FUNCTION
 #define serialPORT_USART_IRQ AVR32_USART0_IRQ
 #define serialPORT_USART_BAUDRATE 57600

void Init_USART ()
{
	static const gpio_map_t USART_SERIAL_GPIO_MAP =
	{
		{ serialPORT_USART_RX_PIN , serialPORT_USART_RX_FUNCTION }, { serialPORT_USART_TX_PIN , serialPORT_USART_TX_FUNCTION }
	};
	static const gpio_map_t USART_DEBUG_GPIO_MAP =
	{
		{ configDBG_USART_RX_PIN , configDBG_USART_RX_FUNCTION }, { configDBG_USART_TX_PIN , configDBG_USART_TX_FUNCTION }
	};
	static const usart_options_t USART_OPTIONS =
	{
		. baudrate = 115200 , .charlength = 8, .paritytype = USART_NO_PARITY , .stopbits = USART_1_STOPBIT , .channelmode = USART_NORMAL_CHMODE
	};
	pm_switch_to_osc0 (&AVR32_PM, FOSC0, OSC0_STARTUP);
	gpio_enable_module (USART_SERIAL_GPIO_MAP, 2);
	gpio_enable_module (USART_DEBUG_GPIO_MAP, 2);
	usart_init_rs232 (configDBG_USART, &USART_OPTIONS, FOSC0);
	usart_init_rs232 (serialPORT_USART, &USART_OPTIONS, FOSC0);
}


void USART_printf_critical (volatile avr32_usart_t * usart, const char * format, ...)
{
	taskENTER_CRITICAL();
	#define Buffer_Size 100
	char buffer[Buffer_Size] = {'\0'};
	va_list args;
	va_start (args, format);
	vsprintf (buffer, format, args);
	va_end (args);
	usart_write_line (usart, buffer);
	#undef Buffer_Size
	taskEXIT_CRITICAL();
};



struct Task_Config
{
	portTickType Start_Delay;
	char * Name;
	int Priority;
	xTaskHandle Handle;
	pdTASK_CODE Function;
	unsigned short Stack_Depth;
	xSemaphoreHandle Semaphore;
	portTickType Semaphore_Timeout;
	portTickType Work;
	portTickType Deadline;
};

struct Task_Config Task_Config_0;
struct Task_Config Task_Config_1;
struct Task_Config Task_Config_2;
xSemaphoreHandle Semaphore_0;


void Task_Create (struct Task_Config * Config)
{
	xTaskCreate (Config->Function, Config->Name, Config->Stack_Depth, Config, tskIDLE_PRIORITY + Config->Priority, &Config->Handle);
}




void Task_Function_0 (void * Arguments)
{
	struct Task_Config * Config = (struct Task_Config *) Arguments;
	vTaskDelay (Config->Start_Delay);
	USART_printf_critical (configDBG_USART, "Task %s starts\n", Config->Name);
	
	portTickType Block_Count = xTaskGetTickCount();
	if (xSemaphoreTake (Config->Semaphore, Config->Semaphore_Timeout) == pdTRUE)
	{
		USART_printf_critical (configDBG_USART, "Task %s takes Semaphore\n", Config->Name);
		portTickType Count = xTaskGetTickCount();
		while (xTaskGetTickCount() - Count < Config->Work){} // Work
		portTickType Delta = xTaskGetTickCount() - Block_Count;
		USART_printf_critical (configDBG_USART, "Task %s worked for %i ticks\n", Config->Name, Delta);
		if (Delta > Config->Deadline)
		{
			USART_printf_critical (configDBG_USART, "Task %s failed deadline\n", Config->Name);
		}
		else
		{
			USART_printf_critical (configDBG_USART, "Task %s within deadline\n", Config->Name);
		}
		USART_printf_critical (configDBG_USART, "Task %s gives Semaphore\n", Config->Name);
		xSemaphoreGive (Config->Semaphore);
	}
	//vTaskSuspend (NULL);
}

void Task_Function_B (void * Arguments)
{
	struct Task_Config * Config = (struct Task_Config *) Arguments;
	vTaskDelay (Config->Start_Delay);
	USART_printf_critical (configDBG_USART, "Task %s started\n", Config->Name);
	portTickType Count = xTaskGetTickCount();
	while (xTaskGetTickCount() - Count < Config->Work){}  // Work
	USART_printf_critical (configDBG_USART, "Task %s work is done\n", Config->Name);
	vTaskSuspend (NULL);
}





int main()
{
	Init_USART ();

	USART_printf_critical (configDBG_USART, "USART is ready\n");

	vSemaphoreCreateBinary (Semaphore_0);
	
	{
		struct Task_Config * Config = &Task_Config_0;
		Config->Start_Delay = 0;
		Config->Handle = NULL;
		Config->Priority = 1;
		Config->Name = "A";
		Config->Function = Task_Function_0;
		Config->Stack_Depth = configMINIMAL_STACK_SIZE;
		Config->Semaphore = Semaphore_0;
		Config->Semaphore_Timeout = 10000;
		Config->Work = 100;
		Config->Deadline = 8000;
		Task_Create (Config);
	}
	
	
	{
		struct Task_Config * Config = &Task_Config_1;
		Config->Start_Delay = 10;
		Config->Handle = NULL;
		Config->Priority = 2;
		Config->Name = "B";
		Config->Function = Task_Function_B;
		Config->Stack_Depth = configMINIMAL_STACK_SIZE;
		Config->Work = 5000;
		Task_Create (Config);
	}
	
	
	{
		struct Task_Config * Config = &Task_Config_2;
		Config->Start_Delay = 10;
		Config->Handle = NULL;
		Config->Priority = 3;
		Config->Name = "C";
		Config->Function = Task_Function_0;
		Config->Stack_Depth = configMINIMAL_STACK_SIZE;
		Config->Semaphore = Semaphore_0;
		Config->Semaphore_Timeout = 10000;
		Config->Work = 100;
		Config->Deadline = 150;
		Task_Create (Config);
	}

	
	
	gpio_clr_gpio_pin (LED4_GPIO);
	vTaskStartScheduler ();
}