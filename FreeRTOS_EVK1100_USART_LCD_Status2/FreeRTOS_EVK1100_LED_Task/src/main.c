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

void USART_Init ()
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


void Display_Init ()
{
	volatile avr32_pm_t* pm = &AVR32_PM;

	// Display pin map
	static const gpio_map_t DIP204_SPI_GPIO_MAP =
	{
		{ DIP204_SPI_SCK_PIN, DIP204_SPI_SCK_FUNCTION },
		{ DIP204_SPI_MISO_PIN, DIP204_SPI_MISO_FUNCTION },
		{ DIP204_SPI_MOSI_PIN, DIP204_SPI_MOSI_FUNCTION },
		{ DIP204_SPI_NPCS_PIN, DIP204_SPI_NPCS_FUNCTION }
	};

	/* SPI options for LCD */
	spi_options_t spiOptions =
	{
		.reg                  = DIP204_SPI_NPCS,
		.baudrate             = 1000000, // min 1 us max 20 us
		.bits                 = 8,
		.spck_delay           = 0,
		.trans_delay  		 = 8,
		.stay_act             = 1,
		.spi_mode             = 0,
		.modfdis              = 1
	};

	// Switch clock to external 12 MHz crystal
	// 1) Configure OSC0 in crystal mode, external crystal with a FOSC0 Hz frequency.
	pm_enable_osc0_crystal(pm, FOSC0);

	// 2) Enable the OSC0
	pm_enable_clk0(pm, OSC0_STARTUP);

	// 3) Set the main clock source as being OSC0.
	pm_switch_to_clock(pm, AVR32_PM_MCSEL_OSC0);

	// Initialize the delay function
	delay_init(12000000);
	// Delay for a ms, to allow the display to power up properly
	delay_ms(1);

	// Enable the SPI peripheral on GPIO pins
	gpio_enable_module(DIP204_SPI_GPIO_MAP,
	sizeof(DIP204_SPI_GPIO_MAP) / sizeof(DIP204_SPI_GPIO_MAP[0]));

	// Initialize as SPI master
	spi_initMaster(DIP204_SPI, &spiOptions);
	spi_selectionMode(DIP204_SPI, 0, 0, 0);
	spi_enable(DIP204_SPI);
	spi_setupChipReg(DIP204_SPI, &spiOptions, FOSC0);

	dip204_init(backlight_IO, true);
	dip204_clear_display();
	dip204_hide_cursor();
	dip204_set_cursor_position(1,1);
}


#define Bounded_String_Count 20
struct Bounded_String
{
	char Data[Bounded_String_Count];
	int Last;
};

struct Bounded_String Bounded_String_1;

int USART_Bounded_String_Receive (volatile avr32_usart_t * USART, Bounded_String * String)
{
	if ()
	{
		
	}
	String->Last = String->Last + 1;
	String->Data[String->Last] = usart_getchar (USART);
	String->Data[String->Last + 1] = '\0';
}



void Receiver_Task (void * Arguments)
{
	Bounded_String_1.Last = -1;
	for (;;)
	{
		USART_Bounded_String_Receive (configDBG_USART, &Bounded_String_1);
	}
}

void Status_Task (void * Arguments)
{
	for (;;)
	{
		
	}
}



int main()
{
	USART_Init ();
	usart_write_line (configDBG_USART, "USART is ready\n");

	adc_configure (&AVR32_ADC);
	adc_enable (&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
	adc_enable (&AVR32_ADC, ADC_LIGHT_CHANNEL);
	adc_enable (&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);

	Display_Init ();
	usart_write_line (configDBG_USART, "Display_Init is ready\n");

	
	Sensor_Queue = xQueueCreate (Sensor_Queue_Count, sizeof (struct Sensor_Message));

	struct Sensor_Config Light_Config = {.ADC = &AVR32_ADC, .Queue = Sensor_Queue, .Timeout = 10, .Delay = 50, .Channel = ADC_LIGHT_CHANNEL, .Row = 1, .Format = "Lum %04lu", .LED = LED1_GPIO};
	struct Sensor_Config Temperature_Config = {.ADC = &AVR32_ADC, .Queue = Sensor_Queue, .Timeout = 10, .Delay = 50, .Channel = ADC_TEMPERATURE_CHANNEL, .Row = 2, .Format = "Tmp %04lu", .LED = LED2_GPIO};
	struct Sensor_Config Potentiometer_Config = {.ADC = &AVR32_ADC, .Queue = Sensor_Queue, .Timeout = 10, .Delay = 50, .Channel = ADC_POTENTIOMETER_CHANNEL, .Row = 3, .Format = "Pot %04lu", .LED = LED3_GPIO};
	struct Presentation_Config Presentation_Config_1 = {.Queue = Sensor_Queue, .Timeout = 10, .Delay = 20, .LED = LED0_GPIO};
		
	xTaskCreate (Presentation_Task_Function, "Pre", configMINIMAL_STACK_SIZE, &Presentation_Config_1, tskIDLE_PRIORITY + 1, &Presentation_Config_1.Handle);
	xTaskCreate (Sensor_Task_Function, "Lum", configMINIMAL_STACK_SIZE, &Light_Config, tskIDLE_PRIORITY + 1, &Light_Config.Handle);
	xTaskCreate (Sensor_Task_Function, "Tmp", configMINIMAL_STACK_SIZE, &Temperature_Config, tskIDLE_PRIORITY + 1, &Light_Config.Handle);
	xTaskCreate (Sensor_Task_Function, "Pot", configMINIMAL_STACK_SIZE, &Potentiometer_Config, tskIDLE_PRIORITY + 1, &Light_Config.Handle);
	

	vTaskStartScheduler ();
}