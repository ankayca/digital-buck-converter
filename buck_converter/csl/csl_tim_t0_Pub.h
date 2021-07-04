/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_tim_t0_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Contains functions for configuring and manipulating the CPU timer modules.
*
* All three timer modules can be used since the CSL library does not use BIOS.
*
* All timing is measured in TIM clock ticks. The duration of a TIM clock tick is
* dependant on the system clock speed. The conversion functions provided can be
* used to convert from frequency and ns to TIM clock ticks.
*
* EXAMPLES
* Creates a timer with a system clock divider of 7. The timer counter register
* is loaded with a number of TIM clock ticks which will cause the timer to
* expire with a frequency of 1Hz using the function TIM_freqToTicks(). An
* interrupt service routine is configured to be executed when the timer module
* interrupt flag is raised.
*
*   TIM_config( TIM_MOD_1,
*               TIM_freqToTicks( 1.0, 7 ),
*               7 );
*   TIM_setCallback( TIM_MOD_1, isr_tim1 );
*
*   interrupt void isr_tim1(void)
*   {
*      // Acknowledge timer interrupt
*      TIM_ackInt(TIM_MOD);
*      // User code
*   }
*
*
* LINKS
* file:///C:/tidcs/c28/CSL_C280x/v100/doc/CSL_C280x.pdf
* http://focus.ti.com/lit/ug/spru712f/spru712f.pdf
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_TIM_T0_PUB_H
#define _CSL_TIM_T0_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/
extern volatile struct CPUTIMER_REGS CpuTimer1Regs;
extern volatile struct CPUTIMER_REGS CpuTimer2Regs;

/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* COMPLEX       : TIM_MOD_X
* DESCRIPTION   :
* These values are used to specify the TIM module.
*******************************************************************************/
#if 1
#define TIM_MOD_1 (&CpuTimer0Regs)
#define TIM_MOD_2 (&CpuTimer1Regs)
#define TIM_MOD_3 (&CpuTimer2Regs)
#endif

/*******************************************************************************
* COMPLEX       : TIM_Module
* DESCRIPTION   :
* This is used to map hardware register values to the TIM Module.
*******************************************************************************/
typedef volatile struct CPUTIMER_REGS* TIM_Module;


/********** PROTOTYPES SECTIONS ***********************************************/

/*******************************************************************************
* MACRO         : TIM_nsToTicks
* INPUT         : uint32_t Ns
* INPUT         : uint16_t Prescaler
* RETURNS       : uint32_t
* DESCRIPTION   :
* Returns the number of TIM ticks required for the time value, in nanoseconds,
* passed to the function.
*
* The duration of one TIM tick is calculated using the Prescaler value passed as
* an argument to the function.
*
* This function could be used when the TIM module is configured with
* TIM_config() to set the period count register and thus the length of time that
* the timer counts for.
*
* EXAMPLES
* Returns the number of TIM ticks required for 100ns with a prescaler of 2.
*
*   ui32_100nszInTicks = TIM_nsToTicks( 100, TIM_getPrescaler( TIM_MOD_1 ) );
*
* For a device with a 100MHz system clock each TIM tick would last for 20ns
* using the Prescaler value of 2. Therefore this function would return a value
* of 5 TIM ticks. This would be the value that the TIM module counter must
* count down from in order to generate an interrupt with a period of 100ns.
*
* Similarly, for a device with a 80MHz system clock each TIM tick would last
* for 25ns using the Prescaler value of 2. Therefore this function would return
* a value of 4 TIM ticks. Again this would be the value that the TIM module
* counter must count down from in order to generate an interrupt with a period
* of 100ns.
*
* NOTES
* The maxium value is 1,000,000 (1ms) before the integer operation result is out
* of range.
*******************************************************************************/
#define TIM_nsToTicks( Ns, Prescaler ) ((1000L*(Ns))/(SYS_CLK_PS*Prescaler))

