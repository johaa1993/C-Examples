/*
Oscillations definitions.
*/
#define EVK1100_Internal_Frequency 115200



/*
Ports definitions.
*/
#define EVK1100_Port_A AVR32_GPIO.port[0]
#define EVK1100_Port_B AVR32_GPIO.port[1]
#define EVK1100_Port_C AVR32_GPIO.port[2]
#define EVK1100_Port_X AVR32_GPIO.port[3]




/*
Glitch Filter Enable Register.
A glitch filter providing rejection of pulses shorter than one clock cycle.
When the glitch filter is enabled, a glitch with duration of less than 1
clock cycle is automatically rejected, while a pulse with duration of 2 clock
cycles or more is accepted. For pulse durations between 1 clock cycle and 2
clock cycles, the pulse may or may not be taken into account, depending on
the precise timing of its occurrence. Thus for a pulse to be guaranteed
visible it must exceed 2 clock cycles, whereas for a glitch to be reliably
filtered out, its duration must not exceed 1 clock cycle. The filter
introduces 2 clock cycles latency.
The glitch filters are controlled by the register GFER (Glitch Filter Enable Register). When a bit is
set in GFER, the glitch filter on the corresponding pin is enabled. The glitch filter affects only
interrupt inputs. Inputs to peripherals or the value read through PVR are not affected by the
glitch filters.
NOTE! The value of this register should only be changed when IER is ‘0’. Updating this GFER while interrupt on the corresponding
pin is enabled can cause an unintentional interrupt to be triggered.
0 = Glitch filter is disabled for the corresponding pin.
1 = Glitch filter is enabled for the corresponding pin.
*/
#define EVK1100_Port_A_Glitch_Filter_Enable_Register_Set   EVK1100_Port_A.gfers
#define EVK1100_Port_A_Glitch_Filter_Enable_Register_Clear EVK1100_Port_A.gferc

/*
Output Value Register.
The level driven on an I/O line can be determined by writing OVR (Output Value Register).
0 = The value to be driven on the I/O line is 0.
1 = The value to be driven on the I/O line is 1.
*/
#define EVK1100_Port_B_Output_Value_Register               EVK1100_Port_B.ovr
#define EVK1100_Port_B_Output_Value_Register_Toggle        EVK1100_Port_B.ovrt
#define EVK1100_Port_B_Output_Value_Register_Clear         EVK1100_Port_B.ovrc
#define EVK1100_Port_B_Output_Value_Register_Set           EVK1100_Port_B.ovrs

/*
Output Driver Enable Register.
When the I/O line is controlled by the GPIO, the value of ODER (Output Driver Enable Register)
determines if the pin is driven or not. When a bit in this register is at 1, the corresponding I/O line
is driven by the GPIO. When the bit is at 0, the GPIO does not drive the line.
*/
#define EVK1100_Port_A_Output_Driver_Enable_Register_Set   EVK1100_Port_A.oders
#define EVK1100_Port_A_Output_Driver_Enable_Register_Clear EVK1100_Port_A.oderc
#define EVK1100_Port_B_Output_Driver_Enable_Register_Set   EVK1100_Port_B.oders
#define EVK1100_Port_C_Output_Driver_Enable_Register_Set   EVK1100_Port_C.oders
#define EVK1100_Port_X_Output_Driver_Enable_Register_Set   EVK1100_Port_X.oders

/*
GPIO Enable Register.
0 = A peripheral function controls the corresponding pin.
1 = The GPIO controls the corresponding pin.
*/
#define EVK1100_Port_A_GPIO_Enable_Register_Set            EVK1100_Port_A.gpers
#define EVK1100_Port_B_GPIO_Enable_Register_Set            EVK1100_Port_B.gpers


/*
Pin Value Register.
The level on each I/O line can be read through PVR (Pin Value Register). This register indicates
the level of the I/O lines regardless of whether the lines are driven by the GPIO or by an external
component. Note that due to power saving measures, PVR register can only be read when
GPER is set for the corresponding pin or if interrupt is enabled for the pin.
*/
#define EVK1100_Port_A_Pin_Value_Register                  EVK1100_Port_A.pvr
#define EVK1100_Port_B_Pin_Value_Register                  EVK1100_Port_B.pvr
#define EVK1100_Port_C_Pin_Value_Register                  EVK1100_Port_C.pvr
#define EVK1100_Port_X_Pin_Value_Register                  EVK1100_Port_X.pvr


