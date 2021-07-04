/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_cap_t0_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* The capture module allows you to take 4 samples of the 32 bit counter.
* Each module has 4 event channels that can be configured to store the counter
* value on either a rising or falling edge of the input capture pin.
* On each capture the counter can be reset to allow the difference between
* events to be stored.
* After the last event channel has been store the event channel can wrap around
* in continuous mode or stop in single shot mode.
*
* The counter is clocked by the system clock.
*
* EXAMPLES
* This sets up CAP_MOD_4 to capture the counter on event channel 1 on the
* negative edge and reset the counter. Then the event channel 2 is triggered on
* the next rising edge. This causes an interrupt and the positive width is
* stored in pos_wdith.
*
* CAP_config( CAP_MOD_4, CAP_DIV_1, CAP_CONTINUOUS,
*                 CAP_MOD_4_PIN_27, GPIO_SAMPLE_3 );
*
* CAP_setCapture( CAP_MOD_4, CAP_CH_1, CAP_CTR_DIF, CAP_EVENT_POS );
* CAP_setCapture( CAP_MOD_4, CAP_CH_2, CAP_CTR_ABS, CAP_EVENT_NEG );
* CAP_setCallback( CAP_MOD_4, Irs, CAP_INT_CEVT2 );
* CAP_stop(CAP_MOD_4, true);
*
* INT_enableGlobal(1);
*
*    interrupt void Irs( void )
*    {
*     CAP_ackInt( CAP_MOD_4, CAP_INT_CEVT2 );
*     pos_wdith = CAP_getValue( CAP_MOD_4, CAP_CH_2 );
*    }
*
*
*
* LINKS
*
* HISTORY       :
* 2010/01/20 hossa1c Created.
*******************************************************************************/


#ifndef _CSL_CAP_T0_PUB_H
#define _CSL_CAP_T0_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/
typedef struct TL_CAP_MOD_SIZE
{
    uint16_t A[0x20];
}TL_CAP_MOD_SIZE;

/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* COMPLEX       : CAP_MOD_X
* DESCRIPTION   :
* These values are used to specify the Capture module.
*******************************************************************************/
#if 1
#define CAP_MOD_1 (&ECap1Regs)
#define CAP_MOD_2 (&ECap2Regs)
#define CAP_MOD_3 (&ECap3Regs)
#define CAP_MOD_4 (&ECap4Regs)
#define CAP_MOD_5 (&ECap5Regs)
#define CAP_MOD_6 (&ECap6Regs)
#endif

/*******************************************************************************
* COMPLEX       : CAP_Module
* DESCRIPTION   :
* This is used to map hardware register values to CAP_Module.
*******************************************************************************/
typedef volatile struct ECAP_REGS* CAP_Module;

/*******************************************************************************
* ENUM          : CAP_ModuleChannel
* DESCRIPTION   :
* Each capture module has 4 capture channels.
*******************************************************************************/
typedef enum CAP_ModuleChannel
{
    CAP_CH_1,
    CAP_CH_2,
    CAP_CH_3,
    CAP_CH_4
} CAP_ModuleChannel;

