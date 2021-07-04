/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_pwm_t1_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Contains functions for configuring the ePWM modules.
*
* PWM_configClocks() or PWM_config() must be called before using any of the ePWM
* API functions. The ePWM hardware contains 6 ePWM modules which are accessed
* using the following pointers,
*
*   PWM_MOD_1
*   PWM_MOD_2
*   PWM_MOD_3
*   PWM_MOD_4
*
* Each ePWM module has two duty counters which are referred to as channels.
*
*   PWM_CH_A
*   PWM_CH_B
*
* To control an ePWM duty the ePWM module must be specified along with the
* channel to be set.
*
* All timing is measured in ePWM ticks. The duration of an ePWM tick is
* dependent on the system clock speed. The conversion functions provided can be
* used to convert from frequency and ns to ePWM ticks.
*
* This module is also capable of performing high resolution PWM using the micro
* edge positioner (MEP) logic on channel A. See PWM_setDutyHiRes() for more
* information.
*
* EXAMPLES
* This example configures ePWM module 1 with a 100kHz frequency and channel A
* of that module with a 25% duty cycle.
*
*   PWM_config( PWM_MOD_1,
*                     PWM_freqToTicks(100000),
*                     PWM_COUNT_UP );
*   PWM_pin( PWM_MOD_1, PWM_CH_A, GPIO_INVERT );
*   PWM_setDutyA( PWM_MOD_1,
*                 PWM_freqToTicks(100000)/4 );
*
* LINKS
* file:///C:/tidcs/c28/CSL_C280x/v100/doc/CSL_C280x.pdf
* http://focus.ti.com/lit/ug/spru791e/spru791e.pdf
*
* HISTORY       :
* 2009-11-11 hossa1c tunred on hi-res & mod 1 clocks within PWM_calibrateMep().
*******************************************************************************/


#ifndef _CSL_PWM_T1_PUB_H
#define _CSL_PWM_T1_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/
typedef struct TL_PWM_MOD_SIZE
{
    uint16_t A[0x40];
}TL_PWM_MOD_SIZE;

/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* COMPLEX       : PWM_TYPE_1
* DESCRIPTION   :
* This can be use to determine the peripheral type used by the csl.
*******************************************************************************/
#define PWM_TYPE_1

/*******************************************************************************
* COMPLEX       : PWM_MODULE_X
* DESCRIPTION   :
* These values are used to specify the ePWM module.
*******************************************************************************/
#if 1
#define PWM_MOD_1 (&EPwm1Regs)
#define PWM_MOD_2 (&EPwm2Regs)
#define PWM_MOD_3 (&EPwm3Regs)
#define PWM_MOD_4 (&EPwm4Regs)
#define PWM_MOD_5 (&EPwm5Regs)
#define PWM_MOD_6 (&EPwm6Regs)
#define PWM_MOD_7 (&EPwm7Regs)
#endif

/*******************************************************************************
* COMPLEX       : PWM_Module
* DESCRIPTION   :
* This is used to map hardware register values to PWM_Module.
*******************************************************************************/
typedef volatile struct EPWM_REGS* PWM_Module;

/*******************************************************************************
* ENUM          : PWM_ModuleChannel
* DESCRIPTION   :
* This is used to select channel A or B for each ePWM module.
*******************************************************************************/
typedef enum PWM_ModuleChannel
{
    PWM_CH_A,
    PWM_CH_B
} PWM_ModuleChannel;

/*******************************************************************************
* ENUM          : PWM_CountMode
* DESCRIPTION   :
* This is used to control the direction of the duty count.
*******************************************************************************/
typedef enum PWM_CountMode
{
    PWM_COUNT_UP,           /* counts up to period */
    PWM_COUNT_DOWN,         /* counts down from period */
    PWM_COUNT_UP_DOWN       /* counts up to period and then down */
} PWM_CountMode;

