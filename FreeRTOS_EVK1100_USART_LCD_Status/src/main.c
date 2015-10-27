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



#define USART_0_Queue_Count 20
static xQueueHandle USART_0_Queue;

__attribute__((__noinline__))
static portBASE_TYPE USART_0_Interrupt_Magic ();

__attribute__((__naked__))
static void USART_0_Interrupt (void);

static void USART_Initialize ();

struct DIP204_Message
{
	unsigned char Row;
	unsigned char Column;
	char Character;
};

#define DIP204_Queue_Count 100

static xQueueHandle DIP204_Queue;

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
	DIP204_Queue = xQueueCreate (DIP204_Queue_Count, sizeof(struct DIP204_Message));
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
	USART_0_Queue = xQueueCreate (USART_0_Queue_Count, sizeof (int));
}

static void USART_0_Interrupt (void)
{
	portENTER_SWITCHING_ISR ();
	USART_0_Interrupt_Magic ();
	portEXIT_SWITCHING_ISR ();
}

static int USART_0_Receive_Count = 0;

signed portBASE_TYPE USART_0_Interrupt_Magic()
{
	int Character;
	signed portBASE_TYPE Woken;
	portBASE_TYPE Queue_Status;
	int Read_Status;

	Read_Status = usart_read_char (&AVR32_USART0, &Character);
	if (Read_Status == USART_SUCCESS)
	{
		portENTER_CRITICAL ();
		Queue_Status = xQueueSendToBackFromISR (USART_0_Queue, &Character, &Woken);
		portENTER_CRITICAL ();
		if (Queue_Status == pdPASS)
		{
			USART_0_Receive_Count = USART_0_Receive_Count + 1;
		}
		else
		{
			gpio_clr_gpio_pin (LED4_GPIO);
		}
	}
	return Woken;
}





void DIP204_Queue_Send (struct DIP204_Message * Message)
{
	xQueueSendToBack (DIP204_Queue, Message, 100);
}

void DIP204_Send_String (int Row, int Col, char * Str)
{
	struct DIP204_Message Message;
	Message.Row = Row;
	Message.Column = Col;
	while (*Str)
	{
		Message.Character = *Str;
		DIP204_Queue_Send (&Message);
		Str = Str + 1;
		Message.Column = Message.Column + 1;
	}
}




void Task_0_Function (void * Arguments)
{
	struct DIP204_Message Message;
	Message.Character = 0;
	char Character = 0;
	while (1)
	{
		gpio_tgl_gpio_pin (LED0_GPIO);
		vTaskDelay (500);
		Message.Character = Character;
		Message.Column = (rand () % 20) + 1;
		Message.Row = (rand () % 4) + 1;
		DIP204_Queue_Send (&Message);
		vTaskDelay (500);
		Message.Character = ' ';
		DIP204_Queue_Send (&Message);
		Character = Character + 1;
	}
}

void Task_Visual_Function (void * Arguments)
{
	struct DIP204_Message * Message;
	while (1)
	{
		gpio_tgl_gpio_pin (LED1_GPIO);
		vTaskDelay (10);

        if (xQueueReceive (DIP204_Queue, Message, 100))
        {
			dip204_set_cursor_position (Message->Column, Message->Row);
			dip204_write_data (Message->Character);
        }
	}
}

void Task_Status_Function (void * Arguments)
{
	portBASE_TYPE Tick_Goal_Count = 0;

	#define Buffer_Count 4
	char Buffer[Buffer_Count];

	#define Status_Stand_By 0
	#define Status_Send_Info 1
	int Status = Status_Stand_By;

	while (1)
	{
		gpio_tgl_gpio_pin (LED2_GPIO);
		vTaskDelay (100);

		switch (Status)
		{

			case Status_Stand_By:
			if (gpio_get_pin_value(GPIO_PUSH_BUTTON_0) == GPIO_PUSH_BUTTON_0_PRESSED)
			{
				DIP204_Send_String (1, 1, "Count ");
				Tick_Goal_Count = xTaskGetTickCount () + 10000;
				Status = Status_Send_Info;
			}
			break;

			case Status_Send_Info:
				snprintf (Buffer, Buffer_Count, "%03i", USART_0_Receive_Count);
				DIP204_Send_String (1, 7, Buffer);
				if (Tick_Goal_Count < xTaskGetTickCount ())
				{
					DIP204_Send_String (1, 1, "          ");
					Status = Status_Stand_By;
				}
			break;
		}
	}
}

void Task_Display_USART_Queue_Function (void * Arguments)
{
	#define LCD_Row 2
	#define Signature_Clear '\0'
	struct DIP204_Message Message;
	int Character;
	Message.Column = 1;
	Message.Row = LCD_Row;
	while (1)
	{
		vTaskDelay (100);
		gpio_tgl_gpio_pin (LED3_GPIO);

		while (xQueueReceive (USART_0_Queue, &Character, 10))
		{
			vTaskDelay (300);
			usart_write_char (&AVR32_USART0, Character);
			if (Character == Signature_Clear)
			{
				DIP204_Send_String (LCD_Row, 1, "                     ");
				Message.Column = 1;
			}
			else
			{
				Message.Character = (char) Character;
				DIP204_Queue_Send (&Message);
				Message.Column = Message.Column + 1;
			}
		}
	}
}




int main(void)
{

	pm_enable_osc0_crystal (&AVR32_PM, FOSC0);
	pm_enable_clk0 (&AVR32_PM, OSC0_STARTUP);
	pm_switch_to_clock (&AVR32_PM, AVR32_PM_MCSEL_OSC0);

	USART_Initialize ();
	DIP204_Initialize ();

	usart_write_line (&AVR32_USART0, "USART initialized\n");
	usart_write_line (&AVR32_USART0, "DIP204 initialized\n");

	xTaskCreate (Task_0_Function, "0", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Task_Visual_Function, "Visual", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Task_Status_Function, "Status", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Task_Display_USART_Queue_Function, "USART_Queue", 512, NULL, tskIDLE_PRIORITY + 1, NULL);

	vTaskStartScheduler ();

	while(1){}
}
