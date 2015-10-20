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


void USART_printf (volatile avr32_usart_t * usart, const char * format, ...)
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



int Byte_Buffer_Last = 0;
#define Byte_Buffer_Count 10
Byte Byte_Buffer[Byte_Buffer_Count];

Byte Produce_Byte ()
{
	vTaskDelay (100 + (rand() % 1050));
	return 1;
}

Byte Consume_Byte (Byte B)
{
	vTaskDelay (100 + (rand() % 1000));
}

void Byte_Buffer_Put (Byte B, Byte * Buffer)
{
	Buffer[Byte_Buffer_Last] = B;
}

Byte Byte_Buffer_Pull (Byte * Buffer)
{
	return Buffer[Byte_Buffer_Last];
}



xSemaphoreHandle Semaphore;

xTaskHandle Producer_Task_Handle;
xTaskHandle Consumer_Task_Handle;

void Producer_Task_Function (void * Parameters)
{
	for (;;)
	{
		USART_printf (configDBG_USART, "Producer_Task Byte_Buffer_Last = %i\n", Byte_Buffer_Last);
		Byte B = Produce_Byte ();
		if (Byte_Buffer_Last == Byte_Buffer_Count)
		{
			vTaskSuspend (Producer_Task_Handle);
		}
		
		while (xSemaphoreTake (Semaphore, 0) != pdPASS);
		Byte_Buffer_Put (Byte_Buffer, B);
		Byte_Buffer_Last = Byte_Buffer_Last + 1;
		xSemaphoreGive (Semaphore);
		
		if (Byte_Buffer_Last == 1)
		{
			vTaskResume (Consumer_Task_Handle);
		}
	}
	vTaskDelete (NULL);
}



void Consumer_Task_Function (void *Parameters)
{
	for (;;)
	{
		USART_printf (configDBG_USART, "Consumer_Task Byte_Buffer_Last = %i\n", Byte_Buffer_Last);
		if (Byte_Buffer_Last == 0)
		{
			//Deadlock if consumer resumes here.
			vTaskDelay (1 + (rand() % 100));
			vTaskSuspend (Consumer_Task_Handle);
		}
		while (xSemaphoreTake (Semaphore, 0) != pdPASS);
		Byte B = Byte_Buffer_Pull (Byte_Buffer);
		Byte_Buffer_Last = Byte_Buffer_Last - 1;
		xSemaphoreGive (Semaphore);
		if (Byte_Buffer_Last == Byte_Buffer_Count - 1)
		{
			vTaskResume (Producer_Task_Handle);
		}
		Consume_Byte (B);
	}
	vTaskDelete (NULL);
}



int main()
{
	Init_USART ();

	USART_printf (configDBG_USART, "USART is ready\n");

	vSemaphoreCreateBinary (Semaphore);
	if (Semaphore == NULL)
	{
		USART_printf (configDBG_USART, "vSemaphoreCreateBinary failed\n");
		for(;;);
	}
	xTaskCreate (Producer_Task_Function, "Producer_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &Producer_Task_Handle);
	xTaskCreate (Consumer_Task_Function, "Consumer_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &Consumer_Task_Handle);
	
	vTaskResume (Producer_Task_Handle);
	vTaskSuspend (Consumer_Task_Handle);
	
	gpio_clr_gpio_pin (LED4_GPIO);
	
	vTaskStartScheduler ();
}