#include <avr32/io.h>
#include <stdio.h>
#include <stdarg.h>
#include <FreeRTOS.h>
#include <task.h>
#include "compiler.h"
#include "board.h"
#include "gpio.h"
#include "pm.h"
#include "usart.h"
#include "semphr.h"
#include "power_clocks_lib.h"
#include "intc.h"
#include "gpio.h"
#include "usart.h"




typedef int Item;
#define Item_Buffer_Count 5
int Item_Buffer [Item_Buffer_Count];
int Item_Buffer_Last = 0;

Item Item_Produce ()
{
	vTaskDelay (10 + (rand() % 200));
	return 1;
}

void Item_Consume (Item X)
{
	vTaskDelay (10 + (rand() % 200));
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


xTaskHandle Producer_Task_Handle;
xTaskHandle Consumer_Task_Handle;


void Producer_Task_Function (void * Parameters)
{
	Item X;
	for (;;)
	{
		gpio_tgl_gpio_pin (LED4_GPIO);
		X = Item_Produce ();
		if (Item_Buffer_Last == Item_Buffer_Count)
		{
			vTaskSuspend (NULL);
		}
		Item_Store (X);
		//Item_Buffer_Last = 2
		if (Item_Buffer_Last == 1)
		{
			vTaskResume (Consumer_Task_Handle);
		}
	}
	vTaskDelete (NULL);
}



void Consumer_Task_Function (void *Parameters)
{
	Item X;
	for (;;)
	{
		gpio_tgl_gpio_pin (LED6_GPIO);
		if (Item_Buffer_Last == 0)
		{
			vTaskDelay (1 + (rand() % 10));
			//Deadlock if vTaskResume (Consumer_Task_Handle); is called.
			vTaskSuspend (NULL);
		}
		X = Item_Remove ();
		if (Item_Buffer_Last == Item_Buffer_Count - 1)
		{
			vTaskResume (Producer_Task_Handle);
		}
		Item_Consume (X);
	}
	vTaskDelete (NULL);
}



void Blinker_Task_Function (void * Parameters)
{
	while (1)
	{
		vTaskDelay (500);
		gpio_tgl_gpio_pin (LED0_GPIO);
	}
}


int main()
{
	
	pm_enable_osc0_crystal (&AVR32_PM, FOSC0);
	pm_enable_clk0 (&AVR32_PM, OSC0_STARTUP);
	pm_switch_to_clock (&AVR32_PM, AVR32_PM_MCSEL_OSC0);

	
	xTaskCreate (Producer_Task_Function, "Producer", 512, NULL, tskIDLE_PRIORITY + 1, &Producer_Task_Handle);
	xTaskCreate (Consumer_Task_Function, "Consumer", 512, NULL, tskIDLE_PRIORITY + 1, &Consumer_Task_Handle);
	xTaskCreate (Blinker_Task_Function, "Blinker", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
	
	
	//vTaskResume (Producer_Task_Handle);
	//vTaskSuspend (Consumer_Task_Handle);
	
	vTaskStartScheduler ();
	while(1){}
}