/******************************************************************************
* (c) Copyright 2009 Biricha Digital Power Limited
* FILE          : main.c
* AUTHOR        : Dr C.J.Hossack
* PROJECT       : Piccolo B Buck Converter Peak Current Mode Control
* Target System : DSP C280x
* CREATION DATE : 07/05/2010
* COPYRIGHT     : Copyright Biricha Digital Power Limited 2009
*                 All rights reserved. Reproduction in whole or part is
*                 prohibited without written consent of the copyright
*                 owner.
* DESCRIPTION   :
*
* This project demonstrates Peak Current Mode Control of a Buck converter
* using the Piccolo B and its CLA.
* The main core is being used to run a 2p2z controller for the current mode
* Buck Converter. Piccolo's comparator 2 is being used to detect when the peak
* current reaches its demand value.
*
* Piccolo B's CLA is being used to create the negative slope ramp needed slope
* compensation.
*
* Phase and gain margins of the digital PSU were then measured using a
* frequency response analyser:
*
*     phase margin         = 42 degrees
*     gain  margin         = 15 db
*     cross over frequency = 15 kHz
*     switching frequency  = 200kHz
*
* IMPORTANT: The following pins are used by the code,
*
* PWM output:       GPIO0       (PWM_MOD_1, Channel A)
* Comparator input: GPIO3       (CMP_MOD_2)
* ADC input:        ADC_CH_B2
* Pin toggle:       GPIO12
*
* This example project only allows four IO pins to be used.
*
* LINKS
******************************************************************************/

/****************************** INCLUDES SECTION *****************************/

#include "csl.h"


/**************************** DECLARATIONS SECTION ***************************/


/* These set up the coefficients for our 2p2z controller for the Buck
*  converter with a 200kHz switching frequency and a cross over of 15kHz
*/

#define K   (0.5)
#define REF (_IQ15toF(2048))
#define MIN_DUTY 0
#define MAX_DUTY 1023
#define A1  +1.69020338
#define A2  -0.69020338
#define B0  +3.22868006
#define B1  +0.29060216
#define B2  -2.93807791


#define PERIOD_NS  5000 /*Our period in ns for fs = 200kHz */

/************************** POST DECLARATIONS SECTION ************************/

/* Data align memory before instantiating a 2p2z controller. */
#pragma DATA_ALIGN ( MyCntrl , 64 );


/* This effectively declares a 2p2z controller called MyCntrl */
CNTRL_2p2zData MyCntrl;


/* This macro generates CLA assembly code called SlopeTask, which implements
* slope compensation by subtracting a slope, of user defined gradient, from
* the demand value of the current before it is fed to the comparator.
*
* SlopeTask -> name of the CLA task
*
* 2 -> use comparator Mod2's DAC value
*
* 1 -> Clear PWM1's interrupt flag.
*      we specified in main() that PWM1's period
*      interrupt would trigger this CLA task
*
* -1.0 -> decrement the initial value on the DAC
*         by 1 every iteration. The initial value
*         was set by the 2p2z controller before
*         slope compensation.
*
* 80 ->  total number of decrements during one
*        sampling period
*
* Each decrement takes 50ns. Therefore 80 decrements will take 4us. This will
* give us a 1 us safety margin before the next switching interval.
*/
CLA_slopeCode( SlopeTask, 2,1, -1.0, 80 );

/****************************** FUNCTIONS SECTION ****************************/

