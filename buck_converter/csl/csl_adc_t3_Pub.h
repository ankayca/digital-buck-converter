/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_adc_t3_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Controls the ADC modules. Use the functions to configure an ADC module to
* sample and convert an input channel according to a trigger source and then
* read the result.
*
* ADC_init() must be called before any of the API functions are called.
*
* There are 16 start of conversion registers which are referred to within this
* library as ADC modules. Therefore there are 16 ADC modules which can be
* configured using ADC_config() to sample any combination of the analog input
* channels. There is only one ADC core which performs the conversions.
* Therefore only one module can be serviced at a time.
*
* A trigger will cause the start of conversion for each module. As there is only
* one ADC core, a round robin arbitration process determines which module will
* be serviced by the ADC core.
*
* As stated in <http://focus.ti.com/lit/ug/spruge5a/spruge5a.pdf> it takes 13
* ADC clock ticks to convert a sample. The minimum number of clock ticks
* required to sample a channel is 7. Therefore the minimum time for a
* conversion is 20 clock ticks. The ADC module clock ticks are derived
* directly from the system clock. Therefore if the system clock is 60MHz
* the minimum time for a complete conversion would be,
*
*   Tmin = 20 cycles x 16.6ns = 333ns
*
* When using a ePWM SOC you need to allow for the following timings
*
*   ADC SOC to sample (2xsys clk)       33.2ns
*   ADC sample & Hold (7xsys clk)      116.2ns
*              total time to 1st sample 150ns
*
* Once a sample has been taken, it takes 600ns to enter the ISR routine using
* late interrupt pulse.
* If you use Early interrupt pulse then this is reduce to 380ns.
*
* The ADC module, channels to sample, and how the conversion is triggered
* must be configured in order to use the ADC.
*
* EXAMPLES
* Configures the channels A0 to A4 to be sampled using ADC modules 1 to 5.
* The function isr_adc_hall is set as the interrupt service routine that is
* called when the last ADC module has completed its conversion. Global
* interrupts are enabled and the ePWM module 1 SOC A is used to generate the
* start of conversion pulse.
*
*   void main( void )
*   {
*      ADC_config( ADC_MOD_1, ADC_SH_WIDTH_7, ADC_CH_A0, ADC_TRIG_EPWM1_SOCA );
*      ADC_config( ADC_MOD_2, ADC_SH_WIDTH_7, ADC_CH_A1, ADC_TRIG_EPWM1_SOCA );
*      ADC_config( ADC_MOD_3, ADC_SH_WIDTH_7, ADC_CH_A2, ADC_TRIG_EPWM1_SOCA );
*      ADC_config( ADC_MOD_4, ADC_SH_WIDTH_7, ADC_CH_A3, ADC_TRIG_EPWM1_SOCA );
*      ADC_config( ADC_MOD_5, ADC_SH_WIDTH_7, ADC_CH_A4, ADC_TRIG_EPWM1_SOCA );
*
*      ADC_setCallback( ADC_MOD_5, isr_adc_hall, ADC_INT_1 );
*
*      PWM_setAdcSoc( PWM_MOD_1, PWM_CH_A, PWM_INT_ZERO );
*
*      INT_enableGlobal( true );
*   }
*
*   interrupt void isr_adc_hall( void )
*   {
*       // Acknowledge interrupt
*       ADC_ackInt( ADC_INT_1 );
*       r1 = ADC_getValue( ADC_MOD_1 );
*       r2 = ADC_getValue( ADC_MOD_1 );
*       r3 = ADC_getValue( ADC_MOD_1 );
*       r4 = ADC_getValue( ADC_MOD_1 );
*       r5 = ADC_getValue( ADC_MOD_1 );
*       ...
*   }
*
* NOTES
* On some devices ADC_CH_A5 and ADC_CH_B5 are not implemented in hardware. Check
* the device specific datasheet for further details.
*
* LINKS
* file:///C:/tidcs/c28/CSL_C2802x/v100/doc/CSL_C2802x.pdf
* http://www.ti.com/litv/pdf/spru716c
*
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_ADC_T3_PUB_H
#define _CSL_ADC_T3_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/


