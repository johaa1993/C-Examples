//EVK1100 Config

//TODO
#define EVK1100_GPIO_Pin_Value(Port) (Port)

//EVK1100 Specifications
#define EVK1100_PB_Port 2
#define EVK1100_PB_0_Port EVK1100_PB_Port
#define EVK1100_PB_1_Port EVK1100_PB_Port
#define EVK1100_PB_2_Port EVK1100_PB_Port
#define EVK1100_PB_0_Offset 24
#define EVK1100_PB_1_Offset 21
#define EVK1100_PB_2_Offset 18
#define EVK1100_PB_0_Mask (1 << EVK1100_PB_0_Offset)
#define EVK1100_PB_1_Mask (1 << EVK1100_PB_1_Offset)
#define EVK1100_PB_2_Mask (1 << EVK1100_PB_2_Offset)
#define EVK1100_PB_Mask (EVK1100_PB_0_Mask | EVK1100_PB_1_Mask | EVK1100_PB_2_Mask)
#define EVK1100_PB_Pressed (~EVK1100_GPIO_Pin_Value (EVK1100_PB_Port) & EVK1100_PB_Mask)


// ROS Config
#define ROS_Start_Trigger (EVK1100_PB_Pressed & EVK1100_PB_0_Mask)



int main (void)
{
	while (ROS_Start_Trigger == 0)
	{};
	//Start ROS
}
