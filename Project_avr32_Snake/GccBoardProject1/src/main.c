#include <asf.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


#include "Snake.h"

#define CYCLES_PER_MS (115/11)
void mdelay(int milliseconds)
{
	long volatile cycles = (milliseconds * CYCLES_PER_MS);
	while (cycles != 0)
	cycles--;
}



void display_init(void)
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
	sizeof(DIP204_SPI_GPIO_MAP) / 	sizeof(DIP204_SPI_GPIO_MAP[0]));

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



#define LCD_Width 20
#define LCD_Height 4
#define LCD_XY_Index(X,Y) (X + (LCD_Width * Y))

#define Appearance_Apple '¤'
#define Appearance_Nothing ' '
#define Appearance_Snake_Head 'O'
#define Appearance_Snake_Body '+'


int Game_Points = 0;

void Game_Over()
{
	dip204_clear_display();
	while(1)
	{
		dip204_set_cursor_position (1, 1);
		dip204_write_string("Game Over");
		dip204_set_cursor_position (1, 2);
		dip204_printf_string("Score %i", Game_Points);
	}
}

void Game_Spawn_Green_Apple (char * Map)
{
	int Y = rand() % LCD_Height;
	int X = rand() % LCD_Width;
	Map[LCD_XY_Index(X, Y)] = Appearance_Apple;
}

void Game_Update (char * Map, struct Snake * S)
{
	Map[LCD_XY_Index(Snake_Head_X (S), Snake_Head_Y (S))] = Appearance_Snake_Body;
	int Direction = Snake_Head_Update (S);
	if(Direction != Snake_Direction_None)
	{
		
		//Optional
		Snake_Flat_Torus_Logic (S, LCD_Width, LCD_Height);
		
		if (Snake_Collision_Wall (S, LCD_Width, LCD_Height) || Snake_Collision_Self (S))
		{
			Game_Over ();
		}
		
		if (Map[LCD_XY_Index (Snake_Head_X (S), Snake_Head_Y (S))] == Appearance_Apple)
		{
			Game_Points++;
			Game_Spawn_Green_Apple (Map);
		}
		else
		{
			Map[LCD_XY_Index (Snake_Tail_X (S), Snake_Tail_Y (S))] = Appearance_Nothing;
			Snake_Tail_Update (S);
		}
	}
	Map[LCD_XY_Index(Snake_Head_X(S), Snake_Head_Y(S))] = Appearance_Snake_Head;
}




int main (void)
{
	char Map[LCD_Width * LCD_Height + 1];
	memset (Map, Appearance_Nothing, sizeof(Map));
	Map[LCD_Width * LCD_Height] = 0;
	
	srand (time (NULL));
	display_init ();
	
	struct Snake * My_Snake = Snake_malloc ();
	
	
	for (int i = 0; i < 10; i++)
	{
		Game_Spawn_Green_Apple (Map);
	}
	
	Game_Update (Map, My_Snake);
	
	gpio_enable_gpio_pin (GPIO_JOYSTICK_LEFT);
	gpio_enable_gpio_pin (GPIO_JOYSTICK_RIGHT);
	gpio_enable_gpio_pin (GPIO_JOYSTICK_UP);
	gpio_enable_gpio_pin (GPIO_JOYSTICK_DOWN);
	
	
	
	while(1)
	{
		
		
		if ( gpio_get_pin_value (GPIO_JOYSTICK_RIGHT) == 0 )
		{
			Snake_Set_Direction (My_Snake, Snake_Direction_Right);
		}
		else if ( gpio_get_pin_value (GPIO_JOYSTICK_LEFT) == 0 )
		{
			Snake_Set_Direction (My_Snake, Snake_Direction_Left);
		}
		else if ( gpio_get_pin_value (GPIO_JOYSTICK_UP) == 0 )
		{
			Snake_Set_Direction (My_Snake, Snake_Direction_Up);
		}
		else if ( gpio_get_pin_value (GPIO_JOYSTICK_DOWN) == 0 )
		{
			Snake_Set_Direction (My_Snake, Snake_Direction_Down);
		}
		else
		{
			Snake_Set_Direction (My_Snake, Snake_Direction_None);
		}
		
		Game_Update (Map, My_Snake);
		
		dip204_set_cursor_position (1, 1);
		dip204_write_string (Map);
		mdelay (10000);
	}

	
}