/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* COMPLEX       : ADC_TYPE_3
* DESCRIPTION   :
* This can be use to determine the peripheral type used by the csl.
*******************************************************************************/
#define ADC_TYPE_3

/*******************************************************************************
* ENUM          : ADC_Module
* DESCRIPTION   :
* This uses the same style as the rest of the CSL, referring to the ADC
* sequencer 1 & 2 as ADC_MOD_1/2.
* For the special case of the cascaded sequencer ADC_MOD_3 should be used.
*
* The naming convention should be ADC_MODULE, but we are trying to make it look
* like a normal pwm, i2c, etc module.
*******************************************************************************/
typedef enum ADC_Module
{
    ADC_MOD_1,          /* ADC SOC 0 */
    ADC_MOD_2,          /* ADC SOC 1 */
    ADC_MOD_3,          /* ADC SOC 2 */
    ADC_MOD_4,          /* ADC SOC 3 */
    ADC_MOD_5,          /* ADC SOC 4 */
    ADC_MOD_6,          /* ADC SOC 5 */
    ADC_MOD_7,          /* ADC SOC 6 */
    ADC_MOD_8,          /* ADC SOC 7 */
    ADC_MOD_9,          /* ADC SOC 8 */
    ADC_MOD_10,         /* ADC SOC 9 */
    ADC_MOD_11,         /* ADC SOC 10 */
    ADC_MOD_12,         /* ADC SOC 11 */
    ADC_MOD_13,         /* ADC SOC 12 */
    ADC_MOD_14,         /* ADC SOC 13 */
    ADC_MOD_15,         /* ADC SOC 14 */
    ADC_MOD_16          /* ADC SOC 15 */
} ADC_Module;

/*******************************************************************************
* ENUM          : ADC_Channel
* DESCRIPTION   :
* The analog ADC channels that can be sampled and converted to a digital value.
* Refer to the device datasheet for the equivalent device pins.
*******************************************************************************/
typedef enum ADC_Channel
{
    ADC_CH_A0   = 0,
    ADC_CH_A1   = 1,
    ADC_CH_A2   = 2,
    ADC_CH_A3   = 3,
    ADC_CH_A4   = 4,
    ADC_CH_A5   = 5,
    ADC_CH_A6   = 6,
    ADC_CH_A7   = 7,
    ADC_CH_B0   = 8,
    ADC_CH_B1   = 9,
    ADC_CH_B2   = 10,
    ADC_CH_B3   = 11,
    ADC_CH_B4   = 12,
    ADC_CH_B5   = 13,
    ADC_CH_B6   = 14,
    ADC_CH_B7   = 15
} ADC_Channel;

/*******************************************************************************
* ENUM          : ADC_SampleHoldWidth
* DESCRIPTION   :
* These are the values used to specify the sample and hold width.
*******************************************************************************/
typedef enum ADC_SampleHoldWidth
{
    ADC_SH_WIDTH_7      = 6,
    ADC_SH_WIDTH_8,
    ADC_SH_WIDTH_9,
    ADC_SH_WIDTH_10,
    ADC_SH_WIDTH_11,
    ADC_SH_WIDTH_12,
    ADC_SH_WIDTH_13,
    ADC_SH_WIDTH_14,
    ADC_SH_WIDTH_15,
    ADC_SH_WIDTH_16,
    ADC_SH_WIDTH_17,
    ADC_SH_WIDTH_18,
    ADC_SH_WIDTH_19,
    ADC_SH_WIDTH_20,
    ADC_SH_WIDTH_21,
    ADC_SH_WIDTH_22,
    ADC_SH_WIDTH_23,
    ADC_SH_WIDTH_24,
    ADC_SH_WIDTH_55     = 54,
    ADC_SH_WIDTH_56,
    ADC_SH_WIDTH_57,
    ADC_SH_WIDTH_58,
    ADC_SH_WIDTH_59,
    ADC_SH_WIDTH_60,
    ADC_SH_WIDTH_61,
    ADC_SH_WIDTH_62,
    ADC_SH_WIDTH_63,
    ADC_SH_WIDTH_64
} ADC_SampleHoldWidth;

