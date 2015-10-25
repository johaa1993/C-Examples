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
		INTC_init_interrupts ();
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


struct DIP204_Message
{
	unsigned char Row;
	unsigned char Column;
	char Text[20];
};
struct DIP204_Message DIP204_Message_Array[2];

static xQueueHandle DIP204_Queue;


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
	struct DIP204_Message * Message = &DIP204_Message_Array[0];
	while (1)
	{
        while (xQueueReceive (DIP204_Queue, &Message, 0))
        {
			dip204_set_cursor_position (Message->Column, Message->Row);
	        dip204_write_string (Message->Text);
        }
		vTaskDelay (1000);
		gpio_tgl_gpio_pin (LED1_GPIO);
	}
}

void Task_Status_Function (void * Arguments)
{
	struct DIP204_Message * Message = &DIP204_Message_Array[0];
	Message->Column = 1;
	Message->Row = 2;
	portBASE_TYPE Tick_Count = 0;
	while (1)
	{
		if (gpio_get_pin_value(GPIO_PUSH_BUTTON_0) == GPIO_PUSH_BUTTON_0_PRESSED)
		{
			Tick_Count = xTaskGetTickCount () + 10000;
		}
		if (Tick_Count > xTaskGetTickCount ())
		{
			sprintf (Message->Text, "Message count %i", uxQueueMessagesWaiting (USART_0_Queue));
		}
		else
		{
			sprintf (Message->Text, "               ");
		}
		xQueueSendToBack (DIP204_Queue, &Message, 0);
		gpio_tgl_gpio_pin (LED2_GPIO);
		vTaskDelay (100);
	}
}

void Task_Display_USART_Queue_Function (void * Arguments)
{
	struct DIP204_Message * Message = &DIP204_Message_Array[1];
	int Index = 0;
	int Character;
	while (1)
	{
		while (xQueueReceive (USART_0_Queue, &Character, 0))
		{
			Message->Column = 1;
			Message->Row = 1;
			Message->Text[Index] = Character;
			Index = Index + 1;
			xQueueSendToBack (DIP204_Queue, &Message, 10);
			vTaskDelay (300);
			if (Index == USART_0_Queue_Count)
			{
				Index = 0;
			}
		}
		vTaskDelay (1000);
		gpio_tgl_gpio_pin (LED3_GPIO);
	}
}





#define Task_Count 4
struct Task_Configuration Task_Config[Task_Count] = 
{
	{
		.Function = Task_0_Function,
		.Name = "0",
		.Priority = 4,
		.Depth = 512
	},
	{
		.Function = Task_Visual_Function,
		.Name = "Visual",
		.Priority = 4,
		.Depth = 512
	},
	{
		.Function = Task_Status_Function,
		.Name = "Status",
		.Priority = 4,
		.Depth = 512
	},
	{
		.Function = Task_Display_USART_Queue_Function,
		.Name = "USART_Queue",
		.Priority = 4,
		.Depth = 512
	}
};


int main(void)
{
	
	pm_enable_osc0_crystal (&AVR32_PM, FOSC0);
	pm_enable_clk0 (&AVR32_PM, OSC0_STARTUP);
	pm_switch_to_clock (&AVR32_PM, AVR32_PM_MCSEL_OSC0);
	
	USART_Initialize ();
	DIP204_Initialize ();
	
	usart_write_line (&AVR32_USART0, "USART initialized\n");
	usart_write_line (&AVR32_USART0, "DIP204 initialized\n");
	
	
	DIP204_Queue = xQueueCreate (10, sizeof(struct DIP204_Message));
	
	for (int I = 0; I < Task_Count; I = I + 1)
	{
		Task_Create (&Task_Config[I]);
	}
	vTaskStartScheduler ();
	
	while(1){}
}