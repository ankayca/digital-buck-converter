/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_int_t0_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Contains functions to handle and configure the interrupts associated with the
* Peripheral Interrupt Expansion controller.
*
* The CPU is limited to 12 interrupt signals. Therefore to service all the
* interrupts from the modules and external sources a Peripheral Interrupt
* Expansion controller (PIE) is used.
*
* The PIE is a complicated multiplexing interrupt module. These functions
* attempts to simplify the use of the PIE controller.
*
* For more information about the PIE read "TMS320x280x, 2801x, 2804x DSP System
* Control and Interrupts" and familiarize yourself with the terminology used and
* how the controller functions.
*
* The functions from this module can be used to set up an interrupt service
* routine (ISR) to be called when an interrupt is raised by another module.
* The example below shows how this is possible.
*
* Please check to see if a customized interrupt configuration function is
* available for the specific module that is being used. Check by browsing the
* functions associated with that module. For example, when using the ADC module
* the function ADC_setCallaback() can be used to assign an ISR to the ADC
* interrupt event. The same result can be achieved using the functions provided
* in this module.
*
* EXAMPLES
* Assigns the interrupt service routine function to the interrupt vector
* associated with the ePWM module 1 interrupt. Enables the interrupt within the
* PIE controller and also enables the global interrupt flag.
*
*   INT_setCallback( INT_pieIdToVectorId( INT_ID_EPWM1 ), isr_pwm1 );
*   INT_enablePieId( INT_ID_EPWM1, true );
*   INT_enableGlobal( true );
*
* The interrupt service routine must acknowledge the correct interrupt and
* clear the peripheral interrupt flag.
*
*   interrupt void isr_pwm1( void )
*   {
*      // Acknowledge PIE interrupt group and clear peripheral interrupt flag
*      INT_ackPieId(INT_ID_EPWM1);
*   }
*
* NOTES
* INT_setCallback() and INT_enablePieId() should only be called when global
* interrupts are disabled.
*
* Convert a INT_PieId to a GROUP_Id using INT_pieIdToGroup().
*
* Convert a INT_PieId to a bit index using INT_pieIdToIndex().
*
* LINKS
* file:///C:/tidcs/c28/CSL_C280x/v100/doc/CSL_C280x.pdf
* http://focus.ti.com/lit/ug/spru712f/spru712f.pdf
*
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_INT_T0_PUB_H
#define _CSL_INT_T0_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/


/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* ENUM          : INT_VectorId
* DESCRIPTION   :
* This is a short list of PIE vectors. There are a lot more not listed below,
* but these are generally generated from a INT_PieId value and
* INT_pieIdToVectorId().
*******************************************************************************/
typedef enum INT_VectorId
{
    INT_VECT_RESET      = 0,
    INT_VECT_INT_13     = 13,   /* TIM_MOD_2 */
    INT_VECT_INT_14     = 14    /* TIM_MOD_3 */
} INT_VectorId;

/*******************************************************************************
* ENUM          : INT_PieGroup
* DESCRIPTION   :
* This is the PIE groups 1 to 12. Each PIE group can have up to 8 different
* interrupt sources.
*******************************************************************************/
typedef enum INT_PieGroup
{
    INT_GROUP_1,
    INT_GROUP_2,
    INT_GROUP_3,
    INT_GROUP_4,
    INT_GROUP_5,
    INT_GROUP_6,
    INT_GROUP_7,
    INT_GROUP_8,
    INT_GROUP_9,
    INT_GROUP_10,
    INT_GROUP_11,
    INT_GROUP_12
} INT_PieGroup;

/*******************************************************************************
* ENUM          : INT_PieIndex
* DESCRIPTION   :
* This is the index to each PIE Group.
*******************************************************************************/
typedef enum INT_PieIndex
{
    INT_INDEX_1,
    INT_INDEX_2,
    INT_INDEX_3,
    INT_INDEX_4,
    INT_INDEX_5,
    INT_INDEX_6,
    INT_INDEX_7,
    INT_INDEX_8
} INT_PieIndex;

/*******************************************************************************
* COMPLEX       : INT_IsrAddr
* DESCRIPTION   :
* This is the prototype for the interrupt service functions.
*******************************************************************************/
typedef interrupt void(*INT_IsrAddr)(void);


/********** PROTOTYPES SECTIONS ***********************************************/

/*******************************************************************************
* MACRO         : INT_ackPieId
* INPUT         : INT_PieIndex PieId
*                 Selects the peripheral interrupt id.
* RETURNS       : void
* DESCRIPTION   :
* Clears both the acknowledgement bit and the interrupt flag for the specified
* INT_PieIndex.
*
* The function calls both INT_ackPieGroup() and INT_ackPieIndex().
*
* This must be called after an interrupt service routine (ISR) has been entered.
* It will clear both the acknowledgement bit and interrupt flag allowing
* any subsequent interrupt flags to be serviced by PIE controller.
*
* This should only be called from inside a interrupt handler or when global
* interrupts are disabled to avoid missing other interrupts within the group.
*  EXAMPLES
* Clears bit 1 in the PIE acknowledgement register. Clears bit 4 in the PIE
* Group 1 interrupt flag register.
*
*   INT_ackPieId( INT_ID_XINT1 );
*
*******************************************************************************/
#define INT_ackPieId( PieId ) INT_ackPieGroup(PieId);INT_ackPieIndex(PieId)

