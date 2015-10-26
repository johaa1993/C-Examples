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




#define Task_LED_Action_Blink 1
#define Task_LED_Action_Lit 2

struct Task_LED_Config
{
	int Action;
	int LED;
	int Blink_Delay;
};

void usart_printf (volatile avr32_usart_t * usart, const char * format, ...)
{
	#define Buffer_Size 100
	char buffer[Buffer_Size];
	va_list args;
	va_start (args, format);
	vsprintf (buffer, format, args);
	va_end (args);
	usart_write_line (usart, buffer);
	#undef Buffer_Size
};

void Task_LED (void *Parameters)
{
	struct Task_LED_Config * Config = (struct Task_LED_Config *) Parameters;
	for (;;)
	{
		switch (Config->Action)
		{
			case Task_LED_Action_Blink:
			usart_printf (configDBG_USART, "Task_LED_%i_Action_Blink\n", Config->LED);
			gpio_tgl_gpio_pin (Config->LED);
			vTaskDelay (Config->Blink_Delay);
			break;
			
			case Task_LED_Action_Lit:
			gpio_clr_gpio_pin (Config->LED);
			vTaskDelay (10000);
			Config->Action = Task_LED_Action_Blink;
			break;
		}
	}
	vTaskDelete (NULL);
}


struct Task_LED_Config Task_LED_0_Config = {.LED = LED0_GPIO, .Action = Task_LED_Action_Blink, .Blink_Delay = 1000};
struct Task_LED_Config Task_LED_1_Config = {.LED = LED1_GPIO, .Action = Task_LED_Action_Blink, .Blink_Delay = 2000};
struct Task_LED_Config Task_LED_2_Config = {.LED = LED2_GPIO, .Action = Task_LED_Action_Blink, .Blink_Delay = 3000};
	
void Task_Button (void *Parameters)
{
	for (;;)
	{
		if (Task_LED_0_Config.Action == Task_LED_Action_Blink && gpio_get_pin_value(GPIO_PUSH_BUTTON_0) == GPIO_PUSH_BUTTON_0_PRESSED)
		{
			Task_LED_0_Config.Action = Task_LED_Action_Lit;
			usart_printf (configDBG_USART, "Task_LED_0_Config.Action = Task_LED_Action_Lit\n");
		}
		if (Task_LED_1_Config.Action == Task_LED_Action_Blink && gpio_get_pin_value(GPIO_PUSH_BUTTON_1) == GPIO_PUSH_BUTTON_1_PRESSED)
		{
			Task_LED_1_Config.Action = Task_LED_Action_Lit;
			usart_printf (configDBG_USART, "Task_LED_1_Config.Action = Task_LED_Action_Lit\n");
		}
		if (Task_LED_2_Config.Action == Task_LED_Action_Blink && gpio_get_pin_value(GPIO_PUSH_BUTTON_2) == GPIO_PUSH_BUTTON_2_PRESSED)
		{
			Task_LED_2_Config.Action = Task_LED_Action_Lit;
			usart_printf (configDBG_USART, "Task_LED_2_Config.Action = Task_LED_Action_Lit\n");
		}
	}
	vTaskDelete (NULL);
};




int main()
{
	Init_USART ();

	usart_write_line (configDBG_USART, "USART is ready\n");

	xTaskCreate (Task_LED, (const signed portCHAR *) "Task_LED0", configMINIMAL_STACK_SIZE, &Task_LED_0_Config, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Task_LED, (const signed portCHAR *) "Task_LED1", configMINIMAL_STACK_SIZE, &Task_LED_1_Config, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Task_LED, (const signed portCHAR *) "Task_LED2", configMINIMAL_STACK_SIZE, &Task_LED_2_Config, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Task_Button, (const signed portCHAR *) "Task_Button", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	
	gpio_clr_gpio_pin (LED4_GPIO);
	
	vTaskStartScheduler ();
}