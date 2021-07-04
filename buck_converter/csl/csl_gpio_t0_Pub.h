/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_gpio_t0_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Contains functions to acquire the GPIO pins for use by the user and other
* modules such as the ePWM and UART.
*
* Once a pin has been acquired no other module can use it. If another module
* attempts to acquire an already acquired pin an assertion is raised.
*
* EXAMPLES
* Configures two GPIO pins, one as an input and the other as an output. The
* input pin is read and the output pin is set to the same value.
*
*   GPIO_config( GPIO_32, GPIO_DIR_IN,  false );
*   GPIO_config( GPIO_31, GPIO_DIR_OUT, false );
*
*    while ( 1 )
*    {
*       GPIO_setValue( GPIO_31, GPIO_get( GPIO_32 ) );
*    }
*
* LINKS
* file:///C:/tidcs/c28/CSL_C280x/v100/doc/CSL_C280x.pdf
* http://focus.ti.com/lit/ug/spru712f/spru712f.pdf
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_GPIO_T0_PUB_H
#define _CSL_GPIO_T0_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/

/* These are used to directly affect the registers instead of using bit fields.
 * This allows the macros to run faster.
 */
#define GPIO_getPinRegPtr( Reg, Pin ) ((volatile uint16_t*)&GpioDataRegs.Reg.all + (((Pin)>>5)*8) + ((Pin)&0x10?1:0))
#define GPIO_getPinRegVal( Pin ) (((uint16_t)1)<<((Pin)&0xF))

#define GPIO_ASSIGN_LIT( Mod, Pin, Mux) ( (((Mod)-1)<<12) | ((Pin)<<4) | (Mux) )
#define GPIO_LIT_MOD( Lit ) ((int)           ( (Lit>>12)&0x0F ))
#define GPIO_LIT_PIN( Lit ) ((GPIO_Pin)      ( (Lit>>4 )&0xFF ))
#define GPIO_LIT_MUX( Lit ) ((GPIO_Multiplex)( (Lit    )&0x0F ))


/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* ENUM          : GPIO_Pin
* DESCRIPTION   :
* These are the GPIO pins.
*******************************************************************************/
typedef enum GPIO_Pin
{
    GPIO_0,
    GPIO_1,
    GPIO_2,
    GPIO_3,
    GPIO_4,
    GPIO_5,
    GPIO_6,
    GPIO_7,
    GPIO_8,
    GPIO_9,
    GPIO_10,
    GPIO_11,
    GPIO_12,
    GPIO_13,
    GPIO_14,
    GPIO_15,
    GPIO_16,
    GPIO_17,
    GPIO_18,
    GPIO_19,
    GPIO_20,
    GPIO_21,
    GPIO_22,
    GPIO_23,
    GPIO_24,
    GPIO_25,
    GPIO_26,
    GPIO_27,
    GPIO_28,
    GPIO_29,
    GPIO_30,
    GPIO_31,
    GPIO_32,
    GPIO_33,
    GPIO_34,        /* last 2808 pin */
    GPIO_35,
    GPIO_36,
    GPIO_37,
    GPIO_38,
    GPIO_39,
    GPIO_40,
    GPIO_41,
    GPIO_42,
    GPIO_43,
    GPIO_44,
    GPIO_45,
    GPIO_46,
    GPIO_47,
    GPIO_48,
    GPIO_49,
    GPIO_50,
    GPIO_51,
    GPIO_52,
    GPIO_53,
    GPIO_54,
    GPIO_55,
    GPIO_56,
    GPIO_57,
    GPIO_58,
    GPIO_59,
    GPIO_60,
    GPIO_61,
    GPIO_62,
    GPIO_63,
    Gpio_MAX
} GPIO_Pin;

/*******************************************************************************
* ENUM          : GPIO_Direction
* DESCRIPTION   :
* This specifies the GPIO as an input or output pin.
*******************************************************************************/
typedef enum GPIO_Direction
{
    GPIO_DIR_IN     = 0,    /* input */
    GPIO_DIR_OUT    = 1     /* output */
} GPIO_Direction;