/*******************************************************************************
* MACRO         : TIM_freqToTicks
* INPUT         : uint16_t Fs_Hz
* INPUT         : void Prescaler
* RETURNS       : uint32_t
* DESCRIPTION   :
* Returns the number of TIM ticks required to generate the frequency value
* (in Hertz) passed to the function.
*
* The duration of one TIM tick is calculated using the Prescaler value passed as
* an argument to the function.
*
* This function could be used when the TIM module is configured with
* TIM_config() to set the period count register and thus frequency of the timer.
*  EXAMPLES
* Returns the number of TIM ticks required for 100kHz with a prescaler of 2.
*
*   ui32_100kHzInTicks = TIM_freqToTicks( 100000, 2 );
*
* For a device with a 100MHz system clock each TIM tick would last for 20ns
* using the Prescaler value of 2. Therefore this function would return a value
* of 500 TIM ticks. This would be the value that the TIM module counter must
* count down from in order to generate an interrupt at a frequency of 100kHz.
*
* Similarly, for a device with a 80MHz system clock each TIM tick would last
* for 25ns using the Prescaler value of 2. Therefore this function would return
* a value of 400 TIM ticks. Again this would be the value that the TIM module
* counter must count down from in order to generate an interrupt at a frequency
* of 100kHz.
*
* NOTES
*******************************************************************************/
#define TIM_freqToTicks( Fs_Hz, Prescaler ) (SYS_CLK_HZ / ( Fs_Hz * Prescaler ))

/*******************************************************************************
* MACRO         : TIM_getIndex
* INPUT         : TIM_Module Mod
*                 Selects the TIM module.
* RETURNS       : void
* DESCRIPTION   :
* Returns the index value for each TIM module, e.g.
*
*   TIM_MOD_1 = 0
*   TIM_MOD_2 = 1
*        :
*   TIM_MOD_n = n-1
*
* EXAMPLES
* Returns 2 for TIM module 3.
*
*   TIM_getIndex( TIM_MOD_3 );
*******************************************************************************/
#define TIM_getIndex( Mod ) (Mod-TIM_MOD_1)

/*******************************************************************************
* MACRO         : TIM_ackInt
* INPUT         : TIM_Module Mod
*                 Selects the TIM module.
* RETURNS       : void
* DESCRIPTION   :
* Used within an interrupt service routine to clear both the TIM interrupt flag
* and the PIE group flag.
*
* EXAMPLES
* Clears the TIM interrupt flag and the PIE group flag after the TIM module 1
* generates an interrupt and the PIE controller calls this ISR.
*
*   interrupt void isr_tim1( void )
*   {
*      TIM_ackInt( TIM_MOD_1 );
*   }
*
*******************************************************************************/
#define TIM_ackInt( Mod ) TIM_clrInt(Mod); if(Mod==TIM_MOD_1){INT_ackPieGroup(INT_ID_TIM1);}

/* public methods */
#ifdef LIB_FUNC
extern void TIM_setPeriod( TIM_Module Mod, uint32_t Ticks );
extern void TIM_clrInt( TIM_Module Mod );
extern uint16_t TIM_isInt( TIM_Module Mod );
extern void TIM_reloadPeriod( TIM_Module Mod );
extern void TIM_stop( TIM_Module Mod, int Value );
#endif /* LIB_FUNC */
extern void TIM_setPrecaler( TIM_Module Mod, uint16_t Value );
extern void TIM_config( TIM_Module Mod, uint32_t Ticks, uint16_t Prescale );
extern void TIM_setCallback( TIM_Module Mod, INT_IsrAddr Func );
extern uint16_t TIM_getPrescaler( TIM_Module Mod );

/********** USER MIDDLE SECTION ***********************************************/
#ifndef LIB_FUNC
#define TIM_setPeriod( Mod, Ticks ) MACRO_TIM_setPeriod( Mod, Ticks )
#define TIM_clrInt( Mod )          MACRO_TIM_clrFlag( Mod )
#define TIM_isInt( Mod )           MACRO_TIM_isFlag( Mod )
#define TIM_reloadPeriod( Mod )     MACRO_TIM_reloadPeriod( Mod )
#define TIM_stop(Mod,Value)         MACRO_TIM_stop(Mod,Value)
#endif

#define MACRO_TIM_setPeriod( Mod, Ticks ) (Mod)->PRD.all = Ticks; (Mod)->TCR.bit.TRB = true
#define MACRO_TIM_clrFlag( Mod )          (Mod)->TCR.bit.TIF = true
#define MACRO_TIM_isFlag( Mod )           (Mod)->TCR.bit.TIF
#define MACRO_TIM_reloadPeriod( Mod )     (Mod)->TCR.bit.TRB = true
#define MACRO_TIM_stop( Mod, Value )      (Mod)->TCR.bit.TSS = (Value)

/********** END ***************************************************************/
#endif