/*
Interrupt Enable Register.
The GPIO can be programmed to generate an interrupt when it detects an input change on an
I/O line. The module can be configured to signal an interrupt whenever a pin changes value or
only to trigger on rising edges or falling edges. Interrupt is enabled on a pin by setting the corresponding
bit in IER (Interrupt Enable Register). The interrupt mode is set by accessing IMR0
(Interrupt Mode Register 0) and IMR1 (Interrupt Mode Register 1). Interrupt can be enabled on a
pin, regardless of the configuration the I/O line, i.e. controlled by the GPIO or assigned to a
peripheral function.
In every port there are four interrupt lines connected to the interrupt controller. Every eight interrupts
in the port are ored together to form an interrupt line.
When an interrupt event is detected on an I/O line, and the corresponding bit in IER is set, the
GPIO interrupt request line is asserted. A number of interrupt signals are ORed-wired together
to generate a single interrupt signal to the interrupt controller.
IFR (Interrupt Flag Register) can by read by software to determine which pin(s) caused the interrupt.
The interrupt flag must be manually cleared by writing to IFR.
GPIO interrupts can only be triggered when the GPIO clock is enabled.
*/
#define EVK1100_Port_A_Interrupt_Enable_Register EVK1100_Port_A.ier

/*
Interrupt Enable Register.
Interrupts can be enabled by writing to the Interrupt Enable Register (IER) and disabled by writing
to Interrupt Disable Register (IDR). The Interrupt Mask Register (IMR) can be read to see
whether an interrupt is enabled or not. The current status of an interrupt source can be read
through the Interrupt Status Register (ISR).
The PDCA has three interrupt sources:
Reload Counter Zero - The Transfer Counter Reload Register is zero.
Transfer Finished - Both the Transfer Counter Register and Transfer Counter Reload Register are zero.
Transfer Error - An error has occurred in accessing memory.
TERR: Transfer Error
0 = No effect.
1 = Enable Transfer Error interrupt.
TRC: Transfer Complete
0 = No effect.
1 = Enable Transfer Complete interrupt.
RCZ: Reload Counter Zero
0 = No effect.
1 = Enable Reload Counter Zero interrupt.
*/
#define EVK1100_Port_A_Interrupt_Enable_Register_Set       EVK1100_Port_A.iers


/*
Interrupt Mode Register.
The interrupt mode is set by accessing IMR0
(Interrupt Mode Register 0) and IMR1 (Interrupt Mode Register 1). Interrupt can be enabled on a
pin, regardless of the configuration the I/O line, i.e. controlled by the GPIO or assigned to a
peripheral function.
{IMR1, IMR0}
00 Pin Change
01 Rising Edge
10 Falling Edge
11 Reserved
*/
#define EVK1100_Port_A_Interrupt_Mode_Register_0_Clear     EVK1100_Port_A.imr0c
#define EVK1100_Port_A_Interrupt_Mode_Register_1_Set       EVK1100_Port_A.imr1s






/*
Interrupt Enable Register.
0 = An interrupt condition has been detected on the corresponding pin.
1 = No interrupt condition has been detected on the corresponding pin.
The number of interrupt request lines is dependent on the number of I/O pins on the MCU. Refer to the product specific
data for details. Note also that a bit in the Interrupt Flag register is only valid if the corresponding bit in IER (Interrupt Enable Register) is set.
*/
#define EVK1100_Port_A_Interrupt_Flag_Register_Clear EVK1100_Port_A.ifrc





#define EVK1100_Time_Channel AVR32_TC.channel
#define EVK1100_Time_Channel_0 EVK1100_Time_Channel[0]
#define EVK1100_Time_Channel_1 EVK1100_Time_Channel[1]
#define EVK1100_Time_Channel_2 EVK1100_Time_Channel[2]

/*
Clock Mode Register.
*/
#define EVK1100_Time_Channel_0_Clock_Mode_Register EVK1100_Time_Channel[0].cmr
#define EVK1100_Time_Channel_1_Clock_Mode_Register EVK1100_Time_Channel[1].cmr


/*
Waveform Selection. 2 bits
WAVSEL Effect
0 0 UP mode without automatic trigger on RC Compare
1 0 UP mode with automatic trigger on RC Compare
0 1 UPDOWN mode without automatic trigger on RC Compare
1 1 UPDOWN mode with automatic trigger on RC Compare
*/
#define Time_Channel_Waveform_Selection_Offset AVR32_TC_WAVSEL_OFFSET
#define Time_Channel_Waveform_Selection_Up_Mode_With_Automatic_Trigger_On_RC_Compare AVR32_TC_WAVSEL_UP_AUTO
#define Time_Channel_Waveform_Selection_Up_Mode_With_Automatic_Trigger_On_RC_Compare_Mask (Time_Channel_Waveform_Selection_Up_Mode_With_Automatic_Trigger_On_RC_Compare << Time_Channel_Waveform_Selection_Offset)


/*
ENETRG: External Event Trigger Enable
0 = The external event has no effect on the counter and its clock. In this case, the selected external event only controls the TIOA output.
1 = The external event resets the counter and starts the counter clock.
*/
#define Time_Channel_External_Event_Trigger_Offset AVR32_TC_ENETRG_OFFSET
#define Time_Channel_External_Event_Trigger_Disable 0
#define Time_Channel_External_Event_Trigger_Enable 1
#define Time_Channel_External_Event_Trigger_Enable_Mask (1 << Time_Channel_External_Event_Trigger_Offset)