/*******************************************************************************
* ENUM          : PWM_TripZone
* DESCRIPTION   :
* These are used for setting the trip zone values.
*******************************************************************************/
typedef enum PWM_TripZone
{
    PWM_TZ1     = SYS_LIT(0, (1<<0)),
    PWM_TZ2     = SYS_LIT(1, (1<<1)),
    PWM_TZ3     = SYS_LIT(2, (1<<2)),
    PWM_TZ4     = SYS_LIT(3, (1<<3)),
    PWM_TZ5     = SYS_LIT(4, (1<<4)),
    PWM_TZ6     = SYS_LIT(5, (1<<5)),
    PWM_DCEVT   = SYS_LIT(6, (1<<6))    /* output from the digital compare unit */
} PWM_TripZone;

/*******************************************************************************
* ENUM          : PWM_IntMode
* DESCRIPTION   :
* This is used to indicate when the ePWM interrupt occurs.
*
*
* Only the following interrupt modes are valid when operating in PWM_COUNT_UP
* mode.
*   PWM_INT_ZERO
*   PWM_INT_PERIOD
*   PWM_INT_CMPA_UP
*   PWM_INT_CMPB_UP
*
* Only the following interrupt modes are valid when operating in PWM_COUNT_DOWN
* mode.
*   PWM_INT_ZERO
*   PWM_INT_PERIOD
*   PWM_INT_CMPA_DOWN
*   PWM_INT_CMPB_DOWN
*
* Any of the interrupt modes are valid when operating in PWM_COUNT_UP_DOWN mode.
*
*******************************************************************************/
typedef enum PWM_IntMode
{
    PWM_INT_ZERO        = 1,    /* interrupt when counter is equal to zero */
    PWM_INT_PERIOD      = 2,    /* interrupt when counter is equal to period register */
    PWM_INT_CMPA_UP     = 4,    /* interrupt when counter is equal to counter compare register A on up-count */
    PWM_INT_CMPA_DOWN   = 5,    /* interrupt when counter is equal to counter compare register A on down-count */
    PWM_INT_CMPB_UP     = 6,    /* interrupt when counter is equal to counter compare register B on up-count */
    PWM_INT_CMPB_DOWN   = 7     /* interrupt when counter is equal to counter compare register B on down-count */
} PWM_IntMode;

/*******************************************************************************
* ENUM          : PWM_TpzMode
* DESCRIPTION   :
* This defines the different trip zone modes.
*******************************************************************************/
typedef enum PWM_TpzMode
{
    PWM_TPZ_CYCLE_BY_CYCLE  = (1<<1),   /* limits output for one cycle */
    PWM_TPZ_ONE_SHOT        = (1<<2)    /* limits output forever */
} PWM_TpzMode;

/*******************************************************************************
* ENUM          : PWM_SyncOutSelect
* DESCRIPTION   :
* This is used to allow one ePWM module to sync to another.
*******************************************************************************/
typedef enum PWM_SyncOutSelect
{
    PWM_SYNCOSEL_IN         = 0,    /* connect sync in to sync out */
    PWM_SYNCOSEL_ZERO       = 1,    /* connect period==zero to sync out */
    PWM_SYNCOSEL_CMPB       = 2,    /* connect period=dutyB to sync out */
    PWM_SYNCOSEL_DISBALE    = 3     /* disable sync out */
} PWM_SyncOutSelect;

/*******************************************************************************
* ENUM          : PWM_HspClkDiv
* DESCRIPTION   :
* This is used to set the high speed ePWM divider.
*******************************************************************************/
typedef enum PWM_HspClkDiv
{
    PWM_HSP_DIV_1   = SYS_LIT( 1,   0 ),
    PWM_HSP_DIV_2   = SYS_LIT( 2,   1 ),
    PWM_HSP_DIV_4   = SYS_LIT( 4,   2 ),
    PWM_HSP_DIV_6   = SYS_LIT( 6,   3 ),
    PWM_HSP_DIV_8   = SYS_LIT( 8,   4 ),
    PWM_HSP_DIV_10  = SYS_LIT( 10,  5 ),
    PWM_HSP_DIV_12  = SYS_LIT( 12,  6 ),
    PWM_HSP_DIV_14  = SYS_LIT( 14,  7 )
} PWM_HspClkDiv;

