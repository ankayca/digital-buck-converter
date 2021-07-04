/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_sys_c2803x_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Contains functions to configure the system, including the system clock.
*  SYS_int() must be called before using any of CSL module functions.
*
* By default, it is assumed that a 60MHz system clock is being used. Therefore,
*
*  Low Speed Clock  (LSPCLK) = System Clock (SYSCLK) / 4 = 15MHz
*
* These settings are defined in csl_sys_c2803x_Pub.h using the macros shown
* below. These macros are then used through the rest of the library to
* calculate various frequency/ns to peripheral ticks.
*
*    #define USR_CLK_IN_HZ      10000000L
*    #define USR_PLL_MUL        SYS_PLL_MUL_6
*    #define USR_CLK_DIV        SYS_CLK_DIV_1
*    #define USR_PER_LSP_DIV    SYS_PER_CLK_DIV_4
*
* If a 60MHz system clock is not being used then either change these values
* directly or define new values before csl_sys_c2803x_Pub.h is included.
* Alternatively, define any changes within the build options as a processor
* define option e.g.
*
*      -d"USR_PLL_MUL=SYS_PLL_MUL_6"
*
* The LSPCLK is used by SCI-A/B and SPI-A/B/C/D.
* The SYSCLK is used by all other peripherals including the PWM.
*
* The csl for the piccolo B assumes you are using the internal 10MHz oscillator
* If you want to use the external crystal you would need to turn on the external
* circuits and select the external clock input after calling SYS_init(). eg
*
*    CLKCTL[XTALOSCOFF]    = 1
*    CLKCTL[OSCCLKSRC2SEL] = 0
*    CLKCTL[OSCCLKSRCSEL]  = 1
*
* LINKS
* file:///C:/tidcs/c28/CSL_C280x/v100/doc/CSL_C280x.pdf
* http://focus.ti.com/lit/ug/spru712f/spru712f.pdf
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_SYS_C2803X_PUB_H
#define _CSL_SYS_C2803X_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/


/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* ENUM          : SYS_ClockDivide
* DESCRIPTION   :
* This is used to select the system clock divider.
*******************************************************************************/
typedef enum SYS_ClockDivide
{
    SYS_CLK_DIV_4   = SYS_LIT( 4,1),
    SYS_CLK_DIV_2   = SYS_LIT( 2,2),
    SYS_CLK_DIV_1   = SYS_LIT( 1,3)
} SYS_ClockDivide;

/*******************************************************************************
* ENUM          : SYS_PllMultiplier
* DESCRIPTION   :
* This is used to select the system clock multiplier.
*******************************************************************************/
typedef enum SYS_PllMultiplier
{
    SYS_PLL_MUL_BYPASS  = SYS_LIT( 1,   0),
    SYS_PLL_MUL_1       = SYS_LIT( 1,   1),
    SYS_PLL_MUL_2       = SYS_LIT( 2,   2),
    SYS_PLL_MUL_3       = SYS_LIT( 3,   3),
    SYS_PLL_MUL_4       = SYS_LIT( 4,   4),
    SYS_PLL_MUL_5       = SYS_LIT( 5,   5),
    SYS_PLL_MUL_6       = SYS_LIT( 6,   6),
    SYS_PLL_MUL_7       = SYS_LIT( 7,   7),
    SYS_PLL_MUL_8       = SYS_LIT( 8,   8),
    SYS_PLL_MUL_9       = SYS_LIT( 9,   9),
    SYS_PLL_MUL_10      = SYS_LIT( 10, 10),
    SYS_PLL_MUL_11      = SYS_LIT( 11, 11),
    SYS_PLL_MUL_12      = SYS_LIT( 12, 12)
} SYS_PllMultiplier;

