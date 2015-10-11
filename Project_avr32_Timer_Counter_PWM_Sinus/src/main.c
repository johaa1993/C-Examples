/*
Often it is needed to generate a sinus signal, for example to drive a motor or a speaker.
However not all microcontroller implement modules which offer such functionality.
For the optional assignment you will modulate a sinus signal using the functionality you implemented in the main assignment.
To do this we use the Digital Direct Synthesis (DDS) method. Divide the time domain in even slots (the period of the PWM).
For each slot the duty cycle of the PWM is set to match the average value of the sine wave in the same slot.
Figure 6 depicts both, the sine wave and the modulated curve.
Note: A lookup table can be used to hold the duty cycle values for each slot.
Make sure to select a suitable frequency of your output signal and a suitable slot size.
The modulated curve can be transformed into a clean sinus signal by connecting a lowpass filter
(usually a Chebyshef Lowpass filter with 12.5 KHz can be used for sampling frequency of 32 KHz).
Since this is hardware based we do not do this for this optional assignment.
*/

#define Pi 3.14159265
#define Millisecond 1000

#include <stdint.h>
#include <math.h>
#include "EVK1100.h"


void Delay_Millisecond (int milliseconds)
{
	#define Instruction_Cycles 11
	long volatile cycles = (milliseconds * Oscillator_Frequency / (Instruction_Cycles * Millisecond));
	while (cycles != 0)
	cycles--;
	#undef Instruction_Cycles
}

void Ring_Pointer_Forward (int Count, int * Index)
{
	(*Index)++;
	if ((*Index) == Count)
	{
		(*Index) = 0;
	}
}


void Generate_Sinusoid (int Count, float * Result)
{
	const float Incrament = ((2.0f * Pi) / Count);
	float Time = 0.0f;
	for (int Index = 0; Index < Count; Index++)
	{
		Result[Index] = (sin(Incrament * Index) + 1.0f) / 2.0f;
	}
}

#define Sinusoid_Resolution 100
float Sinusoid[Sinusoid_Resolution] = {0.0f};
int Sinusoid_Pointer;
#define Sinusoid_Current Sinusoid[Sinusoid_Pointer]
#define Sinusoid_Forward Ring_Pointer_Forward(Sinusoid_Resolution, &Sinusoid_Pointer)




void Set_Float_Duty_Cycle (float Duty_Cycle)
{
	//avr32uc3a0512 page 650
	//Active = Register_A
	//Period = Register_C
	//Duty_Cycle = Active / Period
	#define LED_1_Active Timer_Counter_Channel_Register_A (EVK1100_Timer_Counter_Channel_LED_1)
	#define LED_1_Period Timer_Counter_Channel_Register_C (EVK1100_Timer_Counter_Channel_LED_1)
	LED_1_Period = (Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_2_Second / Millisecond);
	LED_1_Active = Duty_Cycle * LED_1_Period;
	
	//Zero active is full activity.
	if (LED_1_Active == 0)
	{
		LED_1_Active = 1;
	}
	
	#undef LED_1_Active
	#undef LED_1_Period
	//#undef Duty_Cycle
}



int main (void)
{
	
	Generate_Sinusoid (Sinusoid_Resolution, Sinusoid);
	
	GPIO_GPIO_Enable_Register_Clear (GPIO_Port_1) = EVK1100_LED_1;
	GPIO_Peripheral_Mux_Register_0_Clear(GPIO_Port_1) = EVK1100_LED_1;
	GPIO_Peripheral_Mux_Register_1_Clear(GPIO_Port_1) = EVK1100_LED_1;
	
	Timer_Counter_Channel_Mode_Register (EVK1100_Timer_Counter_Channel_LED_1) =
	Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_2_Mask |
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
				Sinusoid_Forward;
				Delay_Millisecond (100);
				break;
			case EVK1100_PB_1:
				Sinusoid_Forward;
				Delay_Millisecond (10);
				break;
			case EVK1100_PB_2:
				Sinusoid_Forward;
				Delay_Millisecond (1);
				break;
			default:
				break;
		}
		
		Set_Float_Duty_Cycle (Sinusoid_Current);
	};
}