/*******************************************************************************
* ENUM          : CAP_PreScale
* DESCRIPTION   :
* This is used to set the pre scalar to the capture module.
*******************************************************************************/
typedef enum CAP_PreScale
{
    CAP_DIV_1   = SYS_LIT( 1,   0 ),
    CAP_DIV_2   = SYS_LIT( 2,   1 ),
    CAP_DIV_4   = SYS_LIT( 4,   2 ),
    CAP_DIV_6   = SYS_LIT( 6,   3 ),
    CAP_DIV_8   = SYS_LIT( 8,   4 ),
    CAP_DIV_10  = SYS_LIT( 10,  5 ),
    CAP_DIV_12  = SYS_LIT( 12,  6 ),
    CAP_DIV_14  = SYS_LIT( 14,  7 ),
    CAP_DIV_16  = SYS_LIT( 16,  8 ),
    CAP_DIV_18  = SYS_LIT( 18,  9 ),
    CAP_DIV_20  = SYS_LIT( 20,  10 ),
    CAP_DIV_22  = SYS_LIT( 22,  11 ),
    CAP_DIV_24  = SYS_LIT( 24,  12 ),
    CAP_DIV_26  = SYS_LIT( 26,  13 ),
    CAP_DIV_28  = SYS_LIT( 28,  14 ),
    CAP_DIV_30  = SYS_LIT( 30,  15 ),
    CAP_DIV_32  = SYS_LIT( 32,  16 ),
    CAP_DIV_34  = SYS_LIT( 34,  17 ),
    CAP_DIV_36  = SYS_LIT( 36,  18 ),
    CAP_DIV_38  = SYS_LIT( 38,  19 ),
    CAP_DIV_40  = SYS_LIT( 40,  20 ),
    CAP_DIV_42  = SYS_LIT( 42,  21 ),
    CAP_DIV_44  = SYS_LIT( 44,  22 ),
    CAP_DIV_46  = SYS_LIT( 46,  23 ),
    CAP_DIV_48  = SYS_LIT( 48,  24 ),
    CAP_DIV_50  = SYS_LIT( 50,  25 ),
    CAP_DIV_52  = SYS_LIT( 52,  26 ),
    CAP_DIV_54  = SYS_LIT( 54,  27 ),
    CAP_DIV_56  = SYS_LIT( 56,  28 ),
    CAP_DIV_58  = SYS_LIT( 58,  29 ),
    CAP_DIV_60  = SYS_LIT( 60,  30 ),
    CAP_DIV_62  = SYS_LIT( 62,  31 )
} CAP_PreScale;

/*******************************************************************************
* ENUM          : CAP_CounterReset
* DESCRIPTION   :
* This is used to set what happens to the counter when a capture event occurs.
*******************************************************************************/
typedef enum CAP_CounterReset
{
    CAP_CTR_ABS     = 0,    /* Do not reset counter on Capture Event (absolute time stamp operation) */
    CAP_CTR_DIF     = 1     /* Reset counter after Capture Event time-stamp has been captured */
} CAP_CounterReset;

/*******************************************************************************
* ENUM          : CAP_EventPolarity
* DESCRIPTION   :
* This is used to set the edge polarity that causes a capture event.
*******************************************************************************/
typedef enum CAP_EventPolarity
{
    CAP_EVENT_POS   = 0,    /* Capture Event triggered on a rising edge */
    CAP_EVENT_NEG   = 1     /* Capture Event triggered on a falling edge */
} CAP_EventPolarity;

/*******************************************************************************
* ENUM          : CAP_Mode
* DESCRIPTION   :
* Continuous or one-shot mode control.
*******************************************************************************/
typedef enum CAP_Mode
{
    CAP_CONTINUOUS  = 0,    /* Operate in continuous mode */
    CAP_ONE_SHOT    = 1     /* Operate in one-Shot mode */
} CAP_Mode;

/*******************************************************************************
* ENUM          : CAP_IntMode
* DESCRIPTION   :
* This is used to indicate when the Capture interrupt occurs.
*******************************************************************************/
typedef enum CAP_IntMode
{
    CAP_INT             = (1<<0),   /* Global Interrupt Flag */
    CAP_INT_CEVT1       = (1<<1),   /* Capture Event channel 1 Interrupt Enable */
    CAP_INT_CEVT2       = (1<<2),   /* Capture Event channel 2 Interrupt Enable */
    CAP_INT_CEVT3       = (1<<3),   /* Capture Event channel 3 Interrupt Enable */
    CAP_INT_CEVT4       = (1<<4),   /* Capture Event channel 4 Interrupt Enable */
    CAP_INT_CTR_OVF     = (1<<5),   /* Counter Overflow Interrupt Enable */
    CAP_INT_CTR_PRD     = (1<<6),   /* Counter Equal Period Interrupt Enable */
    CAP_INT_CTR_CMP     = (1<<7),   /* Counter Equal Compare Interrupt Enable */
    CAP_INT_ALL         = 0xFF      /* All interrupt mask */
} CAP_IntMode;

