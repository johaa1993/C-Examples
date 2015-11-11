#include "Base.h"

#ifndef TIMER_COUNTER_H_
#define TIMER_COUNTER_H_




#define Timer_Counter_Address 0xFFFF3800
#define Timer_Counter_Channel_Stride 0x40
#define Timer_Counter_Channel_0_Offset 0x00
#define Timer_Counter_Channel_1_Offset 0x40
#define Timer_Counter_Channel_2_Offset 0x80

#define Timer_Counter_Channel_1 1
#define Timer_Counter_Channel_2 2
#define Timer_Counter_Channel_3 3
#define Timer_Counter_Channel_4 4

#define Timer_Counter_Channel_PB27 Timer_Counter_Channel_2

/*
Channel Control Register. Write-only.
*/
#define Timer_Counter_Channel_CCR_Offset 0x00
#define Timer_Counter_Channel_Control_Register_Offset 0x00

#define Timer_Counter_Channel_Control_Register_Counter_Clock_Enable_Command_Offset 0
#define Timer_Counter_Channel_Control_Register_Counter_Clock_Disable_Command_Offset 1
#define Timer_Counter_Channel_Control_Register_Software_Trigger_Command_Offset 2

#define Timer_Counter_Channel_Control_Register_Counter_Clock_Enable_Command_Offset_Mask \
(1 << Timer_Counter_Channel_Control_Register_Counter_Clock_Enable_Command_Offset)

#define Timer_Counter_Channel_Control_Register_Counter_Clock_Disable_Command_Offset_Mask \
(1 << Timer_Counter_Channel_Control_Register_Counter_Clock_Disable_Command_Offset)

#define Timer_Counter_Channel_Control_Register_Software_Trigger_Command_Offset_Mask \
(1 << Timer_Counter_Channel_Control_Register_Software_Trigger_Command_Offset)










/*
Channel Mode Register. Read/Write.
*/
#define Timer_Counter_Channel_CMR_Offset 0x04
#define Timer_Counter_Channel_Mode_Register_Offset 0x04

/*
TC Channel Mode Register: Waveform Mode
TCCLKS: Clock Selection
*/
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Offset 0
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_1 0
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_2 1
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3 2
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_4 3
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_5 4
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_XC0 5
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_XC1 6
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_XC2 7

#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_1_Divider 1
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_2_Divider 2
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Divider 8
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_4_Divider 32
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_5_Divider 128

#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_1_Second \
(Oscillator_Frequency / Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_1_Divider)
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_2_Second \
(Oscillator_Frequency / Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_2_Divider)
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Second \
(Oscillator_Frequency / Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Divider)
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_4_Second \
(Oscillator_Frequency / Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_4_Divider)
#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_5_Second \
(Oscillator_Frequency / Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_5_Divider)



#define Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3_Mask \
(Timer_Counter_Channel_Mode_Register_Clock_Selection_Timer_Clock_3 << Timer_Counter_Channel_Mode_Register_Clock_Selection_Offset)




/*
TC Channel Mode Register: Waveform Mode
WAVSEL: Waveform Selection
UP mode with automatic trigger on RC Compare
*/
#define Timer_Counter_Channel_Mode_Register_Waveform_Selection_Offset 13
#define Timer_Counter_Channel_Mode_Register_Waveform_Selection_Up_Mode 0
#define Timer_Counter_Channel_Mode_Register_Waveform_Selection_Up_Mode_Register_C_Trigger 2
#define Timer_Counter_Channel_Mode_Register_Waveform_Selection_Up_Mode_Register_C_Trigger_Mask \
(Timer_Counter_Channel_Mode_Register_Waveform_Selection_Up_Mode_Register_C_Trigger << Timer_Counter_Channel_Mode_Register_Waveform_Selection_Offset)

/*
TC Channel Mode Register: Waveform Mode
WAVE = 1
0 = Waveform Mode is disabled (Capture Mode is enabled).
1 = Waveform Mode is enabled.
*/
#define Timer_Counter_Channel_Mode_Register_Waveform_Mode_Offset 15
#define Timer_Counter_Channel_Mode_Register_Waveform_Mode_Disable 0
#define Timer_Counter_Channel_Mode_Register_Waveform_Mode_Enable 1
#define Timer_Counter_Channel_Mode_Register_Waveform_Mode_Enable_Mask \
(Timer_Counter_Channel_Mode_Register_Waveform_Mode_Enable << Timer_Counter_Channel_Mode_Register_Waveform_Mode_Offset)