/*******************************************************************************
* ENUM          : GPIO_Multiplex
* DESCRIPTION   :
* Each GPIO pin can have different functionality. This is selected by specifing
* the correct mux for each pin.
*******************************************************************************/
typedef enum GPIO_Multiplex
{
    GPIO_MUX_GPIO   = 0,
    GPIO_MUX_ALT1   = 1,
    GPIO_MUX_ALT2   = 2,
    GPIO_MUX_ALT3   = 3
} GPIO_Multiplex;

/*******************************************************************************
* ENUM          : GPIO_Level
* DESCRIPTION   :
* This is used to indicate the polarity of the pin.
*******************************************************************************/
typedef enum GPIO_Level
{
    GPIO_NON_INVERT,        /* non-invert output */
    GPIO_INVERT             /* invert output */
} GPIO_Level;

/*******************************************************************************
* ENUM          : GPIO_TriState
* DESCRIPTION   :
* This is used to indicate the tri state value of the pin.
*******************************************************************************/
typedef enum GPIO_TriState
{
    GPIO_FLOAT,         /* pin set to high impedance */
    GPIO_SET,           /* pin set to logic 1 */
    GPIO_CLR,           /* pin set to logic 0 */
    GPIO_NO_ACTION      /* no action taken */
} GPIO_TriState;

/*******************************************************************************
* ENUM          : GPIO_InputMode
* DESCRIPTION   :
* Selects input qualification type for GPIO pins.
*******************************************************************************/
typedef enum GPIO_InputMode
{
    GPIO_SYNCHRONIZE,       /* Synchronize to SYSCLKOUT only. Valid for both peripheral and GPIO pins */
    GPIO_SAMPLE_3,          /* Qualification using 3 samples */
    GPIO_SAMPLES_6,         /* Qualification using 6 samples */
    GPIO_ASYNCHRONOUS       /* Asynchronous. This option applies to pins configured as peripherals only */
} GPIO_InputMode;


/********** PROTOTYPES SECTIONS ***********************************************/

/* public methods */
extern void GPIO_acquire( GPIO_Pin Pin );
extern int GPIO_getLimit( void );
extern void GPIO_config( GPIO_Pin Pin, GPIO_Direction Direction, bool PullUp );
extern void GPIO_setMux( GPIO_Pin Pin, GPIO_Multiplex Mux );
extern void GPIO_reConfig( GPIO_Pin PinNumber, GPIO_Direction Direction,
                           bool PullUp, GPIO_Multiplex Mux,
                           GPIO_InputMode InputMode );
extern void GPIO_setValue( GPIO_Pin Pin, int Value );
#ifdef LIB_FUNC
extern void GPIO_set( GPIO_Pin Pin );
extern void GPIO_clr( GPIO_Pin Pin );
extern void GPIO_tog( GPIO_Pin Pin );
extern uint16_t GPIO_get( GPIO_Pin Pin );
#endif /* LIB_FUNC */

/********** USER MIDDLE SECTION ***********************************************/
#ifndef LIB_FUNC
#define GPIO_set(  Pin )  MACRO_GPIO_set( Pin )
#define GPIO_clr(  Pin )  MACRO_GPIO_clr( Pin )
#define GPIO_tog(  Pin )  MACRO_GPIO_tog( Pin )
#define GPIO_get(   Pin ) MACRO_GPIO_get( Pin )
#endif /* LIB_FUNC */


#define MACRO_GPIO_set( Pin ) *GPIO_getPinRegPtr(GPASET,    Pin) = GPIO_getPinRegVal(Pin)
#define MACRO_GPIO_clr( Pin ) *GPIO_getPinRegPtr(GPACLEAR,  Pin) = GPIO_getPinRegVal(Pin)
#define MACRO_GPIO_tog( Pin ) *GPIO_getPinRegPtr(GPATOGGLE, Pin) = GPIO_getPinRegVal(Pin)
#define MACRO_GPIO_get( Pin ) (*GPIO_getPinRegPtr(GPADAT,   Pin) & GPIO_getPinRegVal(Pin))


/********** END ***************************************************************/
#endif

