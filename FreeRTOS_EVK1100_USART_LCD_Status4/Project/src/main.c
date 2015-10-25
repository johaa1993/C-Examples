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

struct Task_Configuration
{
	char * Name;
	int Priority;
	xTaskHandle Handle;
	pdTASK_CODE Function;
	unsigned short Depth;
};

static void Task_Create (struct Task_Configuration * Config)
{
	xTaskCreate (Config->Function, Config->Name, Config->Depth, Config, tskIDLE_PRIORITY + Config->Priority, &Config->Handle);
}

#define Task_Count 2
struct Task_Configuration Task_Config[Task_Count];

#define USART_0_Queue_Count 10
static xQueueHandle USART_0_Queue;

__attribute__((__noinline__))
static portBASE_TYPE USART_0_Interrupt_Magic ();

__attribute__((__naked__))
static void USART_0_Interrupt (void);

static void USART_Initialize ();

static void DIP204_Initialize ();

static void DIP204_Initialize ()
{
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
}

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
		INTC_register_interrupt (&USART_0_Interrupt, AVR32_USART0_IRQ, AVR32_INTC_INT0);
		(&AVR32_USART0)->ier = AVR32_USART_IER_RXRDY_MASK;
		(&AVR32_USART0)->cr |= AVR32_USART_CR_TXEN_MASK | AVR32_USART_CR_RXEN_MASK;
	}
	portEXIT_CRITICAL();
	USART_0_Queue = xQueueCreate (USART_0_Queue_Count, sizeof(int));
}


static void USART_0_Interrupt (void)
{
	portENTER_SWITCHING_ISR ();
	USART_0_Interrupt_Magic ();
	portEXIT_SWITCHING_ISR ();
}

signed portBASE_TYPE USART_0_Interrupt_Magic()
{
	int Character;
	signed portBASE_TYPE Woken = pdFALSE;
	int Status = usart_read_char (&AVR32_USART0, &Character);
	if (Status == USART_SUCCESS)
	{
		if (xQueueSendToBackFromISR (USART_0_Queue, &Character, &Woken) != pdPASS)
		{
			gpio_clr_gpio_pin (LED4_GPIO);
		}
	}
	return Woken;
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
        while (xQueueReceive (USART_0_Queue, &C, 0))
        {
			vTaskDelay (100);
			dip204_write_data (C);
        }
		vTaskDelay (500);
	}
}






int main(void)
{
	USART_Initialize ();
	usart_write_line (&AVR32_USART0, "USART initialized\n");
	DIP204_Initialize ();
	usart_write_line (&AVR32_USART0, "DIP204 initialized\n");
	
	

	if (1)
	{
		struct Task_Configuration * Config = &Task_Config[0];
		Config->Handle = NULL;
		Config->Priority = 4;
		Config->Name = "0";
		Config->Function = Task_0_Function;
		Config->Depth = 512;
		Task_Create (Config);
	}
	
	if (1)
	{
		struct Task_Configuration * Config = &Task_Config[1];
		Config->Handle = NULL;
		Config->Priority = 4;
		Config->Name = "Visual";
		Config->Function = Task_Visual_Function;
		Config->Depth = 512;
		Task_Create (Config);
	}
	
	vTaskStartScheduler ();
	
	while(1){}
}