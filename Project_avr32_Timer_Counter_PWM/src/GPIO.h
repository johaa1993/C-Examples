#include "Base.h"

#ifndef GPIO_H_
#define GPIO_H_

#define GPIO_Address 0xFFFF1000

#define GPIO_Mode_Stride 0x04
#define GPIO_Mode_Read_Offset 0x00
#define GPIO_Mode_Read_Write_Offset 0x00
#define GPIO_Mode_Set_Offset 0x04
#define GPIO_Mode_Clear_Offset 0x08
#define GPIO_Mode_Toggle_Offset 0x0C
#define GPIO_Port_Stride 0x100
#define GPIO_Port_0_Offset 0x000
#define GPIO_Port_1_Offset 0x100
#define GPIO_Port_2_Offset 0x200
#define GPIO_Port_3_Offset 0x300
#define GPIO_Port_0 0
#define GPIO_Port_1 1
#define GPIO_Port_2 2
#define GPIO_Port_3 3
#define GPIO_Name_Stride 0x10
#define GPIO_Name_GPER_Offset 0x00
#define GPIO_Name_PMR0_Offset 0x10
#define GPIO_Name_PMR1_Offset 0x20
#define GPIO_Name_ODER_Offset 0x40
#define GPIO_Name_OVR_Offset 0x50
#define GPIO_Name_PVR_Offset 0x60


#define GPIO_GPIO_Enable_Register_Set(Port)           Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_GPER_Offset + GPIO_Mode_Set_Offset)
#define GPIO_GPIO_Enable_Register_Clear(Port)         Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_GPER_Offset + GPIO_Mode_Clear_Offset)
#define GPIO_Output_Drive_Enable_Register_Set(Port)   Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_ODER_Offset + GPIO_Mode_Set_Offset)
#define GPIO_Output_Drive_Enable_Register_Clear(Port) Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_ODER_Offset + GPIO_Mode_Clear_Offset)
#define GPIO_Output_Value_Register(Port)              Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_OVR_Offset + GPIO_Mode_Read_Write_Offset)
#define GPIO_Output_Value_Register_Set(Port)          Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_OVR_Offset + GPIO_Mode_Set_Offset)
#define GPIO_Output_Value_Register_Clear(Port)        Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_OVR_Offset + GPIO_Mode_Clear_Offset)
#define GPIO_Output_Value_Register_Toggle(Port)       Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_OVR_Offset + GPIO_Mode_Toggle_Offset)
#define GPIO_Pin_Value_Register(Port)                 Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_PVR_Offset + GPIO_Mode_Read_Offset)
#define GPIO_Peripheral_Mux_Register_0_Set(Port)      Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_PMR0_Offset + GPIO_Mode_Set_Offset)
#define GPIO_Peripheral_Mux_Register_1_Set(Port)      Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_PMR1_Offset + GPIO_Mode_Set_Offset)
#define GPIO_Peripheral_Mux_Register_0_Clear(Port)    Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_PMR0_Offset + GPIO_Mode_Clear_Offset)
#define GPIO_Peripheral_Mux_Register_1_Clear(Port)    Register (GPIO_Address + (GPIO_Port_Stride * Port) + GPIO_Name_PMR1_Offset + GPIO_Mode_Clear_Offset)



#define GPIO_Enable            GPIO_GPIO_Enable_Register_Set
#define GPIO_Disable           GPIO_GPIO_Enable_Register_Clear
#define GPIO_Output_Enable     GPIO_Output_Drive_Enable_Register_Set
#define GPIO_Output_Disable    GPIO_Output_Drive_Enable_Register_Clear
#define GPIO_Output            GPIO_Output_Value_Register
#define GPIO_Output_High       GPIO_Output_Value_Register_Set
#define GPIO_Output_Low        GPIO_Output_Value_Register_Clear
#define GPIO_Output_Toggle     GPIO_Output_Value_Register_Toggle
#define GPIO_Pin_Value         GPIO_Pin_Value_Register


#endif