/*
ACPA
*/
#define Timer_Counter_Channel_Mode_Register_ACPA_Offset 16
#define Timer_Counter_Channel_Mode_Register_ACPA_Clear 2
#define Timer_Counter_Channel_Mode_Register_ACPA_Set 1
#define Timer_Counter_Channel_Mode_Register_ACPA_Set_Mask \
(Timer_Counter_Channel_Mode_Register_ACPA_Set << Timer_Counter_Channel_Mode_Register_ACPA_Offset)
#define Timer_Counter_Channel_Mode_Register_ACPA_Clear_Mask \
(Timer_Counter_Channel_Mode_Register_ACPA_Clear << Timer_Counter_Channel_Mode_Register_ACPA_Offset)
/*
ACPC
*/
#define Timer_Counter_Channel_Mode_Register_ACPC_Offset 18
#define Timer_Counter_Channel_Mode_Register_ACPC_Clear 2
#define Timer_Counter_Channel_Mode_Register_ACPC_Set 1
#define Timer_Counter_Channel_Mode_Register_ACPC_Set_Mask \
(Timer_Counter_Channel_Mode_Register_ACPC_Set << Timer_Counter_Channel_Mode_Register_ACPC_Offset)
#define Timer_Counter_Channel_Mode_Register_ACPC_Clear_Mask \
(Timer_Counter_Channel_Mode_Register_ACPC_Clear << Timer_Counter_Channel_Mode_Register_ACPC_Offset)










/*
Counter Value. Read-only
*/
#define Timer_Counter_Channel_CV_Offset 0x10
#define Timer_Counter_Channel_Counter_Value_Offset 0x10

/*
Register A. Read/Write.
*/
#define Timer_Counter_Channel_RA_Offset 0x14
#define Timer_Counter_Channel_Register_A_Offset 0x14

/*
Register B. Read/Write.
*/
#define Timer_Counter_Channel_RB_Offset 0x18
#define Timer_Counter_Channel_Register_B_Offset 0x18
/*
Register C. Read/Write.
*/
#define Timer_Counter_Channel_RC_Offset 0x1C
#define Timer_Counter_Channel_Register_C_Offset 0x1C
#define Timer_Counter_Channel_Register_C_Mask 0x0000ffff

/*
TC Status Register. Read-only.
Note: Reading the Status Register will also clear the interrupt flag for the corresponding interrupts.
*/
#define Timer_Counter_Channel_SR_Offset 0x20
#define Timer_Counter_Channel_Status_Register_Offset 0x20

/*
Interrupt Enable Register. Write-only
*/
#define Timer_Counter_Channel_IER_Offset 0x24
#define Timer_Counter_Channel_Interrupt_Enable_Register_Offset 0x24
#define Timer_Counter_Channel_Interrupt_Enable_Register_Register_C_Compare_Offset 4
#define Timer_Counter_Channel_Interrupt_Enable_Register_Register_C_Compare_Mask \
(1 << Timer_Counter_Channel_Interrupt_Enable_Register_Register_C_Compare_Offset)


/*
Interrupt Disable Register. Write-only
*/
#define Timer_Counter_Channel_IDR_Offset 0x28
#define Timer_Counter_Channel_Interrupt_Disable_Register_Offset 0x28

/*
Interrupt Mask Register. Read-only
*/
#define Timer_Counter_Channel_IMR_Offset 0x2C
#define Timer_Counter_Channel_Interrupt_Mask_Register_Offset 0x2C





/*
Block Control Register. Write-only.
*/
#define Timer_Counter_BCR_Offset 0xC0
#define Timer_Counter_Block_Control_Register_Offset 0xC0


/*
Block Mode Register. Read/Write.
*/
#define Timer_Counter_BMR_Offset 0xC4
#define Timer_Counter_Block_Mode_Register_Offset 0xC4


/*
Interrupt Controller Interface - INTC
*/
#define Interrupt_Controller_Address 0xFFFF0800
#define Interrupt_Controller_Priority_Register_Stride 4
#define Interrupt_Controller_Priority_Register(I) Register(Timer_Counter_Address + (Interrupt_Controller_Priority_Register_Stride * I))














#define Timer_Counter_Channel_Status_Register(Channel) \
Register(Timer_Counter_Address + (Timer_Counter_Channel_Stride * Channel) + Timer_Counter_Channel_Status_Register_Offset)

#define Timer_Counter_Channel_Interrupt_Enable_Register(Channel) \
Register(Timer_Counter_Address + (Timer_Counter_Channel_Stride * Channel) + Timer_Counter_Channel_Interrupt_Enable_Register_Offset)

#define Timer_Counter_Channel_Register_A(Channel) \
Register(Timer_Counter_Address + (Timer_Counter_Channel_Stride * Channel) + Timer_Counter_Channel_Register_A_Offset)

#define Timer_Counter_Channel_Register_C(Channel) \
Register(Timer_Counter_Address + (Timer_Counter_Channel_Stride * Channel) + Timer_Counter_Channel_Register_C_Offset)

#define Timer_Counter_Channel_Mode_Register(Channel) \
Register(Timer_Counter_Address + (Timer_Counter_Channel_Stride * Channel) + Timer_Counter_Channel_Mode_Register_Offset)

#define Timer_Counter_Channel_Control_Register(Channel) \
Register(Timer_Counter_Address + (Timer_Counter_Channel_Stride * Channel) + Timer_Counter_Channel_Control_Register_Offset)












#endif