/*******************************************************************************
* ENUM          : SYS_ClockOutDivide
* DESCRIPTION   :
* This is used to select the system output clock.
*******************************************************************************/
typedef enum SYS_ClockOutDivide
{
    SYS_CLK_OUT_DIV_4   = SYS_LIT( 4,   0),
    SYS_CLK_OUT_DIV_2   = SYS_LIT( 2,   1),
    SYS_CLK_OUT_DIV_1   = SYS_LIT( 1,   2),
    SYS_CLK_OUT_NONE    = SYS_LIT( 0,   3)
} SYS_ClockOutDivide;

/*******************************************************************************
* ENUM          : SYS_PerClockDivide
* DESCRIPTION   :
* These are the divide options for the system clock, based on a 100MHz system
* clock.
*
*******************************************************************************/
typedef enum SYS_PerClockDivide
{
    SYS_PER_CLK_DIV_1   = SYS_LIT( 1,  0),  /* 100.00MHz */
    SYS_PER_CLK_DIV_2   = SYS_LIT( 2,  1),  /* 50.00MHz */
    SYS_PER_CLK_DIV_4   = SYS_LIT( 4,  2),  /* 25.00MHz */
    SYS_PER_CLK_DIV_6   = SYS_LIT( 6,  3),  /* 16.66MHz */
    SYS_PER_CLK_DIV_8   = SYS_LIT( 8,  4),  /* 12.50MHz */
    SYS_PER_CLK_DIV_10  = SYS_LIT( 10, 5),  /* 10.00MHz */
    SYS_PER_CLK_DIV_12  = SYS_LIT( 12, 6),  /* 8.33MHz */
    SYS_PER_CLK_DIV_14  = SYS_LIT( 14, 7)   /* 7.14MHz */
} SYS_PerClockDivide;

/*******************************************************************************
* COMPLEX       : USR_CLK_IN_HZ
* DESCRIPTION   :
* This is the default input frequency to the DSP chip.
*
* NOTES
* You can change this value here or in the build options. E.g.
*    -d"USR_CLK_IN_HZ=10000000L"
*******************************************************************************/
#ifndef USR_CLK_IN_HZ
#define USR_CLK_IN_HZ 10000000L
#endif

/*******************************************************************************
* COMPLEX       : USR_PLL_MUL
* DESCRIPTION   :
* This is the default DSP multiplier to the DSP chip.
*
* NOTES
* You can change this value here or in the build options. E.g.
*    -d"USR_PLL_MUL=SYS_PLL_MUL_5"
*******************************************************************************/
#ifndef USR_PLL_MUL
#define USR_PLL_MUL   SYS_PLL_MUL_6
#endif

/*******************************************************************************
* COMPLEX       : USR_CLK_DIV
* DESCRIPTION   :
* This is the default DSP divider for the DSP chip.
*
* NOTES
* You can change this value here or in the build options. E.g.
*    -d"USR_CLK_DIV=SYS_CLK_DIV_1"
*******************************************************************************/
#ifndef USR_CLK_DIV
#define USR_CLK_DIV   SYS_CLK_DIV_1
#endif

/*******************************************************************************
* COMPLEX       : USR_PER_LSP_DIV
* DESCRIPTION   :
* This is the default divider for the DSP low speed system clock.
*
* NOTES
* You can change this value here or in the build options. E.g.
*    -d"USR_PER_LSP_DIV=SYS_PER_CLK_DIV_1"
*******************************************************************************/
#ifndef USR_PER_LSP_DIV
#define USR_PER_LSP_DIV SYS_PER_CLK_DIV_4
#endif

/*******************************************************************************
* COMPLEX       : SYS_CLK_HZ
* DESCRIPTION   :
* This is the calculated system clock in Hz based on USR_CLK_IN_HZ, USR_PLL_MUL
* and USR_CLK_DIV.
*******************************************************************************/
#define SYS_CLK_HZ ((1L*USR_CLK_IN_HZ * SYS_LIT_VALUE(USR_PLL_MUL)) / (1L*SYS_LIT_VALUE(USR_CLK_DIV)))

