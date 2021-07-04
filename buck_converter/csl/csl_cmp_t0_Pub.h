/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_cmp_t0_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Library
* DESCRIPTION COPY:
* Provides the functions necessary for configuring the comparator modules.
*
* CMP_config() must be called before using any of the comparator API functions.
* The comparator hardware contains 2 comparator modules which are accessed using
* the following pointers,
*
*   CMP_MOD_1
*   CMP_MOD_2
*
* The output of the comparator can be fed in to the ePWM module as
* PWM_CMP_COMP1 or PWM_CMP_COMP2.
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_CMP_T0_PUB_H
#define _CSL_CMP_T0_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/
typedef struct TL_CMP_MOD_SIZE
{
    uint16_t A[0x20];
}TL_CMP_MOD_SIZE;

/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* COMPLEX       : CMP_TYPE_0
* DESCRIPTION   :
* This can be use to determine the peripheral type used by the CSL.
*******************************************************************************/
#define CMP_TYPE_0

/*******************************************************************************
* COMPLEX       : CMP_MODULE_X
* DESCRIPTION   :
* These values are used to specify the Comparator module.
*******************************************************************************/
#if 1
#define CMP_MOD_1 (&Comp1Regs)
#define CMP_MOD_2 (&Comp2Regs)
#define CMP_MOD_3 (&Comp3Regs)
#endif

/*******************************************************************************
* COMPLEX       : CMP_Module
* DESCRIPTION   :
* This is used to map hardware register values to CMP_Module.
*******************************************************************************/
typedef volatile struct COMP_REGS* CMP_Module;

/*******************************************************************************
* ENUM          : CMP_Sample
* DESCRIPTION   :
*******************************************************************************/
typedef enum CMP_Sample
{
    CMP_SAMPLE_1    = 0,
    CMP_SAMPLE_2,
    CMP_SAMPLE_3,
    CMP_SAMPLE_4,
    CMP_SAMPLE_5,
    CMP_SAMPLE_6,
    CMP_SAMPLE_7,
    CMP_SAMPLE_8,
    CMP_SAMPLE_9,
    CMP_SAMPLE_10,
    CMP_SAMPLE_11,
    CMP_SAMPLE_12,
    CMP_SAMPLE_13,
    CMP_SAMPLE_14,
    CMP_SAMPLE_15,
    CMP_SAMPLE_16,
    CMP_ASYNC       = 0xFF
} CMP_Sample;

/*******************************************************************************
* ENUM          : CMP_Source
* DESCRIPTION   :
*******************************************************************************/
typedef enum CMP_Source
{
    CMP_DAC     = 0,    /* Comparator inverting input sourced from internal DAC */
    CMP_GPIO            /* Comparator inverting input sourced from input pin */
} CMP_Source;

/*******************************************************************************
* COMPLEX       : CMP_ValueMax
* DESCRIPTION   :
*******************************************************************************/
#define CMP_ValueMax (1023)


/********** PROTOTYPES SECTIONS ***********************************************/

/*******************************************************************************
* MACRO         : CMP_getGpioPin
* INPUT         : PWM_Module Mod
* RETURNS       : void
* DESCRIPTION   :
* Returns the GPIO pin associated with the CMP module channel A.
*
*   GPIO_3 = CMP_getGpioPin( CMP_MOD_2 )
*
*******************************************************************************/
#define CMP_getGpioPin( Mod ) ((GPIO_Pin)(Mod<=CMP_MOD_2?CMP_getIndex(Mod)*2+GPIO_1:GPIO_34))

/* public methods */
extern void CMP_config( CMP_Module Mod, CMP_Sample Sample, GPIO_Level Level,
                        CMP_Source Source );
extern void CMP_pin( CMP_Module Mod );
#ifdef LIB_FUNC
extern int CMP_getIndex( CMP_Module Mod );
extern CMP_Module CMP_getMod( int Index );
#endif /* LIB_FUNC */
extern uint16_t CMP_mVtoDacValue( uint16_t mVolts );
extern void CMP_setDac( CMP_Module Mod, uint16_t Value );

/********** USER MIDDLE SECTION ***********************************************/
#ifndef LIB_FUNC
#define CMP_getIndex( Mod )                     MACRO_CMP_getIndex( Mod )
#define CMP_getMod( Index )                     MACRO_CMP_getMod( Index )
#endif /* LIB_FUNC */

#define MACRO_CMP_getIndex( Mod )           ((TL_CMP_MOD_SIZE*)(Mod)-(TL_CMP_MOD_SIZE*)CMP_MOD_1)
#define MACRO_CMP_getMod( Index )           ((CMP_Module)((TL_CMP_MOD_SIZE*)(CMP_MOD_1)+Index))


/********** END ***************************************************************/
#endif