/*******************************************************************************
* ENUM          : CAP_Pin
* DESCRIPTION   :
* This defines which pins each one the module can use and the required
* multiplexer.
*
* GPIO_ASSIGN_LIT(Module_Index, GPIO_Pin,  GPIO_Mux)
*******************************************************************************/
typedef enum CAP_Pin
{
    CAP_MOD_6_PIN_1     = GPIO_ASSIGN_LIT(6, 1,  2),
    CAP_MOD_5_PIN_3     = GPIO_ASSIGN_LIT(5, 3,  2),
    CAP_MOD_1_PIN_5     = GPIO_ASSIGN_LIT(1, 5,  3),
    CAP_MOD_2_PIN_7     = GPIO_ASSIGN_LIT(2, 7,  3),
    CAP_MOD_3_PIN_9     = GPIO_ASSIGN_LIT(3, 9,  3),
    CAP_MOD_4_PIN_11    = GPIO_ASSIGN_LIT(4, 11, 3),
    CAP_MOD_1_PIN_24    = GPIO_ASSIGN_LIT(1, 24, 1),
    CAP_MOD_2_PIN_25    = GPIO_ASSIGN_LIT(2, 25, 1),
    CAP_MOD_3_PIN_26    = GPIO_ASSIGN_LIT(3, 26, 1),
    CAP_MOD_4_PIN_27    = GPIO_ASSIGN_LIT(4, 27, 1),
    CAP_MOD_1_PIN_34    = GPIO_ASSIGN_LIT(1, 34, 1),
    CAP_MOD_2_PIN_37    = GPIO_ASSIGN_LIT(2, 37, 1),
    CAP_MOD_5_PIN_48    = GPIO_ASSIGN_LIT(5, 48, 1),
    CAP_MOD_6_PIN_49    = GPIO_ASSIGN_LIT(6, 49, 1)
} CAP_Pin;


/********** PROTOTYPES SECTIONS ***********************************************/

/* public methods */
extern void CAP_config( CAP_Module Mod, CAP_PreScale PreScale, CAP_Mode Mode,
                        CAP_Pin Pin, GPIO_InputMode InputMode );
extern void CAP_enableLoad( CAP_Module Mod, int Enable );
extern uint32_t CAP_getValue( CAP_Module Mod, CAP_ModuleChannel Ch );
extern void CAP_setReArm( CAP_Module Mod );
extern void CAP_setMaxChannel( CAP_Module Mod, CAP_ModuleChannel Ch );
extern void CAP_stop( CAP_Module Mod, int Disable );
extern void CAP_setCapture( CAP_Module Mod, CAP_ModuleChannel Channel,
                            CAP_CounterReset Reset, CAP_EventPolarity Polarity );
extern int CAP_getIndex( CAP_Module Mod );
extern INT_PieId CAP_getPieId( CAP_Module Mod );
extern void CAP_enableInt( CAP_Module Mod, int Mask );
extern void CAP_setCallback( CAP_Module Mod, INT_IsrAddr Func, int Mask );
extern uint16_t CAP_getIntFlags( CAP_Module Mod );
extern void CAP_clrInt( CAP_Module Mod, int Mask );
extern void CAP_ackInt( CAP_Module Mod, int Mask );
extern void CAP_setMode( CAP_Module Mod, CAP_Mode Mode );
extern void CAP_softwareStart( CAP_Module Mod, int Mask );
extern void CAP_setCounter( CAP_Module Mod, uint32_t Value );
#ifdef LIB_FUNC
extern uint32_t CAP_nsToTicks( uint64_t Ns );
extern uint32_t CAP_usToTicks( uint32_t Us );
#endif /* LIB_FUNC */

/********** USER MIDDLE SECTION ***********************************************/


#ifndef LIB_FUNC
#define CAP_nsToTicks( Ns )                     MACRO_CAP_nsToTicks( Ns )
#define CAP_usToTicks( Us )                     MACRO_CAP_usToTicks( Us )
#endif /* LIB_FUNC */


#define MACRO_CAP_nsToTicks( Ns )           ((1000L*(Ns))/SYS_CLK_PS)
#define MACRO_CAP_usToTicks( Us )           ((1000L*(Us))/(SYS_CLK_PS/1000L))


/********** END ***************************************************************/
#endif

