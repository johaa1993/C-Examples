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
#include "spi.h"
#include "dip204.h"
#include "adc.h"
#include "delay.h"
#include "power_clocks_lib.h"
#include "intc.h"
#include "gpio.h"
#include "usart.h"

struct Task_Config
{
	portTickType Start_Delay;
	char * Name;
	int Priority;
	xTaskHandle Handle;
	pdTASK_CODE Function;
	unsigned short Stack_Depth;
};

void Task_Create (struct Task_Config * Config)
{
	xTaskCreate (Config->Function, Config->Name, Config->Stack_Depth, Config, tskIDLE_PRIORITY + Config->Priority, &Config->Handle);
}

xQueueHandle USART_Queue;
struct Task_Config Task_Config_0;
struct Task_Config Task_Config_1;



__attribute__((__noinline__))
static portBASE_TYPE USART_Magic ();

__attribute__((__naked__))
static void USART_Interrupt_0 (void);


void USART_Initialize ()
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
	//Disable_global_interrupt ();
	{
		gpio_enable_module(USART_GPIO_MAP, sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));
		usart_init_rs232 (&AVR32_USART0, &USART_OPTIONS, FOSC0);
		(&AVR32_USART0)->cr |= AVR32_USART_CR_RXDIS_MASK | AVR32_USART_CR_TXDIS_MASK;
		//INTC_init_interrupts ();
		INTC_register_interrupt (&USART_Interrupt_0, AVR32_USART0_IRQ, AVR32_INTC_INT0);
		(&AVR32_USART0)->ier = AVR32_USART_IER_RXRDY_MASK;
		(&AVR32_USART0)->cr |= AVR32_USART_CR_TXEN_MASK | AVR32_USART_CR_RXEN_MASK;
	}
	//Enable_global_interrupt();
	portEXIT_CRITICAL();
	
	
}









signed portBASE_TYPE USART_Magic_1()
{
	int Character;
	signed portBASE_TYPE Woken = pdFALSE;
	int Status = usart_read_char (&AVR32_USART0, &Character);
	if (Status == USART_SUCCESS)
	{
		if (xQueueSendToBackFromISR (USART_Queue, &Character, &Woken) != pdPASS)
		{
			gpio_clr_gpio_pin (LED4_GPIO);
		}
	}
	return Woken;
}





__attribute__((__naked__))
static void USART_Interrupt_0 (void)
{
	//portENTER_CRITICAL ();
	//portSAVE_CONTEXT_OS_INT ();
	portENTER_SWITCHING_ISR ();
	//USART_Magic ();
	
	/*
	int Character;
	signed portBASE_TYPE Woken = pdFALSE;
	int Status = usart_read_char (&AVR32_USART0, &Character);
	if (Status == USART_SUCCESS)
	{
		if (xQueueSendToBackFromISR (USART_Queue, &Character, &Woken) != pdPASS)
		{
			gpio_clr_gpio_pin (LED4_GPIO);
		}
	}
	*/
	
	/*
	
	
	signed portBASE_TYPE Woken = pdFALSE;
	int Character;
	
	int Status = usart_read_char (&AVR32_USART0, &Character);
	if (Status == USART_SUCCESS)
	{
		if (xQueueSendToBackFromISR (USART_Queue, &Character, &Woken) != pdPASS)
		{
			gpio_clr_gpio_pin (LED4_GPIO);
		}
	}

	*/
	
	USART_Magic_1 ();
	
	//gpio_tgl_gpio_pin (LED6_GPIO);
	
	portEXIT_SWITCHING_ISR ();
	//portRESTORE_CONTEXT ();
	//portEXIT_CRITICAL ();
}


__attribute__((__noinline__))
static portBASE_TYPE USART_Magic ()
{
	signed portBASE_TYPE Woken = pdFALSE;
	int Character;
	int status = usart_read_char (&AVR32_USART0, &Character);
	if (status == USART_SUCCESS)
	{
		if (xQueueSendToBackFromISR (USART_Queue, &Character, &Woken) != pdPASS)
		{
			gpio_clr_gpio_pin (LED4_GPIO);
		}
	}
	else if (status == USART_RX_EMPTY)
	{
		usart_write_line (&AVR32_USART0, "USART_RX_EMPTY\n");
	}
	else if (status == USART_RX_ERROR)
	{
		usart_write_line (&AVR32_USART0, "USART_RX_ERROR\n");
	}
	gpio_tgl_gpio_pin (LED6_GPIO);
}