/*******************************************************************************
* ENUM          : PWM_ClkDiv
* DESCRIPTION   :
* This is used to set the ePWM divider.
*******************************************************************************/
typedef enum PWM_ClkDiv
{
    PWM_DIV_1       = SYS_LIT( 1,   0 ),
    PWM_DIV_2       = SYS_LIT( 2,   1 ),
    PWM_DIV_4       = SYS_LIT( 4,   2 ),
    PWM_DIV_8       = SYS_LIT( 8,   3 ),
    PWM_DIV_16      = SYS_LIT( 16,  4 ),
    PWM_DIV_32      = SYS_LIT( 32,  5 ),
    PWM_DIV_64      = SYS_LIT( 64,  6 ),
    PWM_DIV_128     = SYS_LIT( 128, 7 )
} PWM_ClkDiv;

/*******************************************************************************
* ENUM          : PWM_IntPrd
* DESCRIPTION   :
* This is used to set when the ePWM interrupt will occur.
*******************************************************************************/
typedef enum PWM_IntPrd
{
    PWM_INT_PRD_1   = SYS_LIT( 1, 1 ),  /* cause interrupt on every occurance */
    PWM_INT_PRD_2   = SYS_LIT( 2, 2 ),  /* cause interrupt on every 2nd occurance */
    PWM_INT_PRD_3   = SYS_LIT( 3, 3 )   /* cause interrupt on every 3rd occurance */
} PWM_IntPrd;

/*******************************************************************************
* ENUM          : PWM_Half_Bridge
* DESCRIPTION   :
*******************************************************************************/
typedef enum PWM_Half_Bridge
{
    HALF_BRIDGE_NN,
    HALF_BRIDGE_PP,
    HALF_BRIDGE_PN
} PWM_Half_Bridge;

/*******************************************************************************
* ENUM          : PWM_CmpSelect
* DESCRIPTION   :
*******************************************************************************/
typedef enum PWM_CmpSelect
{
    PWM_CMP_TPZ1    = 0,
    PWM_CMP_TPZ2    = 1,
    PWM_CMP_TPZ3    = 2,
    PWM_CMP_COMP1   = 8,
    PWM_CMP_COMP2   = 9,
    PWM_CMP_COMP3   = 10    /* piccolo B only */
} PWM_CmpSelect;


/********** PROTOTYPES SECTIONS ***********************************************/

/*******************************************************************************
* MACRO         : PWM_getGpioPinA
* INPUT         : PWM_Module Mod
* RETURNS       : void
* DESCRIPTION   :
* Returns the GPIO pin associated with the ePWM module channel A.
*
*   GPIO_2 = PWM_getGpioPinA( PWM_MOD_2 )
*
*******************************************************************************/
#define PWM_getGpioPinA( Mod ) ((GPIO_Pin)(PWM_getIndex(Mod)*2+GPIO_0))

/*******************************************************************************
* MACRO         : PWM_getGpioPinB
* INPUT         : PWM_Module Mod
* RETURNS       : void
* DESCRIPTION   :
* Returns the GPIO pin associated with the ePWM module channel B.
*
*   GPIO_3 = PWM_getGpioPinA( PWM_MOD_2 )
*
*******************************************************************************/
#define PWM_getGpioPinB( Mod ) ((GPIO_Pin)(PWM_getIndex(Mod)*2+GPIO_1))

/* public methods */
#ifdef LIB_FUNC
extern int PWM_getIndex( PWM_Module Mod );
extern INT_PieId PWM_getPieId( PWM_Module Mod );
extern void PWM_ackInt( PWM_Module Mod );
extern uint16_t PWM_freqToTicks( uint32_t freq );
extern uint16_t PWM_freqToTicksClocks( uint32_t freq, PWM_HspClkDiv HspClkDiv,
                                       PWM_ClkDiv ClkDiv );
#endif /* LIB_FUNC */
extern void PWM_configClocks( PWM_Module Module, uint16_t Ticks,
                              PWM_HspClkDiv HspClkDiv, PWM_ClkDiv ClkDiv,
                              PWM_CountMode CountMode );
extern void PWM_config( PWM_Module Module, uint16_t Ticks,
                        PWM_CountMode CountMode );
extern void PWM_pin( PWM_Module Module, PWM_ModuleChannel Channel,
                     GPIO_Level Invert );
