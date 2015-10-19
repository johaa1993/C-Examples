#include <FreeRTOS.h>
#include <task.h>
#include <ASF/avr32/drivers/gpio/gpio.h>


#define Mode_Blink 1
#define Mode_Lit 2

struct Task_LED_Config
{
	int Mode;
	int LED;
	int Blink_Delay;
};



void Task_LED (void *Parameters)
{
	struct Task_LED_Config * Config = (struct Task_LED_Config *) Parameters;
	for (;;)
	{
		switch (Config->Mode)
		{
			case Mode_Blink:
			gpio_tgl_gpio_pin (Config->LED);
			vTaskDelay (Config->Blink_Delay);
			break;
			case Mode_Lit:
			gpio_clr_gpio_pin (Config->LED);
			vTaskDelay (100);
			Config->Mode = Mode_Blink;
			break;
		}
	}
	vTaskDelete (NULL);
}


struct Task_LED_Config Task_LED_0_Config = {.LED = LED0_GPIO, .Mode = Mode_Blink, .Blink_Delay = 5};
struct Task_LED_Config Task_LED_1_Config = {.LED = LED1_GPIO, .Mode = Mode_Blink, .Blink_Delay = 10};
struct Task_LED_Config Task_LED_2_Config = {.LED = LED2_GPIO, .Mode = Mode_Blink, .Blink_Delay = 15};
	
void Task_Button (void *Parameters)
{
	for (;;)
	{
		if (gpio_get_pin_value(GPIO_PUSH_BUTTON_0) == GPIO_PUSH_BUTTON_0_PRESSED)
		{
			Task_LED_0_Config.Mode = Mode_Lit;
		}
		if (gpio_get_pin_value(GPIO_PUSH_BUTTON_1) == GPIO_PUSH_BUTTON_1_PRESSED)
		{
			Task_LED_1_Config.Mode = Mode_Lit;
		}
		if (gpio_get_pin_value(GPIO_PUSH_BUTTON_2) == GPIO_PUSH_BUTTON_2_PRESSED)
		{
			Task_LED_2_Config.Mode = Mode_Lit;
		}
	}
	vTaskDelete (NULL);
};




int main()
{
	

	xTaskCreate (Task_LED, (const signed portCHAR *) "Task_LED0", configMINIMAL_STACK_SIZE, &Task_LED_0_Config, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Task_LED, (const signed portCHAR *) "Task_LED1", configMINIMAL_STACK_SIZE, &Task_LED_1_Config, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Task_LED, (const signed portCHAR *) "Task_LED2", configMINIMAL_STACK_SIZE, &Task_LED_2_Config, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Task_Button, (const signed portCHAR *) "Task_Button", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	
	gpio_clr_gpio_pin (LED4_GPIO);
	
	vTaskStartScheduler ();
}