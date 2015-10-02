#include <asf.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gpio.h>
#include <limits.h>

#include "EVK1100_R1.h"

#define My_Timer_Counter_Channel 0
#define My_Timer_Counter_RC ((EVK1100_Internal_Frequency / 8))



volatile int Counter_4_Bit = 0;

__attribute__((__interrupt__)) static void IRQ_Handler (void)
{
	static int Last_Counter_4_Bit;
	static int Last_Bounce;
	
	if ((Counter_4_Bit - Last_Counter_4_Bit) > 1)
	{
		EVK1100_Port_B_Output_Value_Register_Toggle = EVK1100_Port_B_LED_7_Mask;
		Last_Bounce = Counter_4_Bit;
	}
	
	Last_Counter_4_Bit = Counter_4_Bit;
	EVK1100_Port_B_Output_Value_Register_Toggle = EVK1100_Port_B_LED_6_Mask;
	
	
	//TC Status Register
	//Note: Reading the Status Register will also clear the interrupt flag for the corresponding interrupts.
	AVR32_TC.channel[My_Timer_Counter_Channel].sr;
}




__attribute__((__interrupt__)) static void IRQ_Button (void)
{
	Counter_4_Bit = Counter_4_Bit + 1;
	if (Counter_4_Bit == 16)
	{
		Counter_4_Bit = 0;
	}
	
	//Turn off previous LED group state.
	//To find the on state of the LEDs we need to invert the Output Value Register.
	EVK1100_Port_B_Output_Value_Register_Set = (~EVK1100_Port_B_Output_Value_Register) & EVK1100_Port_B_LED_1_2_3_4_Mask;
	
	//Update LED group state so that it represent Counter_4_Bit.
	EVK1100_Port_B_Output_Value_Register_Clear = (Counter_4_Bit << EVK1100_Port_B_LED_1);
	
	
	EVK1100_Port_A_Interrupt_Flag_Register_Clear = EVK1100_Port_A_Joystick_Push_Mask;
}






static tc_waveform_opt_t waveform_opt =
{
	. channel = My_Timer_Counter_Channel , // Channel selection .
	. wavsel = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER ,
	. enetrg = 0 , // External event trigger enable .
	. eevt = 0, // External event selection .
	. eevtedg = TC_SEL_NO_EDGE , // External event edge selection .
	. cpcdis = 0 , // Counter disable when RC compare .
	. cpcstop = 0 , // Counter clock stopped with RC
	. burst = 0 , // Burst signal selection .
	. clki = 0 , // Clock inversion .
	. tcclks = TC_CLOCK_SOURCE_TC3 // Internal clock source 3,
};

static const tc_interrupt_t TC_INTERRUPT_OPT =
{
	. cpcs = 1, // RC compare status
};


#define My_Port_B_LEDs EVK1100_Port_B_LED_1_2_3_4_Mask | EVK1100_Port_B_LED_6_Mask | EVK1100_Port_B_LED_7_Mask;

int main (void)
{


	//Init Port B's LEDs and set them to off state.
	EVK1100_Port_B_GPIO_Enable_Register_Set = My_Port_B_LEDs;
	EVK1100_Port_B_Output_Driver_Enable_Register_Set = My_Port_B_LEDs;
	EVK1100_Port_B_Output_Value_Register_Set = My_Port_B_LEDs;


	//EVK1100_Port_A_Glitch_Filter_Enable_Register_Set = EVK1100_Port_A_Joystick_Push_Mask;
	EVK1100_Port_A_Output_Driver_Enable_Register_Clear = EVK1100_Port_A_Joystick_Push_Mask;
	EVK1100_Port_A_GPIO_Enable_Register_Set = EVK1100_Port_A_Joystick_Push_Mask;
	EVK1100_Port_A_Interrupt_Enable_Register_Set = EVK1100_Port_A_Joystick_Push_Mask;
	EVK1100_Port_A_Interrupt_Mode_Register_0_Clear = EVK1100_Port_A_Joystick_Push_Mask;
	EVK1100_Port_A_Interrupt_Mode_Register_1_Set = EVK1100_Port_A_Joystick_Push_Mask;

	
	



	cpu_irq_disable ();
	{
		
		EVK1100_Time_Channel_0_Clock_Mode_Register = 
			Time_Channel_Waveform_Selection_Up_Mode_With_Automatic_Trigger_On_RC_Compare_Mask;
		
		//EVK1100_Time_Channel_0_Waveform_Selection = Time_Channel_Waveform_Selection_Up_Mode_With_Automatic_Trigger_On_RC_Compare;
		
		tc_init_waveform (&AVR32_TC , &waveform_opt);
		tc_write_rc (&AVR32_TC, My_Timer_Counter_Channel, My_Timer_Counter_RC);
		
		// Enable, reset and start the selected timer/counter channel.
		AVR32_TC.channel[My_Timer_Counter_Channel].ccr = AVR32_TC_SWTRG_MASK | AVR32_TC_CLKEN_MASK;
		
		//tc_start (&AVR32_TC, Lab_3_TC_Channel);
		
		INTC_register_interrupt (&IRQ_Handler, AVR32_TC_IRQ0, AVR32_INTC_INT1);
		INTC_register_interrupt (&IRQ_Button, (AVR32_GPIO_IRQ_0 + EVK1100_Port_A_Joystick_Push/8), AVR32_INTC_INT0);
		tc_configure_interrupts (&AVR32_TC, My_Timer_Counter_Channel, &TC_INTERRUPT_OPT);
	}
	cpu_irq_enable ();




	while (1)
	{

	};
}