extern void PWM_setDuty( PWM_Module Module, PWM_ModuleChannel Channel,
                         uint16_t Ticks );
extern void PWM_setTripZone( PWM_Module Module, uint16_t Mask, PWM_TpzMode Mode );
extern void PWM_setCallback( PWM_Module Module, INT_IsrAddr Func,
                             PWM_IntMode Mode, PWM_IntPrd Prd );
extern void PWM_setDeadBand( PWM_Module Module, uint16_t Ticks,
                             GPIO_Level InvertA, GPIO_Level InvertB );
extern void PWM_setAdcSoc( PWM_Module Module, PWM_ModuleChannel Ch,
                           PWM_IntMode Mode );
#ifdef LIB_FUNC
extern uint16_t PWM_nsToTicks( uint32_t Ns );
extern uint16_t PWM_nsToTicksClocks( uint32_t Ns, PWM_HspClkDiv HspClkDiv,
                                     PWM_ClkDiv ClkDiv );
extern int PWM_isInt( PWM_Module Module );
extern void PWM_clrInt( PWM_Module Module );
extern uint16_t PWM_getPeriod( PWM_Module Module );
extern void PWM_setPeriod( PWM_Module Module, uint16_t Ticks );
extern void PWM_setDutyA( PWM_Module Module, uint16_t Ticks );
extern void PWM_setDutyHiRes( PWM_Module Module, uint32_t Ticks );
extern void PWM_setDutyB( PWM_Module Module, uint16_t Ticks );
extern void PWM_softwareSync( PWM_Module Module );
extern void PWM_setPhase( PWM_Module Module, uint16_t Phase );
#endif /* LIB_FUNC */
extern void PWM_setSyncOutSelect( PWM_Module Module, PWM_SyncOutSelect Mode );
extern uint16_t PWM_getDuty( PWM_Module Mod, PWM_ModuleChannel Channel );
extern void PWM_enableTpzInt( PWM_Module Mod, PWM_TpzMode Mode, int Enable );
#ifdef LIB_FUNC
extern void PWM_clrTpzInt( PWM_Module Mod, PWM_TpzMode Mode );
extern void PWM_ackTpzInt( PWM_Module Mod, PWM_TpzMode Mode );
extern INT_PieId PWM_getTzPieId( PWM_TripZone Tz );
#endif /* LIB_FUNC */
extern void PWM_setTripState( PWM_Module Module, PWM_ModuleChannel Channel,
                              GPIO_TriState TripState );
#ifdef LIB_FUNC
extern PWM_Module PWM_getMod( int Index );
#endif /* LIB_FUNC */
extern void PWM_setDeadBandHalfBridge( PWM_Module Module, uint16_t Ticks,
                                       PWM_Half_Bridge HalfBridge );
extern uint16_t PWM_calibrateMep( void );
extern void PWM_configBlanking( PWM_Module Mod, PWM_CmpSelect Select,
                                GPIO_Level Level, bool Async );
extern void PWM_setBlankingOffset( PWM_Module Mod, uint16_t Value );
extern void PWM_setBlankingWindow( PWM_Module Mod, uint8_t Value );