void Task_0_Function (void * Arguments)
{
	while (1)
	{
		vTaskDelay (100);
		gpio_tgl_gpio_pin (LED0_GPIO);
	}
}

void Task_Visual_Function (void * Arguments)
{
	int C;
	while (1)
	{
		gpio_tgl_gpio_pin (LED1_GPIO);
        while (xQueueReceive (USART_Queue, &C, 0))
        {
			vTaskDelay (100);
			dip204_write_data (C);
        }
		vTaskDelay (500);
	}
}






int main(void)
{

	pcl_switch_to_osc (PCL_OSC0, FOSC0, OSC0_STARTUP);
	
	
	USART_Initialize ();

	

	//usart_write_line (&AVR32_USART0, "USART initialized\n");
	


	static const gpio_map_t DIP204_SPI_GPIO_MAP =
	{
		{ DIP204_SPI_SCK_PIN, DIP204_SPI_SCK_FUNCTION },
		{ DIP204_SPI_MISO_PIN, DIP204_SPI_MISO_FUNCTION },
		{ DIP204_SPI_MOSI_PIN, DIP204_SPI_MOSI_FUNCTION },
		{ DIP204_SPI_NPCS_PIN, DIP204_SPI_NPCS_FUNCTION }
	};
	spi_options_t spiOptions =
	{
		.reg                  = DIP204_SPI_NPCS,
		.baudrate             = 1000000,
		.bits                 = 8,
		.spck_delay           = 0,
		.trans_delay  		  = 8,
		.stay_act             = 1,
		.spi_mode             = 0,
		.modfdis              = 1
	};
	pm_enable_osc0_crystal (&AVR32_PM, FOSC0);
	pm_enable_clk0 (&AVR32_PM, OSC0_STARTUP);
	pm_switch_to_clock (&AVR32_PM, AVR32_PM_MCSEL_OSC0);
	delay_init (12000000);
	delay_ms (1);
	gpio_enable_module (DIP204_SPI_GPIO_MAP, sizeof(DIP204_SPI_GPIO_MAP) / sizeof(DIP204_SPI_GPIO_MAP[0]));
	spi_initMaster (DIP204_SPI, &spiOptions);
	spi_selectionMode (DIP204_SPI, 0, 0, 0);
	spi_enable (DIP204_SPI);
	spi_setupChipReg (DIP204_SPI, &spiOptions, FOSC0);
	dip204_init (backlight_IO, true);
	dip204_clear_display ();
	dip204_hide_cursor ();
	dip204_set_cursor_position (1, 1);
	
	
	//usart_write_line (&AVR32_USART0, "dip204 initialized\n");
	dip204_write_string ("dip204 initialized");
	
	if (0)
	{
		int Character;
		int Status;
		while (1)
		{
			Status = usart_read_char (&AVR32_USART0, &Character);
			switch (Status)
			{
				case USART_SUCCESS:
					dip204_write_data (Character);
				break;
				
				case USART_RX_EMPTY:
					//dip204_write_byte (&AVR32_USART0, Character);
				break;
				
				case USART_RX_ERROR:
					dip204_write_string ("ERROR");
					while (1);
				break;
			}
		}
	}

	
	
	
	USART_Queue = xQueueCreate (10, sizeof(int));

	if (1)
	{
		struct Task_Config * Config = &Task_Config_0;
		Config->Start_Delay = 1000;
		Config->Handle = NULL;
		Config->Priority = 4;
		Config->Name = "0";
		Config->Function = Task_0_Function;
		Config->Stack_Depth = 512;
		Task_Create (Config);
	}
	
	if (1)
	{
		struct Task_Config * Config = &Task_Config_1;
		Config->Start_Delay = 1000;
		Config->Handle = NULL;
		Config->Priority = 4;
		Config->Name = "Visual";
		Config->Function = Task_Visual_Function;
		Config->Stack_Depth = 512;
		Task_Create (Config);
	}
	
	//usart_write_line (&AVR32_USART0, "Scheduler starting\n");
	vTaskStartScheduler ();
	
	
	
	while(1){}
}