/*******************************************************************************
* COMPLEX       : SYS_CLK_NS
* DESCRIPTION   :
* This is the calculated system clock in ns based on USR_CLK_IN_HZ, USR_PLL_MUL
* and USR_CLK_DIV.
*******************************************************************************/
#define SYS_CLK_NS (NS_PER_SEC/SYS_CLK_HZ)

/*******************************************************************************
* COMPLEX       : SYS_CLK_LSP_HZ
* DESCRIPTION   :
* This is the calculated low speed system clock in Hz based on SYS_CLK_HZ and
* USR_PER_LSP_DIV.
*******************************************************************************/
#define SYS_CLK_LSP_HZ (SYS_CLK_HZ / SYS_LIT_VALUE(USR_PER_LSP_DIV))

/*******************************************************************************
* COMPLEX       : SYS_CLK_PS
* DESCRIPTION   :
* This is the calculated system clock in ps based on USR_CLK_IN_HZ, USR_PLL_MUL
* and USR_CLK_DIV.
*******************************************************************************/
#define SYS_CLK_PS (NS_PER_SEC/(SYS_CLK_HZ/1000))

/*******************************************************************************
* ENUM          : INT_PieId
* DESCRIPTION   :
* This is the enum for each PIE interrupt source. Each value is defined as an
* INT_PieGroup and INT_PieIndex.
*******************************************************************************/
typedef enum INT_PieId
{
    INT_ID_ADCINT1H     = INT_GROUP_VAL( 1,  1  ),  /* high prioity ADC INT1 */
    INT_ID_ADCINT2H     = INT_GROUP_VAL( 1,  2  ),  /* high prioity ADC INT2 */
    INT_ID_XINT1        = INT_GROUP_VAL( 1,  4  ),  /* Group(1-12)   Index(1-8) */
    INT_ID_XINT2        = INT_GROUP_VAL( 1,  5  ),
    INT_ID_ADCINT9H     = INT_GROUP_VAL( 1,  6  ),
    INT_ID_TIM1         = INT_GROUP_VAL( 1,  7  ),  /* TIM2/3 are done using int13/14 */
    INT_ID_WAKE         = INT_GROUP_VAL( 1,  8  ),
    INT_ID_TZINT1       = INT_GROUP_VAL( 2,  1  ),
    INT_ID_TZINT2       = INT_GROUP_VAL( 2,  2  ),
    INT_ID_TZINT3       = INT_GROUP_VAL( 2,  3  ),
    INT_ID_TZINT4       = INT_GROUP_VAL( 2,  4  ),
    INT_ID_TZINT5       = INT_GROUP_VAL( 2,  5  ),
    INT_ID_TZINT6       = INT_GROUP_VAL( 2,  6  ),
    INT_ID_EPWM1        = INT_GROUP_VAL( 3,  1  ),
    INT_ID_EPWM2        = INT_GROUP_VAL( 3,  2  ),
    INT_ID_EPWM3        = INT_GROUP_VAL( 3,  3  ),
    INT_ID_EPWM4        = INT_GROUP_VAL( 3,  4  ),
    INT_ID_EPWM5        = INT_GROUP_VAL( 3,  5  ),
    INT_ID_EPWM6        = INT_GROUP_VAL( 3,  6  ),
    INT_ID_EPWM7        = INT_GROUP_VAL( 3,  7  ),
    INT_ID_ECAP1        = INT_GROUP_VAL( 4,  1  ),
    INT_ID_SPIRXA       = INT_GROUP_VAL( 6,  1  ),  /* SPI-A */
    INT_ID_SPITXA       = INT_GROUP_VAL( 6,  2  ),  /* SPI-A */
    INT_ID_SPIRXB       = INT_GROUP_VAL( 6,  3  ),  /* SPI-B */
    INT_ID_SPITXB       = INT_GROUP_VAL( 6,  4  ),  /* SPI-B */
    INT_ID_SPIRXC       = INT_GROUP_VAL( 6,  5  ),  /* SPI-C */
    INT_ID_SPITXC       = INT_GROUP_VAL( 6,  6  ),  /* SPI-C */
    INT_ID_SPIRXD       = INT_GROUP_VAL( 6,  7  ),  /* SPI-D */
    INT_ID_SPITXD       = INT_GROUP_VAL( 6,  8  ),  /* SPI-D */
    INT_ID_SCIRXINTA    = INT_GROUP_VAL( 9,  1  ),  /* SCI-A-RX */
    INT_ID_SCITXINTA    = INT_GROUP_VAL( 9,  2  ),  /* SCI-A-TX */
    INT_ID_SCIRXINTB    = INT_GROUP_VAL( 9,  3  ),  /* SCI-B-RX */
    INT_ID_SCITXINTB    = INT_GROUP_VAL( 9,  4  ),  /* SCI-B-TX */
    INT_ID_ADCINT1      = INT_GROUP_VAL( 10, 1  ),
    INT_ID_ADCINT2      = INT_GROUP_VAL( 10, 2  ),
    INT_ID_ADCINT3      = INT_GROUP_VAL( 10, 3  ),
    INT_ID_ADCINT4      = INT_GROUP_VAL( 10, 4  ),
    INT_ID_ADCINT5      = INT_GROUP_VAL( 10, 5  ),
    INT_ID_ADCINT6      = INT_GROUP_VAL( 10, 6  ),
    INT_ID_ADCINT7      = INT_GROUP_VAL( 10, 7  ),
    INT_ID_ADCINT8      = INT_GROUP_VAL( 10, 8  ),
    INT_ID_CLA1         = INT_GROUP_VAL( 11, 1  ),
    INT_ID_CLA2         = INT_GROUP_VAL( 11, 2  ),
    INT_ID_CLA3         = INT_GROUP_VAL( 11, 3  ),
    INT_ID_CLA4         = INT_GROUP_VAL( 11, 4  ),
    INT_ID_CLA5         = INT_GROUP_VAL( 11, 5  ),
    INT_ID_CLA6         = INT_GROUP_VAL( 11, 6  ),
    INT_ID_CLA7         = INT_GROUP_VAL( 11, 7  ),
    INT_ID_CLA8         = INT_GROUP_VAL( 11, 8  )
} INT_PieId;