/*******************************************************************************
* MACRO         : INT_pieIdToVectorId
* INPUT         : INT_PieIndex PieId
*                 Selects the peripheral interrupt id.
* RETURNS       : INT_VectorId
* DESCRIPTION   :
* Converts the INT_PieIndex to the corresponding vector ID (INT_VectorId).
*
* This can be used to assign an interrupt function to the interrupt vector.
*  EXAMPLES
* Assigns the interrupt function for INT_ID_TZINT1.
*
*   INT_setCallback( INT_pieIdToVectorId( INT_ID_TZINT1 ), epwm1_tzint_isr );
*
*******************************************************************************/
#define INT_pieIdToVectorId( PieId ) ((INT_VectorId)  ((PieId)+32))

/*******************************************************************************
* MACRO         : INT_pieIdToGroup
* INPUT         : INT_PieIndex PieId
*                 Selects the peripheral interrupt ID.
* RETURNS       : INT_PieGroup
* DESCRIPTION   :
* Converts the interrupt ID (INT_PieIndex) to the corresponding PIE interrupt
* group (INT_PieGroup).
*
* The format of the PIE multiplexed interrupts are,
*
*   INTx.y
*
* Where up to eight interrupts, y, are mapped on to the twelve system
* interrupts, x, INT1 to INT12. This function returns the 'x' value.
*
* EXAMPLES
* Returns INT_GROUP_3 (2) as the bit index of INT_ID_EPWM1 within its interrupt
* group.
*
*   PieGroup = INT_pieIdToGroup( INT_ID_EPWM1 );
*
*******************************************************************************/
#define INT_pieIdToGroup( PieId ) ((INT_PieGroup)((PieId)>>3))

/*******************************************************************************
* MACRO         : INT_pieIdToIndex
* INPUT         : INT_PieIndex PieId
*                 Selects the peripheral interrupt ID.
* RETURNS       : INT_PieIndex
* DESCRIPTION   :
* Converts the interrupt ID (INT_PieIndex) to the bit index (INT_PieIndex) of
* the multiplexed interrupt within the PIE interrupt group.
*
* The format of the PIE multiplexed interrupts are,
*
*   INTx.y
*
* Where up to eight interrupts, y, are mapped on to the twelve system
* interrupts, x, INT1 to INT12. This function returns the 'y' value.
*
* EXAMPLES
* Returns INT_INDEX_1 (0) as the bit index of INT_ID_EPWM1 within its interrupt
* group.
*
*   PieIndex = INT_pieIdToIndex( INT_ID_EPWM1 );
*
*******************************************************************************/
#define INT_pieIdToIndex( PieId ) ((INT_PieIndex)  ((PieId)&7 ))

/*******************************************************************************
* MACRO         : INT_ackPieGroup
* INPUT         : INT_PieIndex PieId
*                 Selects the peripheral interrupt id.
* RETURNS       : void
* DESCRIPTION   :
* Clears the bit corresponding of the PIE group within the PIE acknowledgement
* register for the specified PIE ID.
*
* This must be called after an interrupt service routine (ISR) has been entered.
* If the PIE acknowledgement flag for this PIE group is not cleared then any
* subsequent interrupts will not be serviced by the PIE controller.
*
* This should only be called from inside a interrupt handler or when global
* interrupts are disabled to avoid missing other interrupts within the group.
*  EXAMPLES
* Clears bit 1 in the PIE acknowledgement register.
*
*   INT_ackPieGroup( INT_ID_XINT1 );
*
* The same result can be achieved by using INT_ackGroup() if the group ID
* associated with the device generating an interrupt is known.
*
*******************************************************************************/
#define INT_ackPieGroup( PieId ) INT_ackGroup( INT_pieIdToGroup(PieId) )

/* public methods */
extern void INT_setCallback( INT_VectorId VectorId, INT_IsrAddr Func );
extern void INT_enableGlobal( int Enable );
extern void INT_enableInt( int IntId );
extern void INT_enablePieIndex( INT_PieId PieId, int Value );
extern void INT_enablePieGroup( INT_PieId PieId, int Value );
extern void INT_enablePieId( INT_PieId PieId, int Value );
#ifdef LIB_FUNC
extern void INT_ackInt( int IntId );
#endif /* LIB_FUNC */
extern void INT_ackPieIndex( INT_PieId PieId );
#ifdef LIB_FUNC
extern void INT_ackGroup( INT_PieGroup GroupId );
#endif /* LIB_FUNC */

/********** USER MIDDLE SECTION ***********************************************/

#ifndef LIB_FUNC
#define INT_ackGroup( GroupId )         MACRO_INT_ackGroup( GroupId )
#define INT_ackInt( IntId )             MACRO_INT_ackInt( IntId)
#endif /* LIB_FUNC */

#define MACRO_INT_ackGroup( GroupId )   PieCtrlRegs.PIEACK.all = (1<<(GroupId))
#define MACRO_INT_ackInt( IntId )       IFR &= ~(1<<IntId)

/********** END ***************************************************************/
#endif

