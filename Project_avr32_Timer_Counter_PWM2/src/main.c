/*
Write a program that set the intensity of a LED at 30%, 60% and 90% by setting the duty cycle of a PWM signal.
The timer module must be set in waveform mode.

Pressed PB0 = 30%
Pressed PB1 = 60%
Pressed PB2 = 90%
Otherwise   = 100%
*/


#include <stdint.h>
#include "EVK1100.h"



void Set_Float_Duty_Cycle (float Duty_Cycle)
{
	//avr32uc3a0512 page 650
	//Active = Register_A
	//Period = Register_C
	//Duty_Cycle = Active / Period
	#define Active Timer_Counter_Channel_Register_A (EVK1100_Timer_Counter_Channel_LED_1)
	#define Period Timer_Counter_Channel_Register_C (EVK1100_Timer_Counter_Channel_LED_1)
	#define Second Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Second
	//#define Duty_Cycle 0.3
	
	Period = Second;
	Active = Duty_Cycle * Period;
	
	#undef Active
	#undef Period
	#undef Second
	//#undef Duty_Cycle
}



int main (void)
{
	GPIO_GPIO_Enable_Register_Clear (GPIO_Port_1) = EVK1100_LED_1;
	GPIO_Peripheral_Mux_Register_0_Clear(GPIO_Port_1) = EVK1100_LED_1;
	GPIO_Peripheral_Mux_Register_1_Clear(GPIO_Port_1) = EVK1100_LED_1;
	
	Timer_Counter_Channel_Mode_Register (EVK1100_Timer_Counter_Channel_LED_1) =
	Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Mask |
	Timer_Counter_Channel_Mode_Register_Waveform_Mode_Enable_Mask |
	Timer_Counter_Channel_Mode_Register_Waveform_Selection_Up_Mode_Register_C_Trigger_Mask |
	Timer_Counter_Channel_Mode_Register_ACPA_Set_Mask |
	Timer_Counter_Channel_Mode_Register_ACPC_Clear_Mask;
		
	Timer_Counter_Channel_Control_Register (EVK1100_Timer_Counter_Channel_LED_1) =
	Timer_Counter_Channel_Control_Register_Counter_Clock_Enable_Command_Offset_Mask |
	Timer_Counter_Channel_Control_Register_Software_Trigger_Command_Offset_Mask;
		

	Timer_Counter_Channel_Interrupt_Enable_Register (EVK1100_Timer_Counter_Channel_LED_1) =
	Timer_Counter_Channel_Interrupt_Enable_Register_Register_C_Compare_Mask;

	while (1)
	{
		switch (EVK1100_PB_Pressed)
		{
			case EVK1100_PB_0:
				Set_Float_Duty_Cycle (0.3f);
				break;
			case EVK1100_PB_1:
				Set_Float_Duty_Cycle (0.6f);
				break;
			case EVK1100_PB_2:
				Set_Float_Duty_Cycle (0.9f);
				break;
			default:
				Set_Float_Duty_Cycle (1.0f);
				break;
		}
	};
}