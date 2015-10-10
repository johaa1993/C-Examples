#include <stdint.h>
#include "EVK1100.h"

#define My_Channel 2

int main (void)
{
	GPIO_GPIO_Enable_Register_Clear (GPIO_Port_1) = EVK1100_LED_1;
	GPIO_Peripheral_Mux_Register_0_Clear(GPIO_Port_1) = EVK1100_LED_1;
	GPIO_Peripheral_Mux_Register_1_Clear(GPIO_Port_1) = EVK1100_LED_1;
	
	Timer_Counter_Channel_Mode_Register (My_Channel) =
	Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Mask |
	Timer_Counter_Channel_Mode_Register_Waveform_Mode_Enable_Mask |
	Timer_Counter_Channel_Mode_Register_Waveform_Selection_Up_Mode_Register_C_Trigger_Mask |
	Timer_Counter_Channel_Mode_Register_ACPA_Set_Mask |
	Timer_Counter_Channel_Mode_Register_ACPC_Clear_Mask;
		
	//Timer_Counter_Channel_Register_A (My_Channel) = Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Second;
	//Timer_Counter_Channel_Register_C (My_Channel) = Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Second;
		

	//Page 650
	Timer_Counter_Channel_Register_A (My_Channel) = Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Second;
	Timer_Counter_Channel_Register_C (My_Channel) = Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Second * 2;
		
		
	Timer_Counter_Channel_Control_Register (My_Channel) =
	Timer_Counter_Channel_Control_Register_Counter_Clock_Enable_Command_Offset_Mask |
	Timer_Counter_Channel_Control_Register_Software_Trigger_Command_Offset_Mask;
		

	Timer_Counter_Channel_Interrupt_Enable_Register (My_Channel) =
	Timer_Counter_Channel_Interrupt_Enable_Register_Register_C_Compare_Mask;




	while (1)
	{
	
		if (EVK1100_PB_Pressed)
		{
			EVK1100_LED_Toggle (EVK1100_LED_2);
		}
		
		if (EVK1100_Joystick_Pressed)
		{
			EVK1100_LED_Toggle (EVK1100_LED_3);
		}
		
	};
}