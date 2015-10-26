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
#include "queue.h"

static void USART_Initialize ()
{
	static const gpio_map_t USART_GPIO_MAP =
	{
		{AVR32_USART0_RXD_0_0_PIN, AVR32_USART0_RXD_0_0_FUNCTION},
		{AVR32_USART0_TXD_0_0_PIN, AVR32_USART0_TXD_0_0_FUNCTION}
	};
	static const usart_options_t USART_OPTIONS =
	{
		.baudrate     = 57600,
		.charlength   = 8,
		.paritytype   = USART_NO_PARITY,
		.stopbits     = USART_1_STOPBIT,
		.channelmode  = USART_NORMAL_CHMODE
	};
	portENTER_CRITICAL ();
	{
		gpio_enable_module(USART_GPIO_MAP, sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));
		usart_init_rs232 (&AVR32_USART0, &USART_OPTIONS, FOSC0);
		(&AVR32_USART0)->cr |= AVR32_USART_CR_RXDIS_MASK | AVR32_USART_CR_TXDIS_MASK;
		//INTC_init_interrupts ();
		//INTC_register_interrupt (&USART_0_Interrupt, AVR32_USART0_IRQ, AVR32_INTC_INT0);
		//(&AVR32_USART0)->ier = AVR32_USART_IER_RXRDY_MASK;
		(&AVR32_USART0)->cr |= AVR32_USART_CR_TXEN_MASK | AVR32_USART_CR_RXEN_MASK;
	}
	portEXIT_CRITICAL();
}

void USART_printf (volatile avr32_usart_t * usart, const char * format, ...)
{
	//taskENTER_CRITICAL();
	#define Buffer_Size 100
	char Buffer[Buffer_Size] = {'\0'};
	va_list args;
	va_start (args, format);
	vsprintf (Buffer, format, args);
	va_end (args);
	usart_write_line (usart, Buffer);
	#undef Buffer_Size
	//taskEXIT_CRITICAL();
};










typedef int Item;
#define Item_Buffer_Count 10
int Item_Buffer [Item_Buffer_Count];
int Item_Buffer_Last = 0;

Item Item_Produce ()
{
	vTaskDelay (10 + (rand() % 500));
	return 1;
}

void Item_Consume (Item X)
{
	vTaskDelay (10 + (rand() % 500));
}

void Item_Store (Item X)
{
	Item_Buffer [Item_Buffer_Last] = X;
	Item_Buffer_Last = Item_Buffer_Last + 1;
}

Item Item_Remove ()
{
	Item Result;
	Result = Item_Buffer[Item_Buffer_Last];
	Item_Buffer_Last = Item_Buffer_Last - 1;
	return Result;
}





xSemaphoreHandle Item_Available_Count;
xSemaphoreHandle Space_Available_Count;

xTaskHandle Producer_Task_Handle;
xTaskHandle Consumer_Task_Handle;


void Producer_Task_Function (void * Parameters)
{
	Item X;
	for (;;)
	{
		gpio_tgl_gpio_pin (LED4_GPIO);
		X = Item_Produce ();
		while (xSemaphoreTake (Space_Available_Count, 1000) != pdPASS);
		/*
		if (xSemaphoreTake (Space_Available_Count, 0) != pdPASS)
		{
			vTaskSuspend (NULL);
		}
		*/
		Item_Store (X);
        if( xSemaphoreGive( Item_Available_Count ) != pdTRUE )
        {
			usart_write_line (&AVR32_USART0, "xSemaphoreGive Item_Available_Count failed\n");
        }
		//vTaskResume (Consumer_Task_Handle);
	}
	vTaskDelete (NULL);
}



void Consumer_Task_Function (void *Parameters)
{
	Item X;
	for (;;)
	{
		gpio_tgl_gpio_pin (LED6_GPIO);
		while (xSemaphoreTake (Item_Available_Count, 1000) != pdPASS);
		/*
		if (xSemaphoreTake (Item_Available_Count, 0) != pdPASS)
		{
			vTaskSuspend (NULL);
		}
		*/
		X = Item_Remove ();
		xSemaphoreGive (Space_Available_Count);
		//vTaskResume (Producer_Task_Handle);
		Item_Consume (X);
	}
	vTaskDelete (NULL);
}




void Blinker_Task_Function (void *Parameters)
{
	char Buffer [3];
	for (;;)
	{
		vTaskDelay (1000);
		gpio_tgl_gpio_pin (LED0_GPIO);
		usart_write_line (&AVR32_USART0, "Last ");
		snprintf (Buffer, 3, "%02i", Item_Buffer_Last);
		usart_write_line (&AVR32_USART0, Buffer);
		usart_write_line (&AVR32_USART0, "\n");
	}
}











int main()
{
	pm_enable_osc0_crystal (&AVR32_PM, FOSC0);
	pm_enable_clk0 (&AVR32_PM, OSC0_STARTUP);
	pm_switch_to_clock (&AVR32_PM, AVR32_PM_MCSEL_OSC0);
	
	USART_Initialize ();
	
	usart_write_line (&AVR32_USART0, "USART initialized\n");
	usart_write_line (&AVR32_USART0, "DIP204 initialized\n");
	
	
	Item_Available_Count = xSemaphoreCreateCounting (Item_Buffer_Count, 0);
	Space_Available_Count = xSemaphoreCreateCounting (Item_Buffer_Count, Item_Buffer_Count - 1);
	
	xTaskCreate (Producer_Task_Function, "Producer", 512, NULL, tskIDLE_PRIORITY + 4, &Producer_Task_Handle);
	xTaskCreate (Consumer_Task_Function, "Consumer", 512, NULL, tskIDLE_PRIORITY + 4, &Consumer_Task_Handle);
	xTaskCreate (Blinker_Task_Function, "Blinker", 512, NULL, tskIDLE_PRIORITY + 4, NULL);


	vTaskStartScheduler ();
}