/********** PROTOTYPES SECTIONS ***********************************************/

/*******************************************************************************
* MACRO         : SYS_init
* RETURNS       : void
* DESCRIPTION   :
* Initializes the Chip Support Library.
*
* This function must be called before any of the API functions are called.
*
* This function performs different initialization actions depending on the DSP
* chip being used. For all processors, this function initializes the stack,
* peripheral clocks and interrupt module. It copies time critical routines and
* the flash set up code to RAM.
*
* The clocking options are configured to the user defined values by the
* function.
*
*   SYS_configClk( USR_PLL_MUL, USR_CLK_DIV, SYS_CLK_OUT_DIV_1 );
*   SYS_setPerhiperalClk(USR_PER_HSP_DIV, USR_PER_LSP_DIV);
*
* EXAMPLES
* This initializes the CSL library.
*
*    SYS_init();
*
*******************************************************************************/
#define SYS_init(  ) SYS_initFunc();    \
                    SYS_configClk( USR_PLL_MUL, USR_CLK_DIV, SYS_CLK_OUT_DIV_1 ); \
                    SYS_setPerhiperalClk( USR_PER_LSP_DIV)

/* public methods */
extern void SYS_configClk( SYS_PllMultiplier InMultiplier,
                           SYS_ClockDivide InDiv, SYS_ClockOutDivide OutDiv );
extern void SYS_setPerhiperalClk( SYS_PerClockDivide LspDiv );

/********** USER MIDDLE SECTION ***********************************************/


/********** END ***************************************************************/
#endif