/******************************************************************************
* FUNCTION      : IsrAdc
* DESCRIPTION   :
* This interrupt is called when the ADC sequencer has finished sampling.
******************************************************************************/
interrupt void IsrAdc( void )
{

    /* Sets GPIO pin 12 tied to TZ test pin on hardware */
    GPIO_set( GPIO_12);


     /* Ack group and ADC SEQ interrupt. Re-enable the ADC interrupts -Int1 */
     ADC_ackInt( ADC_INT_1 );


    /* These three lines read the ADC, call the 2p2z control loop & then update
    *  the duty cycle respectively.
    */
    MyCntrl.Fdbk.m_Int = ADC_getValue(ADC_MOD_1);
    CNTRL_2p2z(&MyCntrl);


    /* This inputs the "initial" value of the demand current (from the 2p2z)
    * controller to the DAC of the comparator. i.e. the demand current before
    * slope compensation is fed to the inverting pin of the on board
    * comparator 2. This initial DAC value will later get updated by
    * the CLAs slope compensation algorithm
    */
    CMP_setDac( CMP_MOD_2, MyCntrl.Out.m_Int );

    
    /* Clears GPIO12 pin */
    GPIO_clr( GPIO_12);


     /* Sets up soft-start*/
     CNTRL_2p2zSoftStartUpdate(&MyCntrl);
}


