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









inline void EVK1100_Port_1_Init ()
{
	GPIO_Enable (GPIO_Port_1) = EVK1100_Port_1_LED_Mask;
	GPIO_Output_Enable (GPIO_Port_1) = EVK1100_Port_1_LED_Mask;
	GPIO_Output_High (GPIO_Port_1) = EVK1100_Port_1_LED_Mask;
}













/*
Buttons definitions.
*/
#define EVK1100_Port_2_PB0 24
#define EVK1100_Port_2_PB1 21
#define EVK1100_Port_2_PB2 18
#define EVK1100_Port_2_PB0_Mask (1 << EVK1100_Port_2_PB0)
#define EVK1100_Port_2_PB1_Mask (1 << EVK1100_Port_2_PB1)
#define EVK1100_Port_2_PB2_Mask (1 << EVK1100_Port_2_PB2)
#define EVK1100_Port_2_PB_Mask (EVK1100_Port_2_PB0_Mask | EVK1100_Port_2_PB1_Mask | EVK1100_Port_2_PB2_Mask)
#define EVK1100_PB_Pressed (~GPIO_Pin_Value(2) & EVK1100_Port_2_PB_Mask)
#define EVK1100_Port_0_Joystick_Push  20
#define EVK1100_Port_0_Joystick_Left  25
#define EVK1100_Port_0_Joystick_Right 28
#define EVK1100_Port_0_Joystick_Up    26
#define EVK1100_Port_0_Joystick_Down  27
#define EVK1100_Port_0_Joystick_Push_Mask  (1 << EVK1100_Port_0_Joystick_Push)
#define EVK1100_Port_0_Joystick_Left_Mask  (1 << EVK1100_Port_0_Joystick_Left)
#define EVK1100_Port_0_Joystick_Right_Mask (1 << EVK1100_Port_0_Joystick_Right)
#define EVK1100_Port_0_Joystick_Up_Mask    (1 << EVK1100_Port_0_Joystick_Up)
#define EVK1100_Port_0_Joystick_Down_Mask  (1 << EVK1100_Port_0_Joystick_Down)
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