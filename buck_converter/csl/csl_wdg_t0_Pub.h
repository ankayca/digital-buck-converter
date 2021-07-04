/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_wdg_t0_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Contains functions to configure and use the watch dog module.
*
* By default the watch dog timer is disabled.
*
* EXAMPLES
* Configures the watch dog timer to cause a reset if it expires. The main idle
* loop kicks the watch dog to prevent this from occurring.
*
*   WDG_config( WDG_RESET, NULL );
*
*   while(1)
*   {
*      WDG_kick();
*   }
*
* LINKS
* file:///C:/tidcs/c28/CSL_C280x/v100/doc/CSL_C280x.pdf
* http://focus.ti.com/lit/ug/spru712f/spru712f.pdf
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_WDG_T0_PUB_H
#define _CSL_WDG_T0_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/


/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* ENUM          : WDG_Mode
* DESCRIPTION   :
* This is used to select the different watch dog modes of operation.
*******************************************************************************/
typedef enum WDG_Mode
{
    WDG_NONE,               /* disable watchdog */
    WDG_RESET,              /* cause a reset */
    WDG_RESET_LOCK,         /* cause a reset, can't be changed */
    WDG_INTERRUPT,          /* cause an interrupt */
    WDG_INTERRUPT_LOCK      /* cause an interrupt, can't be changed */
} WDG_Mode;


/********** PROTOTYPES SECTIONS ***********************************************/

/*******************************************************************************
* MACRO         : WDG_ackInt
* RETURNS       : void
* DESCRIPTION   :
* Used within an interrupt service routine (ISR) to clear the watch dog PIE
* group flag.
*
* EXAMPLES
* The watch dog is configured to call the ISR isr_wdg() if its timer expires.
* Inside the ISR the interrupt flags are cleared to allow future watch dog
* interrupt functions to be called.
*
*    WDG_config( WDG_INTERRUPT, isr_wdg );
*
*   interrupt void isr_wdgr( void )
*   {
*      WDG_ackInt();
*   }
*
*******************************************************************************/
#define WDG_ackInt(  ) INT_ackPieGroup(INT_ID_WAKE)

/* public methods */
extern void WDG_config( WDG_Mode Mode, INT_IsrAddr Func );
extern void WDG_kick( void );

/********** USER MIDDLE SECTION ***********************************************/


/********** END ***************************************************************/
#endif

