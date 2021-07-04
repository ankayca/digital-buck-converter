/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_c2000_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* This files contains all functions that are common to the C2000
* microcontroller.
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_C2000_PUB_H
#define _CSL_C2000_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/
#include "csl_stdint.h"
#include "csl_stdbool.h"

#include "csl_err_Pub.h"
#include "csl_cntrl_Pub.h"

/********** USER START SECTION ************************************************/
/*used for creating a literal that contains both the register value and calculating value*/
#define SYS_LIT( Value, Reg) (((Value)<<8) | Reg )
#define SYS_LIT_VALUE( Lit ) (((uint16_t)(Lit))>>8)
#define SYS_LIT_REG(   Lit ) (((uint16_t)(Lit))&0xFF)

#define NS_PER_SEC (1000000000L)

#define INT_GROUP_VAL( Group, Index ) ((Group-1)*8 + (Index-1))

/********** PROTOTYPES SECTIONS ***********************************************/

/* public methods */
extern void SYS_checkStack( void );
extern uint16_t SYS_getStackUnused( void );
extern void SYS_setTideMarker( void );
extern void SYS_dummyRamFuncs( void );
#ifdef LIB_FUNC
extern void SYS_usDelay( uint16_t Delay );
extern void SYS_msDelay( uint16_t Delay );
#endif /* LIB_FUNC */

/********** USER MIDDLE SECTION ***********************************************/
extern void SYS_initFunc( void );

#ifndef LIB_FUNC
#define SYS_usDelay( Delay )                    MACRO_SYS_usDelay( Delay )
#define SYS_msDelay( Delay )                    MACRO_SYS_msDelay( Delay )
#endif /* LIB_FUNC */

#define MACRO_SYS_usDelay( Delay )   DSP28x_usDelay((((1L*Delay*(SYS_CLK_HZ/10000L))/100L)-9L)/5L);
#define MACRO_SYS_msDelay( Delay )   {uint16_t ms; for(ms=0; ms<Delay;ms++){SYS_usDelay(1000);}}


/********** END ***************************************************************/
#endif