/*******************************************************************************
* ENUM          : ADC_TriggerSelect
* DESCRIPTION   :
*******************************************************************************/
typedef enum ADC_TriggerSelect
{
    ADC_TRIG_NONE,                  /* Software trigger only */
    ADC_TRIG_TIMER_0,
    ADC_TRIG_TIMER_1,
    ADC_TRIG_TIMER_2,
    ADC_TRIG_XINT,
    ADC_TRIG_EPWM1_SOCA,
    ADC_TRIG_EPWM1_SOCB,
    ADC_TRIG_EPWM2_SOCA,
    ADC_TRIG_EPWM2_SOCB,
    ADC_TRIG_EPWM3_SOCA,
    ADC_TRIG_EPWM3_SOCB,
    ADC_TRIG_EPWM4_SOCA,
    ADC_TRIG_EPWM4_SOCB,
    ADC_TRIG_EPWM5_SOCA,
    ADC_TRIG_EPWM5_SOCB,
    ADC_TRIG_EPWM6_SOCA,
    ADC_TRIG_EPWM6_SOCB,
    ADC_TRIG_EPWM7_SOCA,
    ADC_TRIG_EPWM7_SOCB,
    ADC_TRIG_ADCINT1    = 0x81,     /* special trigger from another EOC using ADCINT1 */
    ADC_TRIG_ADCINT2    = 0x82      /* special trigger from another EOC using ADCINT2 */
} ADC_TriggerSelect;

/*******************************************************************************
* ENUM          : ADC_Interrupt
* DESCRIPTION   :
* Each ADC Module can generate 1 of 9 interrupts.
* When you configure the ADC module for a call back you need to specif which
* interrupt it will generate.
*
* ADCINT9H/1H/2H are high prioritys.
*
* NOTES
*
*******************************************************************************/
typedef enum ADC_Interrupt
{
    ADC_INT_1   = SYS_LIT( 0, INT_ID_ADCINT1),      /* Group 10 PIE */
    ADC_INT_2   = SYS_LIT( 1, INT_ID_ADCINT2),      /* Group 10 PIE */
    ADC_INT_3   = SYS_LIT( 2, INT_ID_ADCINT3),      /* Group 10 PIE */
    ADC_INT_4   = SYS_LIT( 3, INT_ID_ADCINT4),      /* Group 10 PIE */
    ADC_INT_5   = SYS_LIT( 4, INT_ID_ADCINT5),      /* Group 10 PIE */
    ADC_INT_6   = SYS_LIT( 5, INT_ID_ADCINT6),      /* Group 10 PIE */
    ADC_INT_7   = SYS_LIT( 6, INT_ID_ADCINT7),      /* Group 10 PIE */
    ADC_INT_8   = SYS_LIT( 7, INT_ID_ADCINT8),      /* Group 10 PIE */
    ADC_INT_9H  = SYS_LIT( 8, INT_ID_ADCINT9H),     /* Group 1 PIE */
    ADC_INT_1H  = SYS_LIT( 0, INT_ID_ADCINT1H),     /* Group 1 PIE */
    ADC_INT_2H  = SYS_LIT( 1, INT_ID_ADCINT2H)      /* Group 1 PIE */
} ADC_Interrupt;

/*******************************************************************************
* COMPLEX       : ADC_VrefMax
* DESCRIPTION   :
* This is the ADC internal max reference voltage.
*******************************************************************************/
#define ADC_VrefMax (3.3)

/*******************************************************************************
* COMPLEX       : ADC_ValueMax
* DESCRIPTION   :
* This is the ADC max value returned from the ADC.
*******************************************************************************/
#define ADC_ValueMax (4095)


/********** PROTOTYPES SECTIONS ***********************************************/

