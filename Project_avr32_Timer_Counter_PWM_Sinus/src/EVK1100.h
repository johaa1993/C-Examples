#include "Base.h"
#include "GPIO.h"
#include "Timer_Counter.h"


#ifndef EVK1100_H_
#define EVK1100_H_


/*
LEDs definitions.
*/
#define EVK1100_Port_1_LED_1_Offset       27
#define EVK1100_Port_1_LED_2_Offset       28
#define EVK1100_Port_1_LED_3_Offset       29
#define EVK1100_Port_1_LED_4_Offset       30
#define EVK1100_Port_1_LED_5_Red_Offset   19
#define EVK1100_Port_1_LED_5_Green_Offset 20
#define EVK1100_Port_1_LED_6_Red_Offset   21
#define EVK1100_Port_1_LED_6_Green_Offset 22
#define EVK1100_Port_1_LED_1_Mask       (1 << EVK1100_Port_1_LED_1_Offset)
#define EVK1100_Port_1_LED_2_Mask       (1 << EVK1100_Port_1_LED_2_Offset)
#define EVK1100_Port_1_LED_3_Mask       (1 << EVK1100_Port_1_LED_3_Offset)
#define EVK1100_Port_1_LED_4_Mask       (1 << EVK1100_Port_1_LED_4_Offset)
#define EVK1100_Port_1_LED_5_Green_Mask (1 << EVK1100_Port_1_LED_5_Green_Offset)
#define EVK1100_Port_1_LED_5_Red_Mask   (1 << EVK1100_Port_1_LED_5_Red_Offset)
#define EVK1100_Port_1_LED_6_Green_Mask (1 << EVK1100_Port_1_LED_6_Green_Offset)
#define EVK1100_Port_1_LED_6_Red_Mask   (1 << EVK1100_Port_1_LED_6_Red_Offset)
#define EVK1100_Port_1_LED_1_2_3_4_Mask (EVK1100_Port_1_LED_1_Mask       | EVK1100_Port_1_LED_2_Mask | EVK1100_Port_1_LED_3_Mask | EVK1100_Port_1_LED_4_Mask)
#define EVK1100_Port_1_LED_5_Mask       (EVK1100_Port_1_LED_5_Green_Mask | EVK1100_Port_1_LED_5_Red_Mask)
#define EVK1100_Port_1_LED_6_Mask       (EVK1100_Port_1_LED_6_Green_Mask | EVK1100_Port_1_LED_6_Red_Mask)
#define EVK1100_Port_1_LED_5_6_Mask     (EVK1100_Port_1_LED_5_Mask       | EVK1100_Port_1_LED_6_Mask)
#define EVK1100_Port_1_LED_Mask         (EVK1100_Port_1_LED_1_2_3_4_Mask | EVK1100_Port_1_LED_5_6_Mask)
#define EVK1100_LED_1 EVK1100_Port_1_LED_1_Mask
#define EVK1100_LED_2 EVK1100_Port_1_LED_2_Mask
#define EVK1100_LED_3 EVK1100_Port_1_LED_3_Mask
#define EVK1100_LED_4 EVK1100_Port_1_LED_4_Mask
#define EVK1100_LED_5_Red EVK1100_Port_1_LED_5_Red_Mask
#define EVK1100_LED_5_Green EVK1100_Port_1_LED_5_Green_Mask
#define EVK1100_LED_6_Red EVK1100_Port_1_LED_6_Red_Mask
#define EVK1100_LED_6_Green EVK1100_Port_1_LED_6_Green_Mask
#define EVK1100_LED_On(Value) GPIO_Output_Low (1) = Value & EVK1100_Port_1_LED_Mask;
#define EVK1100_LED_Off(Value) GPIO_Output_High (1) = Value & EVK1100_Port_1_LED_Mask;
#define EVK1100_LED_Toggle(Value) GPIO_Output_Toggle (1) = Value & EVK1100_Port_1_LED_Mask;


#define EVK1100_Timer_Counter_Channel_LED_1 Timer_Counter_Channel_PB27







inline void EVK1100_Port_1_Init ()
{
	GPIO_Enable (GPIO_Port_1) = EVK1100_Port_1_LED_Mask;
	GPIO_Output_Enable (GPIO_Port_1) = EVK1100_Port_1_LED_Mask;
	GPIO_Output_High (GPIO_Port_1) = EVK1100_Port_1_LED_Mask;
}













/*
Buttons definitions.
*/
#define EVK1100_Port_2_PB0_Offset 24
#define EVK1100_Port_2_PB1_Offset 21
#define EVK1100_Port_2_PB2_Offset 18
#define EVK1100_Port_2_PB0_Mask (1 << EVK1100_Port_2_PB0_Offset)
#define EVK1100_Port_2_PB1_Mask (1 << EVK1100_Port_2_PB1_Offset)
#define EVK1100_Port_2_PB2_Mask (1 << EVK1100_Port_2_PB2_Offset)
#define EVK1100_Port_2_PB_Mask (EVK1100_Port_2_PB0_Mask | EVK1100_Port_2_PB1_Mask | EVK1100_Port_2_PB2_Mask)

#define EVK1100_PB_Pressed (~GPIO_Pin_Value(2) & EVK1100_Port_2_PB_Mask)
#define EVK1100_PB_0 EVK1100_Port_2_PB0_Mask
#define EVK1100_PB_1 EVK1100_Port_2_PB1_Mask
#define EVK1100_PB_2 EVK1100_Port_2_PB2_Mask

#define EVK1100_Port_0_Joystick_Push_Offset  20
#define EVK1100_Port_0_Joystick_Left_Offset  25
#define EVK1100_Port_0_Joystick_Right_Offset 28
#define EVK1100_Port_0_Joystick_Up_Offset    26
#define EVK1100_Port_0_Joystick_Down_Offset  27
#define EVK1100_Port_0_Joystick_Push_Mask  (1 << EVK1100_Port_0_Joystick_Push_Offset)
#define EVK1100_Port_0_Joystick_Left_Mask  (1 << EVK1100_Port_0_Joystick_Left_Offset)
#define EVK1100_Port_0_Joystick_Right_Mask (1 << EVK1100_Port_0_Joystick_Right_Offset)
#define EVK1100_Port_0_Joystick_Up_Mask    (1 << EVK1100_Port_0_Joystick_Up_Offset)
#define EVK1100_Port_0_Joystick_Down_Mask  (1 << EVK1100_Port_0_Joystick_Down_Offset)
#define EVK1100_Port_0_Joystick_Mask \
(\
EVK1100_Port_0_Joystick_Push_Mask|\
EVK1100_Port_0_Joystick_Left_Mask|\
EVK1100_Port_0_Joystick_Right_Mask|\
EVK1100_Port_0_Joystick_Up_Mask|\
EVK1100_Port_0_Joystick_Down_Mask\
)
#define EVK1100_Joystick_Pressed (~GPIO_Pin_Value(0) & EVK1100_Port_0_Joystick_Mask)












#endif /* EVK1100_H_ */