/******************************************************************************
* FUNCTION      : main
* DESCRIPTION   :
*
******************************************************************************/
void main( void )
{

    /* Initialize the MCU, ADC & GPIO12 */
    SYS_init();
    ADC_init();
    GPIO_config( GPIO_12, GPIO_DIR_OUT, false );
    

    /* Configures the CLA Mod1 to run CLA code "SlopeTask" whenever PWM trigger
    * occurs - The PWM event that causes the trigger is defined later.
    */
    CLA_config( CLA_MOD_1, &SlopeTask, CLA_INT_PWM );


    /* Setup PWM Mod1 for fs = 200kHz. PWM1 Ch A is being used for switching
    * the converter. PWM1 Ch B is being used for timing purposes - more on this
    * later.
    */
    PWM_config( PWM_MOD_1, PWM_nsToTicks(PERIOD_NS), PWM_COUNT_UP );
    PWM_pin( PWM_MOD_1, PWM_CH_A, GPIO_NON_INVERT );
    PWM_pin( PWM_MOD_1, PWM_CH_B, GPIO_NON_INVERT );


    /* Typically for digital current mode we set the PWM Ch A duty 100%; then
    * use the cycle by cycle trip function to pull the PWM pin low when the
    * current reaches our demand peak value. However for safety we have set the
    * maximum duty to 60%. i.e if your control algorithm fails, the PWM will
    * reset after 60% rather than staying at 100%.
    */
    PWM_setDutyA(PWM_MOD_1, PWM_nsToTicks(PERIOD_NS)*0.6 );




    /* PWM1 Channel A is being used for the PWM drive of the MOSFET. Hence, the
    * sampling, conversion, ADC interrupt entry, 2p2z, scaling and then DAC &
    * comparator set up must happen just before PWM1 Ch A goes high. For this
    * reason we use PWM1 Channel B to start the sampling process. The falling
    * edge of PWM1 Ch B is used to start the sampling process followed by all
    * relevant calculations. Therefore the duty of Ch B should be set such that
    * all sampling and calculations are completed just as PWM1 Ch A goes high.
    * This time has been measured on the scope as 2.45us.
    *
    *                       <---PERIOD_NS-->
    *                        ___             ___
    *        PWM A: _______|   |___________|   |___________|
    *
    *                              PERIOD_NS-2450
    *                        ________<-----> ________
    *        PWM B: _______|        |______|        |______|
    *                              ^      ^
    *   PWM B triggers ADC SOC here^      ^ here PWM B starts CLA slope
    *   This falling edge needs to be       compensation function for the next
    *   adjusted such that all              cycle
    *   calculations are completed
    *   before the next cycle
    *   (i.e. before the next rising edge)
    */



    /* This function sets the PWM1 Ch B such that the calculations are complete
    * just before the rising edge PWM A. PERIOD_NS is our period and set to
    * 5000 ns. Therefore we are setting our pulse width to (5000 - 2450) ns
    */
    PWM_setDutyB(PWM_MOD_1, PWM_nsToTicks( PERIOD_NS-2450+0 ) );



    /* This sets up the PWM Mod1 to start the ADC conversion whenever PWM1
    * Channel B timebase counter matches Ch B's duty. i.e. falling edge of PWM
    * Ch B triggers ADC SoC, As discussed above.
    */
    PWM_setAdcSoc( PWM_MOD_1, PWM_CH_B, PWM_INT_CMPB_UP );



    /* This sets up PWM Mod1 to generate an interrupt every PWM cycle whenever
    * timebase counter = 0. the "0" instead of an ISR function name means that
    * an interrupt is generated but no jump to an ISR function is carried out.
    * The CLA will detect this interrupt and run the CLA code instead. Finally
    * PWM_INT_PRD_1 indicates that an interrupt should be generated every cycle
    * as opposed to every other cycle
    */
    PWM_setCallback(PWM_MOD_1, 0, PWM_INT_ZERO, PWM_INT_PRD_1 );



    /*-----------------------------------------------------------**
    **  The next 5 functions set up the one-shot trip zone from  **
    **  comparator output and the leading edge blanking          **
    **-----------------------------------------------------------*/

    /* This effectively feeds the output of the comparator Mod2 into PWM Mod1
    * and activates the blanking by setting the digital compare event
    * PWM_DCEVT at the correct time. We will use PWM_DCEVT later to trip our
    * PWM.
    * The input to the blanking block is not inverted  & "true" ensures that
    * the output is not synchronized with the PWM's time-base
    * clock.
    */
    PWM_configBlanking( PWM_MOD_1, PWM_CMP_COMP2, GPIO_NON_INVERT, true );
    
    
    /* Sets the size of the blanking window to 420ns */
    PWM_setBlankingWindow( PWM_MOD_1, PWM_nsToTicks(420) );
    

    /* sets up the relevant trip zones: i.e. when PWM_DCEVT occurs clear
    * PWM1 Ch A on a cycle by cycle basis but take no action on PWM1 Ch B
    * PWM_DCEVT was set up in PWM_configBlanking().
    */
    PWM_setTripZone(  PWM_MOD_1, PWM_DCEVT, PWM_TPZ_CYCLE_BY_CYCLE );
    PWM_setTripState( PWM_MOD_1, PWM_CH_A, GPIO_CLR );
    PWM_setTripState( PWM_MOD_1, PWM_CH_B, GPIO_NO_ACTION );


    /* Configures ADC to sample Vo when triggered by PWM1 Ch B's falling edge
     */
    ADC_config( ADC_MOD_1, ADC_SH_WIDTH_7, ADC_CH_B2, ADC_TRIG_EPWM1_SOCB );

    /* When conversion is finished, cause interrupt and jump to IsrAdc */
    ADC_setCallback( ADC_MOD_1, IsrAdc, ADC_INT_1 );


    /* Initalise the 2p2z control structure */
    CNTRL_2p2zInit(&MyCntrl
        ,_IQ15(REF)
        ,_IQ26(A1),_IQ26(A2)
        ,_IQ26(B0),_IQ26(B1),_IQ26(B2)
        ,_IQ23(K),MIN_DUTY,MAX_DUTY
        );


    /* Configures the comparator Mod2 with 0 qualification window
    * i.e. asynchronous. The comparator output is not inverted & the inverting
    * input of the comparator is tied to the on board DAC.
    */
    CMP_config( CMP_MOD_2, CMP_ASYNC, GPIO_NON_INVERT, CMP_DAC );

    /* This line connects the output of the comparator to a GPIO pin. On the
    * Piccolo B part this is hard wired to GPIO3 which is connected to PWM2B
    * test pin of your daughter card.
    */
    CMP_pin( CMP_MOD_2 );


    /* Set up a 500ms soft-start */
    CNTRL_2p2zSoftStartConfig(&MyCntrl, 500, PERIOD_NS );


    /* Enables global interrupts and wait in idle loop */
    INT_enableGlobal(true);

    while(1)
    {

    }
}