/*******************************************************************************
* MACRO         : ADC_getIqValueMult
* INPUT         : ADC_Module Mod
*                 Selects the ADC sequencer.
* INPUT         : _iq Mult
*                 IQ multiplier.
* RETURNS       : void
* DESCRIPTION   :
* Returns the ADC value as an _iq number after multiplying it with the _iq
* number passed as an argument to the function.
*
* The ADC value is read from the result registers at the index value of the ADC
* module specified in the function call.
*
* EXAMPLES
* Reads the value from index 2 and multiples it by 3.14.
*
*   iq_result = ADC_getIqValueMult(ADC_MOD_1, 2, _IQ(3.14) );
*
* If the result stored in the ADC mirror register ADCRESULT2 is 0x9 and an _iq
* value of _iq(3.14) is passed as an argument to this function, the _iq value
* returned will be 102891 assuming a GLOBAL_Q of 12 is defined.
*
* In this case the ADC result, 0x9, is divided by 4096 (left-shifted 12 places)
* and multiplied by the _iq version of 3.14.
*
* NOTES
* The GLOBAL_Q must be greater than or equal to the number of ADC bits, e.g. 12.
*
*******************************************************************************/
#define ADC_getIqValueMult( Mod, Mult ) _IQmpy( (uint32_t)ADC_getValue(Mod)<< (GLOBAL_Q-12), Mult )

/* public methods */
extern void ADC_init( void );
extern void ADC_setEarlyInterrupt( int Enable );
extern void ADC_setCallback( ADC_Module Mod, INT_IsrAddr Func,
                             ADC_Interrupt AdcInt );
extern uint16_t ADC_startConversion( ADC_Module Mod, ADC_Interrupt AdcInt );
#ifdef LIB_FUNC
extern void ADC_clrInt( ADC_Interrupt AdcInt );
extern uint16_t ADC_getValue( ADC_Module Mod );
#endif /* LIB_FUNC */
extern void ADC_setPriority( ADC_Module Mod );
#ifdef LIB_FUNC
extern void ADC_ackInt( ADC_Interrupt AdcInt );
#endif /* LIB_FUNC */
extern void ADC_socSoftware( ADC_Module Mod );
#ifdef LIB_FUNC
extern int ADC_isReady( ADC_Interrupt AdcInt );
#endif /* LIB_FUNC */
extern void ADC_config( ADC_Module Mod, ADC_SampleHoldWidth SH,
                        ADC_Channel Chan, ADC_TriggerSelect TrigSel );
#ifdef LIB_FUNC
extern int ADC_getIndex( ADC_Module Mod );
#endif /* LIB_FUNC */
extern void ADC_setExternalRefernce( int Enable );
#ifdef LIB_FUNC
extern INT_PieId ADC_getPieId( ADC_Interrupt AdcInt );
#endif /* LIB_FUNC */

/********** USER MIDDLE SECTION ***********************************************/
#ifndef LIB_FUNC
#define ADC_isReady( AdcInt )    MACRO_ADC_isSeqReady( AdcInt )
#define ADC_getPieId(   AdcInt )    MACRO_ADC_getPieId(AdcInt)
#define ADC_clrInt(     AdcInt )    MACRO_ADC_clrInt( AdcInt )
#define ADC_ackInt(     AdcInt )    MACRO_ADC_ackInt( AdcInt )
#define ADC_getValue( Mod )         MACRO_ADC_getValue( Mod )
#define ADC_getIndex( Mod )         MACRO_ADC_getIndex(Mod)
#endif /* LIB_FUNC */

#define MACRO_ADC_isSeqReady( AdcInt )  (AdcRegs.ADCINTFLG.all & (1<<SYS_LIT_VALUE(AdcInt)))
#define MACRO_ADC_getPieId(   AdcInt )  ((INT_PieId)SYS_LIT_REG(AdcInt))
#define MACRO_ADC_clrInt(     AdcInt )  AdcRegs.ADCINTFLGCLR.all = (1<<SYS_LIT_VALUE(AdcInt))
#define MACRO_ADC_ackInt(     AdcInt )  ADC_clrInt(AdcInt); INT_ackPieGroup(ADC_getPieId(AdcInt))
#define MACRO_ADC_getValue( Mod )       *((Mod) + &AdcResult.ADCRESULT0 )
#define MACRO_ADC_getIndex( Mod )       ((int)(Mod))
/********** END ***************************************************************/
#endif

