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
#include "spi.h"
#include "dip204.h"
#include "adc.h"
#include "delay.h"

struct DIP204_Message
{
	unsigned char Row;
	unsigned char Column;
	char Character;
};

#define DIP204_Queue_Count 100
static xQueueHandle DIP204_Queue;

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




#define USART_0_Queue_Count 20
static xQueueHandle USART_0_Queue;
static int USART_0_Receive_Count = 0;

__attribute__((__noinline__))
static portBASE_TYPE USART_0_Interrupt_Magic ();

__attribute__((__naked__))
static void USART_0_Interrupt (void);


static void USART_0_Interrupt (void)
{
	portENTER_SWITCHING_ISR ();
	USART_0_Interrupt_Magic ();
	portEXIT_SWITCHING_ISR ();
}

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
		(&AVR32_USART0)->ier = AVR32_USART_IER_RXRDY_MASK;
		(&AVR32_USART0)->cr |= AVR32_USART_CR_TXEN_MASK | AVR32_USART_CR_RXEN_MASK;
	}
	portEXIT_CRITICAL();
	USART_0_Queue = xQueueCreate (USART_0_Queue_Count, sizeof (int));
}








void Visual_Task_Function (void * Arguments)
{
	struct DIP204_Message * Message;
	while (1)
	{
		gpio_tgl_gpio_pin (LED1_GPIO);
		vTaskDelay (1);
		if (xQueueReceive (DIP204_Queue, Message, 100))
		{
			dip204_set_cursor_position (Message->Column, Message->Row);
			dip204_write_data (Message->Character);
		}
	}
}






struct Sensor_Config
{
	uint16_t Channel;
	unsigned char Column;
	unsigned char Row;
};

void Sensor_Task_Function (void * Parameters)
{
	#define ADC &AVR32_ADC
	#define Buffer_Count 5
	#define Buffer_Format "%04i"
	char Buffer[Buffer_Count];
	struct Sensor_Config * Config = (struct Sensor_Config *) Parameters;
	while (1)
	{
		vTaskDelay (10);
		adc_start (ADC);
		snprintf (Buffer, Buffer_Count, Buffer_Format, adc_get_value (ADC, Config->Channel));
		DIP204_Send_String (Config->Row, Config->Column, Buffer);
	}
}





int main()
{
	
	pm_enable_osc0_crystal (&AVR32_PM, FOSC0);
	pm_enable_clk0 (&AVR32_PM, OSC0_STARTUP);
	pm_switch_to_clock (&AVR32_PM, AVR32_PM_MCSEL_OSC0);

	USART_Initialize ();
	DIP204_Initialize ();

	usart_write_line (&AVR32_USART0, "USART initialized\n");
	usart_write_line (&AVR32_USART0, "DIP204 initialized\n");
	
	adc_configure (&AVR32_ADC);
	adc_enable (&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
	adc_enable (&AVR32_ADC, ADC_LIGHT_CHANNEL);
	adc_enable (&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);


	xTaskCreate (Visual_Task_Function, "Visual", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
	

	
	struct Sensor_Config Temperature_Config = {.Channel = ADC_TEMPERATURE_CHANNEL, .Row = 1, .Column = 8};
	struct Sensor_Config Potentiometer_Config = {.Channel = ADC_POTENTIOMETER_CHANNEL, .Row = 2, .Column = 8};
	struct Sensor_Config Light_Config = {.Channel = ADC_LIGHT_CHANNEL, .Row = 3, .Column = 8};
			
	xTaskCreate (Sensor_Task_Function, "Sensor 1", 512, &Temperature_Config, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Sensor_Task_Function, "Sensor 2", 512, &Potentiometer_Config, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate (Sensor_Task_Function, "Sensor 3", 512, &Light_Config, tskIDLE_PRIORITY + 1, NULL);
	
	dip204_set_cursor_position (1, Temperature_Config.Row);
	dip204_write_string ("Tmp");
	dip204_set_cursor_position (1, Potentiometer_Config.Row);
	dip204_write_string ("Pot");
	dip204_set_cursor_position (1, Light_Config.Row);
	dip204_write_string ("Lum");
	
	
	
	vTaskStartScheduler ();
}