/*
EEVTEDG: External Event Edge Selection
The EEVTEDG parameter defines the trigger edge for each of the possible external triggers (rising, falling or both).
If EEVTEDG is cleared (none), no external event is defined.
0 0 none
0 1 rising edge
1 0 falling edge
1 1 each edge
*/
#define Time_Channel_External_Event_Edge_Selection_Offset AVR32_TC_EEVTEDG_OFFSET
#define Time_Channel_External_Event_Edge_Selection_None AVR32_TC_EEVTEDG_NO_EDGE
#define Time_Channel_External_Event_Edge_Selection_Rising AVR32_TC_EEVTEDG_POS_EDGE
#define Time_Channel_External_Event_Edge_Selection_Falling AVR32_TC_EEVTEDG_NEG_EDGE
#define Time_Channel_External_Event_Edge_Selection_Both AVR32_TC_EEVTEDG_BOTH_EDGES



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


int tc_init_waveform(volatile avr32_tc_t *tc, const tc_waveform_opt_t *opt)
{
	// Check for valid input.
	if (opt->channel >= TC_NUMBER_OF_CHANNELS)
	return TC_INVALID_ARGUMENT;

	// GENERATE SIGNALS: Waveform operating mode.
	tc->channel[opt->channel].cmr = opt->bswtrg << AVR32_TC_BSWTRG_OFFSET |
	opt->beevt << AVR32_TC_BEEVT_OFFSET |
	opt->bcpc << AVR32_TC_BCPC_OFFSET |
	opt->bcpb << AVR32_TC_BCPB_OFFSET |
	opt->aswtrg << AVR32_TC_ASWTRG_OFFSET |
	opt->aeevt << AVR32_TC_AEEVT_OFFSET |
	opt->acpc << AVR32_TC_ACPC_OFFSET |
	opt->acpa << AVR32_TC_ACPA_OFFSET |
	1 << AVR32_TC_WAVE_OFFSET |
	opt->wavsel << AVR32_TC_WAVSEL_OFFSET |
	opt->enetrg << AVR32_TC_ENETRG_OFFSET |
	opt->eevt << AVR32_TC_EEVT_OFFSET |
	opt->eevtedg << AVR32_TC_EEVTEDG_OFFSET |
	opt->cpcdis << AVR32_TC_CPCDIS_OFFSET |
	opt->cpcstop << AVR32_TC_CPCSTOP_OFFSET |
	opt->burst << AVR32_TC_BURST_OFFSET |
	opt->clki << AVR32_TC_CLKI_OFFSET |
	opt->tcclks << AVR32_TC_TCCLKS_OFFSET;

	return 0;
}









/*
LEDs definitions.
*/
#define EVK1100_Port_B_LED_1 27
#define EVK1100_Port_B_LED_2 28
#define EVK1100_Port_B_LED_3 29
#define EVK1100_Port_B_LED_4 30
#define EVK1100_Port_B_LED_5 19
#define EVK1100_Port_B_LED_6 20
#define EVK1100_Port_B_LED_7 21
#define EVK1100_Port_B_LED_8 22

#define EVK1100_Port_B_LED_1_Mask (1 << EVK1100_Port_B_LED_1)
#define EVK1100_Port_B_LED_2_Mask (1 << EVK1100_Port_B_LED_2)
#define EVK1100_Port_B_LED_3_Mask (1 << EVK1100_Port_B_LED_3)
#define EVK1100_Port_B_LED_4_Mask (1 << EVK1100_Port_B_LED_4)
#define EVK1100_Port_B_LED_5_Mask (1 << EVK1100_Port_B_LED_5)
#define EVK1100_Port_B_LED_6_Mask (1 << EVK1100_Port_B_LED_6)
#define EVK1100_Port_B_LED_7_Mask (1 << EVK1100_Port_B_LED_7)
#define EVK1100_Port_B_LED_8_Mask (1 << EVK1100_Port_B_LED_8)

#define EVK1100_Port_B_LED_1_2_3_4_Mask (EVK1100_Port_B_LED_1_Mask | EVK1100_Port_B_LED_2_Mask | EVK1100_Port_B_LED_3_Mask | EVK1100_Port_B_LED_4_Mask)





/*
Buttons definitions.
*/
#define EVK1100_Port_X_PB0 16
#define EVK1100_Port_X_PB1 19
#define EVK1100_Port_X_PB2 22

#define EVK1100_Port_X_PB0_Mask (1 << EVK1100_Port_X_PB0)
#define EVK1100_Port_X_PB1_Mask (1 << EVK1100_Port_X_PB1)
#define EVK1100_Port_X_PB2_Mask (1 << EVK1100_Port_X_PB2)

#define EVK1100_Port_A_Joystick_Push 20
#define EVK1100_Port_A_Joystick_Left 25

#define EVK1100_Port_A_Joystick_Push_Mask (1 << EVK1100_Port_A_Joystick_Push)
#define EVK1100_Port_A_Joystick_Left_Mask (1 << EVK1100_Port_A_Joystick_Left)