/********** USER MIDDLE SECTION ***********************************************/
#ifndef LIB_FUNC
#define PWM_isInt( Module )                     MACRO_PWM_isInt( Module )
#define PWM_clrInt( Module )                    MACRO_PWM_clrInt( Module )
#define PWM_getPeriod( Module )                 MACRO_PWM_getPeriod( Module )
#define PWM_setPeriod( Module, Ticks )          MACRO_PWM_setPeriod( Module, Ticks )
#define PWM_setDutyA( Module, Ticks )           MACRO_PWM_setDutyA( Module, Ticks )
#define PWM_setDutyHiRes( Module, Ticks )       MACRO_PWM_setDutyHiRes( Module, Ticks )
#define PWM_setDutyB( Module, Ticks )           MACRO_PWM_setDutyB( Module, Ticks )
#define PWM_softwareSync( Module )              MACRO_PWM_softwareSync( Module )
#define PWM_setPhase( Module, Value )           MACRO_PWM_setPhase( Module, Value )
#define PWM_getIndex( Mod )                     MACRO_PWM_getIndex( Mod )
#define PWM_getPieId( Mod )                     MACRO_PWM_getPieId( Mod )
#define PWM_ackInt( Mod )                       MACRO_PWM_ackInt( Mod )
#define PWM_freqToTicks( freq )                 MACRO_PWM_freqToTicks( freq )
#define PWM_freqToTicksClocks( freq, HspClkDiv, ClkDiv ) MACRO_PWM_freqToTicksClocks( freq, HspClkDiv, ClkDiv )
#define PWM_nsToTicks( Ns )                     MACRO_PWM_nsToTicks( Ns )
#define PWM_nsToTicksClocks( Ns, HspClkDiv, ClkDiv ) MACRO_PWM_nsToTicksClocks( Ns, HspClkDiv, ClkDiv )
#define PWM_clrTpzInt( Mod, Mode )              MACRO_PWM_clrTpzInt( Mod, Mode )
#define PWM_ackTpzInt( Mod, Mode )              MACRO_PWM_ackTpzInt( Mod, Mode )
#define PWM_getTzPieId( Tz )                    MACRO_PWM_getTzPieId( Tz )
#define PWM_getMod( Index )                     MACRO_PWM_getMod( Index )
#endif /* LIB_FUNC */


#define MACRO_PWM_isInt( Module )           ((Module)->ETFLG.all&1) /* INT flag*/
#define MACRO_PWM_clrInt( Module )          (Module)->ETCLR.all = (1<<0) /* INT flag*/
#define MACRO_PWM_getPeriod( Module )       ((Module)->TBPRD+1)
#define MACRO_PWM_setPeriod( Module, Ticks ) (Module)->TBPRD           = ((Ticks)-1)
#define MACRO_PWM_setDutyA( Module, Ticks ) (Module)->CMPA.half.CMPA   = Ticks
#define MACRO_PWM_setDutyHiRes( Module, Ticks ) (Module)->CMPA.all = (Ticks)
#define MACRO_PWM_setDutyB( Module, Ticks ) (Module)->CMPB             = Ticks
#define MACRO_PWM_softwareSync( Module )    (Module)->TBCTL.bit.SWFSYNC = 1
#define MACRO_PWM_setPhase( Module, Value ) (Module)->TBPHS.half.TBPHS = Value
#define MACRO_PWM_getIndex( Mod )           ((TL_PWM_MOD_SIZE*)(Mod)-(TL_PWM_MOD_SIZE*)PWM_MOD_1)
#define MACRO_PWM_getPieId( Mod )           ((INT_PieId)(PWM_getIndex(Mod)+(int)INT_ID_EPWM1))
#define MACRO_PWM_ackInt( Mod )             PWM_clrInt(Mod);INT_ackPieGroup(PWM_getPieId(Mod))
#define MACRO_PWM_freqToTicks( freq )       (SYS_CLK_HZ/(freq))
#define MACRO_PWM_freqToTicksClocks( freq, HspClkDiv, ClkDiv ) (SYS_CLK_HZ/(1L*SYS_LIT_VALUE(HspClkDiv)*SYS_LIT_VALUE(ClkDiv)*(freq)))
#define MACRO_PWM_nsToTicks( Ns )           ((1000L*(Ns))/SYS_CLK_PS)
#define MACRO_PWM_nsToTicksClocks( Ns, HspClkDiv, ClkDiv )((1000L*(Ns))/(SYS_CLK_PS*SYS_LIT_VALUE(HspClkDiv)*SYS_LIT_VALUE(ClkDiv)))
#define MACRO_PWM_clrTpzInt( Mod, Mode )    EALLOW; (Mod)->TZCLR.all = (Mode)|1; EDIS
#define MACRO_PWM_ackTpzInt( Mod, Mode )    PWM_clrTpzInt(Mod, Mode);INT_ackPieGroup(INT_ID_TZINT1)
#define MACRO_PWM_getTzPieId( Tz )          ((INT_PieId)(SYS_LIT_VALUE(Tz)+(int)INT_ID_TZINT1))
#define MACRO_PWM_getMod( Index )           ((PWM_Module)((TL_PWM_MOD_SIZE*)(PWM_MOD_1)+Index))

/********** END ***************************************************************/
#endif

