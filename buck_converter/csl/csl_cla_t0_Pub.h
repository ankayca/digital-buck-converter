/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_cla_t0_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Library
* DESCRIPTION COPY:
* Contains functions to execute a digital 3 pole 3 zero (3p3z) and 2 pole 2 zero
* (2p2z) algorithm for use in the control of switch mode power supplies (SMPS)
* using the CLA.
*
* The CLA code must be declared using CLA_3p3zVMode() or CLA_2p2zVMode() and
* then initialized using CLA_config().
*
* The control functions have been optimized in Assembler for maximum speed.
*
*         type      instructions  us(60Mhz)
*    CLA_3p3zVMode()      46       .780us
*    CLA_2p2zVMode()      39       .650us
*
* The controllers have sufficient information in the current time step to
* pre-calculate some of the result for the following time step. This
* pre-calculation is performed after the duty has been updated. Therefore,
* due to the pre-calculation, the controller can calculate the current output
* and then update the duty within 290ns.
*
* This means that, when using early ADC interrupts, the ADC can be sampled
* and duty updated within 940ns. However, if shadow registers are turned on
* then the duty will not take effect until the next PWM period.
*
*               (see above)
*    880ns     -------------
* -------------|           |--------------
*              --->290ns PWM update (1170ns from edge to update)
*
*              (see above)
*    650ns   -------------   early ADC
* -----------|           |--------------
*            --->290ns PWM update (940ns from edge to update)
*
*
* The parameters for the 3p3z algorithm must be determined through control
* theory analysis of the system. The poles and zeros in the analogue frequency
* domain can be converted to the digital domain using the tool provided on the
* Biricha
* Digital Power website <http://www.biricha.com/resources/converter.php?type=4>
*
* Arguments are passed to the CLA_3p3zVMode() and CLA_2p2zVMode() functions as
* float numbers. Macros, constants or variables cannot be used.
*
* In the function CLA_setRef(), the argument REF is compared to the feedback
* value from the system under control. The CLA code reads the feedback value
* from the ADC and stores it within the structure during each cycle of the
* control loop. The CLA code is used to update the output value based on REF
* and this feedback value.
*
*  EXAMPLES
* Initializes the 3p3z structure with the correct coefficients. When ADC_MOD_7
* has completed a conversion the CLA code begins execution. This reads the
* value from the ADC result register. The duty is calculated and then PWM_MOD_3
* duty is updated all within the CLA code.
*
*   CLA_3p3zVMode( ClaTask, 7, 3,
*             +1.46818, -0.314933, -0.153248,
*              1.784224053, -1.629063952, -1.780916725, 1.632371281,
*              0.48, 0.0, 240.0 );
*
*   void main ( void )
*   {
*     SYS_init();
*     ADC_init();
*
*     PWM_config( PWM_MOD_3, PWM_freqToTicks(200000), PWM_COUNT_DOWN );
*     PWM_pin( PWM_MOD_3, PWM_CH_A, GPIO_NON_INVERT );
*     PWM_setAdcSoc( PWM_MOD_3, PWM_CH_A, PWM_INT_ZERO );
*
*     ADC_setEarlyInterrupt( 1 );
*     ADC_config( ADC_MOD_1, ADC_SH_WIDTH_7, ADC_CH_A0, ADC_TRIG_EPWM3_SOCA );
*     ADC_setCallback( ADC_MOD_1, 0, ADC_INT_7 );
*
*     CLA_setRef( CLA_getCtrlPtr(ClaTask), 2048 );
*     CLA_config( CLA_MOD_7, &ClaTask, CLA_INT_ADC );
*     CLA_setCallback( CLA_MOD_7, IsrFunc );
*
*     INT_enableGlobal( 1 );
*
*     while( 1 )
*     {
*     }
*
* LINKS
* file:///C:/tidcs/c28/CSL_C2803x/v100/doc/CSL_C2803x.pdf
* http://focus.ti.com/lit/ug/spruge6a/spruge6a.pdf
*
* NOTES
* At power up all of the CLA to CPU message RAM is set to zero and CLA task 8 is
* pre-configured for use with CLA_memSet().
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_CLA_T0_PUB_H
#define _CSL_CLA_T0_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/


/********** FORWARD REFERENCES SECTION ****************************************/
typedef struct  CLA_3p3zData    CLA_3p3zData;
typedef struct  CLA_2p2zData    CLA_2p2zData;
typedef struct  CLA_Ctrl        CLA_Ctrl;

/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* ENUM          : CLA_Module
* DESCRIPTION   :
* This uses the same style as the rest of the CSL, referring to the CLA
* tasks 1..8 as CLA_MOD_1..8.
*
*******************************************************************************/
typedef enum CLA_Module
{
    CLA_MOD_1,
    CLA_MOD_2,
    CLA_MOD_3,
    CLA_MOD_4,
    CLA_MOD_5,
    CLA_MOD_6,
    CLA_MOD_7,
    CLA_MOD_8
} CLA_Module;

/*******************************************************************************
* ENUM          : CLA_IntMode
* DESCRIPTION   :
* This describes the available triggers for the CLA task.
*
* The PWM and ADC modules used correspond to the CLA module number. e.g.
* CLA_MOD_1 can only be triggered with PWM_MOD_1 or ADC_MOD_1/
*
* The only exception to this is CLA_MOD_8 for which there is no equivalent
* PWM_MOD_8 module. The CPU Timer 0 interrupt is used in place of the PWM
* module however the CLA_IntMode should still be specified as CLA_INT_PWM.
*
*******************************************************************************/
typedef enum CLA_IntMode
{
    CLA_INT_ADC     = 0,    /* The matching ADC interrupt triggers the CLA module */
    CLA_INT_PWM     = 2,    /* The matching PWM interrupt triggers the CLA module */
    CLA_INT_NONE    = 1     /* There is no interrupt source for the CLA module */
} CLA_IntMode;

/*******************************************************************************
* STRUCT        : CLA_3p3zData
* DESCRIPTION   :
* This structure is used by the 3p3z controllers for internal values.
* This structure is only readable by the CPU.
*******************************************************************************/
struct CLA_3p3zData
{
    float   m_PreValue;     /* +0 */
    float   m_U[3];         /* +2 +4 +6 */
    float   m_E[3];         /* +8 +10 +12 ram */
};

/*******************************************************************************
* STRUCT        : CLA_2p2zData
* DESCRIPTION   :
* This structure is used by the 2p2z controllers for internal values.
* This structure is only readable by the CPU.
*******************************************************************************/
struct CLA_2p2zData
{
    float   m_PreValue;     /* +0 */
    float   m_U[2];         /* +2 +4 */
    float   m_E[2];         /* +6 +8 ram */
};

/*******************************************************************************
* STRUCT        : CLA_Ctrl
* DESCRIPTION   :
* This structure is used by both controllers to set the reference and for soft
* start.
* This structure is readable and writeable by the CPU.
*******************************************************************************/
struct CLA_Ctrl
{
    long    m_Ref;  /* +0 */
    long    m_Delta;
    long    m_Max;
};


/********** PROTOTYPES SECTIONS ***********************************************/

/*******************************************************************************
* MACRO         : CLA_3p3zVMode
* INPUT         : void Name
* INPUT         : void Adc
*                 ADC module number.
* INPUT         : void Pwm
*                 PWM module number.
* INPUT         : void A1
* INPUT         : void A2
* INPUT         : void A3
* INPUT         : void B0
* INPUT         : void B1
* INPUT         : void B2
* INPUT         : void B3
* INPUT         : void K
* INPUT         : void MiN
*                 Minimum number of ticks that the duty can be set to.
* INPUT         : void MaX
*                 Maximum number of ticks that the duty can be set to.
* RETURNS       : void
* DESCRIPTION   :
* This macro must be called at the top of the C file, before the main
* function begins.
*
* The values passed to the function call must be literals. Constants,
* variables or macros cannot be used.
*
* The function creates the CLA code for a 3p3z controller.
*
* EXAMPLES
* Creates the CLA function called ClaTask. This reads the ADC value from
* ADC_MOD_7 and writes the duty to PWM_MOD_3.
*
*   CLA_3p3zVMode( ClaTask, 7, 3,
*             +1.46818, -0.314933, -0.153248,
*              1.784224053, -1.629063952, -1.780916725, 1.632371281,
*              0.48, 0, 240 );
*
*******************************************************************************/
#define CLA_3p3zVMode( Name, Adc, Pwm, A1, A2, A3, B0, B1, B2, B3, K, MiN, MaX ) \
extern Uint32 Name; \
extern CLA_Ctrl     Name##Ctrl; \
extern CLA_3p3zData Name##Data; \
asm (\
"\n\t.global _Comp2Regs"\
"\n\t.global _AdcResult"\
"\n\t.global _EPwm"#Pwm"Regs"\
"\n\t.global _"#Name"Ctrl"\
"\n\t.global _"#Name"Data"\
"\n\t.global _"#Name""\
"\n\t.align  2"\
"\n\t"\
"\n_"#Name"Ctrl .usect \"CpuToCla1MsgRAM\", 6" \
"\n_"#Name"Data .usect \"Cla1ToCpuMsgRAM\", 14" \
"\n\t"\
"\n\t .sect Cla1Prog"\
"\n_"#Name":"\
"\n\t    ;MMOVXI     MR3, #257"\
"\n\t    ;MMOV16     @_Comp2Regs+0, MR3"\
"\n\t"\
"\n\t    MI16TOF32  MR3, @_AdcResult+"#Adc"-1 ; MR3 = Adc"\
"\n\t"\
"\n\t    MI16TOF32  MR2, @_"#Name"Ctrl+1  ; MR2 = msb of Ref"\
"\n\t    MSUBF32    MR0, MR2, MR3         ;E0(MR0) = Ref-Adc"\
"\n\t    MMOVF32    MR2, #"#B0"           ;MR2 = B0"\
"\n\t    MMPYF32    MR1, MR0, MR2         ;MR1 = E0*B0"\
"\n\t    MMOV32     MR3, @_"#Name"Data+0  ;MR3 = PreValue"\
"\n\t"\
"\n\t    MADDF32    MR2, MR3, MR1         ;MR2 = Prevalue + E0*B0"\
"\n\t"\
"\n\t    MMOV32     @_"#Name"Data+2+(0*2),MR2 ;U0 = MR2"\
"\n\t"\
"\n\t    MMOVF32    MR1, #"#K"            ;K = 5.0"\
"\n\t    MMPYF32    MR2, MR2, MR1"\
"\n\t    MMINF32    MR2,#"#MaX"          ;MR2 = min(490, MR2)"\
"\n\t    MMAXF32    MR2,#"#MiN"          ;MR2 = max(10, MR2)"\
"\n\t    MF32TOUI16 MR2,MR2               ;MR2 = int(MR2)"\
"\n\t    MMOV16     @_EPwm"#Pwm"Regs+9, MR2    ;Duty = MR2 .CMPA.half.CMPA"\
"\n\t"\
"\n\t    ;MMOVXI     MR3, #261                  ;MR2 = B0"\
"\n\t    ;MMOV16     @_Comp2Regs+0, MR3"\
"\n\t    ;MEALLOW"\
"\n\t"\
"\n\t    MMOV32     @_"#Name"Data+8+(0*2), MR0 ;E0 = MR0"\
"\n\t"\
"\n\t    MMOVF32    MR3,#0.0L"\
"\n\t    MMOVF32    MR2,#0.0L"\
"\n\t"\
"\n\t    MMOVF32    MR1, #"#A3"                ; MR2 = A3"\
"\n\t    MMOV32     MR0, @_"#Name"Data+2+(2*2) ; MR0 = U[2]"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = A3*U[2]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+2+(1*2) ; MR0 = U[1]"\
"\n\t    MMOVF32    MR1, #"#A2"                ; MR1 = A2"\
"\n\t    MMOV32     @_"#Name"Data+2+(2*2), MR0 ; U[2]= U[1]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = A2*U[1]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+2+(0*2) ; MR0 = U[0]"\
"\n\t    MMOVF32    MR1, #"#A1"                ; MR1 = A1"\
"\n\t    MMOV32     @_"#Name"Data+2+(1*2), MR0 ; U[1]= U[0]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = A1*U[0]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+8+(2*2) ; MR0 = E[2]"\
"\n\t    MMOVF32    MR1, #"#B3"                ; MR1 = B3"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = B3*E[2]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+8+(1*2) ; MR0 = E[1]"\
"\n\t    MMOVF32    MR1, #"#B2"                ; MR1 = B2"\
"\n\t    MMOV32     @_"#Name"Data+8+(2*2), MR0 ; E[2]= E[1]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = B2*E[1]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+8+(0*2) ; MR0 = E[0]"\
"\n\t    MMOVF32    MR1, #"#B1"                ; MR1 = B1"\
"\n\t    MMOV32     @_"#Name"Data+8+(1*2), MR0 ; E[1]= E[0]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = B1*E[0]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+8+0     ; dummy"\
"\n\t"\
"\n\t    MADDF32    MR3, MR3, MR2              ; MR3 += MR2"\
"\n\t    MMOV32     @_"#Name"Data+0, MR3       ; _PreValue = MR3"\
"\n\t"\
"\n\t"\
"\n\t    MSTOP"\
"\n\t    MNOP"\
"\n\t    MNOP"\
"\n\t    MNOP"\
);

/*******************************************************************************
* MACRO         : CLA_getCtrlPtr
* INPUT         : void Mod
*                 Selects the CLA module.
* RETURNS       : void
* DESCRIPTION   :
* Returns a pointer to the CLA module controller structure that holds the
* reference value.
*
*******************************************************************************/
#define CLA_getCtrlPtr( Mod ) (&(Mod##Ctrl))

/*******************************************************************************
* MACRO         : CLA_2p2zVMode
* INPUT         : void Name
* INPUT         : void Adc
* INPUT         : void Pwm
* INPUT         : void A1
* INPUT         : void A2
* INPUT         : void B0
* INPUT         : void B1
* INPUT         : void B2
* INPUT         : void K
* INPUT         : void MiN
*                 Minimum number of ticks that the duty can be set to.
* INPUT         : void MaX
*                 Maximum number of ticks that the duty can be set to.
* RETURNS       : void
* DESCRIPTION   :
* This macro must be called at the top of the C file, before the main
* function begins.
*
* The values passed to the function call must be literals. Constants,
* variables or macros cannot be used.
*
* The function creates the CLA code for a 2p2z controller.
*
* EXAMPLES
* Creates the CLA function called ClaTask. This reads the ADC value from
* ADC_MOD_7 and writes the duty to PWM_MOD_3.
*
*   CLA_2p2zVMode( ClaTask, 7, 3,
*             +1.46818, -0.314933,
*              1.784224053, -1.629063952, -1.780916725
*              0.48, 0, 240 );
*
*******************************************************************************/
#define CLA_2p2zVMode( Name, Adc, Pwm, A1, A2, B0, B1, B2, K, MiN, MaX ) \
extern Uint32 Name; \
extern CLA_Ctrl     Name##Ctrl; \
extern CLA_2p2zData Name##Data; \
asm (\
"\n\t.global _Comp2Regs"\
"\n\t.global _AdcResult"\
"\n\t.global _EPwm"#Pwm"Regs"\
"\n\t.global _"#Name"Ctrl"\
"\n\t.global _"#Name"Data"\
"\n\t.global _"#Name""\
"\n\t.align  2"\
"\n\t"\
"\n_"#Name"Ctrl .usect \"CpuToCla1MsgRAM\", 6" \
"\n_"#Name"Data .usect \"Cla1ToCpuMsgRAM\", 10" \
"\n\t"\
"\n\t .sect Cla1Prog"\
"\n_"#Name":"\
"\n\t    ;MMOVXI     MR3, #257"\
"\n\t    ;MMOV16     @_Comp2Regs+0, MR3"\
"\n\t"\
"\n\t    MI16TOF32  MR3, @_AdcResult+"#Adc"-1 ; MR3 = Adc"\
"\n\t"\
"\n\t    MI16TOF32  MR2, @_"#Name"Ctrl+1  ; MR2 = msb of Ref"\
"\n\t    MSUBF32    MR0, MR2, MR3         ;E0(MR0) = Ref-Adc"\
"\n\t    MMOVF32    MR2, #"#B0"           ;MR2 = B0"\
"\n\t    MMPYF32    MR1, MR0, MR2         ;MR1 = E0*B0"\
"\n\t    MMOV32     MR3, @_"#Name"Data+0  ;MR3 = PreValue"\
"\n\t"\
"\n\t    MADDF32    MR2, MR3, MR1         ;MR2 = Prevalue + E0*B0"\
"\n\t"\
"\n\t    MMOV32     @_"#Name"Data+2+(0*2),MR2 ;U0 = MR2"\
"\n\t"\
"\n\t    MMOVF32    MR1, #"#K"            ;K = 5.0"\
"\n\t    MMPYF32    MR2, MR2, MR1"\
"\n\t    MMINF32    MR2,#"#MaX"          ;MR2 = min(490, MR2)"\
"\n\t    MMAXF32    MR2,#"#MiN"          ;MR2 = max(10, MR2)"\
"\n\t    MF32TOUI16 MR2,MR2               ;MR2 = int(MR2)"\
"\n\t    MMOV16     @_EPwm"#Pwm"Regs+9, MR2    ;Duty = MR2 .CMPA.half.CMPA"\
"\n\t"\
"\n\t    MMOV32     @_"#Name"Data+6+(0*2), MR0 ;E0 = MR0"\
"\n\t"\
"\n\t    MMOVF32    MR3,#0.0L"\
"\n\t    MMOVF32    MR2,#0.0L"\
"\n\t"\
"\n\t    MMOVF32    MR1, #"#A2"                ; MR2 = A2"\
"\n\t    MMOV32     MR0, @_"#Name"Data+2+(1*2) ; MR0 = U[1]"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = A2*U[1]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+2+(0*2) ; MR0 = U[0]"\
"\n\t    MMOVF32    MR1, #"#A1"                ; MR1 = A1"\
"\n\t    MMOV32     @_"#Name"Data+2+(1*2), MR0 ; U[1]= U[0]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = A1*U[0]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+6+(1*2) ; MR0 = E[1]"\
"\n\t    MMOVF32    MR1, #"#B2"                ; MR1 = B2"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = B2*E[1]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+6+(0*2) ; MR0 = E[0]"\
"\n\t    MMOVF32    MR1, #"#B1"                ; MR1 = B1"\
"\n\t    MMOV32     @_"#Name"Data+6+(1*2), MR0; E[1]= E[0]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = B1*E[0]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+6+0     ; dummy"\
"\n\t"\
"\n\t    MADDF32    MR3, MR3, MR2              ; MR3 += MR2"\
"\n\t    MMOV32     @_"#Name"Data+0, MR3       ; _PreValue = MR3"\
"\n\t"\
"\n\t    ;MEALLOW"\
"\n\t    ;MMOVXI     MR3, #261                  ;MR2 = B0"\
"\n\t    ;MMOV16     @_Comp2Regs+0, MR3"\
"\n\t"\
"\n\t"\
"\n\t    MSTOP"\
"\n\t    MNOP"\
"\n\t    MNOP"\
"\n\t    MNOP"\
);

/*******************************************************************************
* MACRO         : CLA_slopeCode
* INPUT         : void Name
*                 Name of CLA code.
* INPUT         : int Comp
*                 CMP_MOD number 1..3
* INPUT         : int Pwm
*                 PWM_MOD number 1..6
* INPUT         : float Delta
*                 The delta added to the CMP_MOD DAC value
* INPUT         : void Steps
*                 The number of time Delta is added to the DAC value.
* RETURNS       : void
* DESCRIPTION   :
* This macro must be called at the top of the C file, before the main
* function begins.
*
* The values passed to the function call must be literals. Constants,
* variables or macros cannot be used.
*
* This function creates the CLA code to adjust the CMP_MOD DAC value.
*
* The current DAC value is adjusted by Delta every 50ns. This means that the new
* DAC value must be set before the CLA slope code is executed.
*
* The DAC is adjusted after 364ns from the PWM interrupt.
* The DAC value must be valid before 280ns after the interrupt value where it is
* read by the CLA code.
* The CLA code also clears the PWM interrupt.
*
* Each instruction takes 16.666ns to execute assuming a 60MHz system clock.
* Three instructions are used to decrement the DAC register by the value Delta.
* Therefore each decrement by Delta will occur at fixed intervals of 50ns
* (16.666ns*3).
*
* The number of decrements that occur during each execution of the CLA task is
* determined by the argument Steps. The CLA task begins executing 280ns after
* the interrupt for the PWM module specified occurs.
*
* Therefore the DAC value must be valid no greater than 280ns after the
* interrupt as it is read in to the CLA task code. Similarly, the CLA code must
* finish executing before the new DAC value is set by the control function.
* Otherwise the DAC value will be overwritten by the CLA slope task value.
*
* The designer must ensure that the CLA_slopeCode function finishes before the
* new DAC value is written and that the Delta value is not too large such that
* the DAC value wraps around from zero by the end of the number of steps.
*
*             16.666ns*3*Steps
* 364ns   _______________________
* _______|                      |__________
*
*
* -------
*        ----
*            ----  16.666ns*3
*  DAC           ----
*  value             ----
*                        ---
*                           ----------------
*
* EXAMPLES
* This creates the CLA function called SlopeTask. When PWM_MOD_3 generates an
* interrupt the CLA code is run where it decrements the CMP_MOD_1 DAC value by
* 1 every 50ns for 6 cycles.
*
*  CLA_slopeCode( SlopeTask, 1,3, -1.0, 6 );
*
*  //in the main code
*  //set up the comp and set the DAC value
*  CMP_config( CMP_MOD_1, CMP_SAMPLE_1, GPIO_NON_INVERT, CMP_DAC );
*  CMP_pin( CMP_MOD_1);
*  CMP_setDac( CMP_MOD_1, 100 );
*
*  //configure the CLA  to run after a PWM interrupt occurs
*  CLA_config( CLA_MOD_3, &SlopeTask, CLA_INT_PWM );
*
*  //configure the PWM
*  PWM_config( PWM_MOD_3, PWM_freqToTicks(200000), PWM_COUNT_DOWN );
*  PWM_pin( PWM_MOD_3, PWM_CH_A, GPIO_NON_INVERT );
*  PWM_setCallback(PWM_MOD_3, 0, PWM_INT_ZERO, PWM_INT_PRD_1 );
*
*******************************************************************************/
#define CLA_slopeCode( Name, Comp, Pwm, Delta, Steps ) \
extern Uint32 Name; \
asm (\
"\n\t.global _EPwm"#Pwm"Regs"\
"\n\t.global _Comp"#Comp"Regs"\
"\n\t.global _Comp2Regs"\
"\n\t.global _"#Name""\
"\n\t.eval   0, x"\
"\n\t.align  2"\
"\n\t"\
"\n\t .sect Cla1Prog"\
"\n_"#Name":"\
"\n\t    ;MDEBUGSTOP"\
"\n\t    ; used for timing on comp3 GPIO3 - set "\
"\n\t    ;MMOVXI     MR3, #257"\
"\n\t    ;MMOV16     @_Comp2Regs+0, MR3"\
"\n\t    ; clear the PWM INT flag"\
"\n\t    MMOVXI     MR3, #1"\
"\n\t    MMOV16     @_EPwm"#Pwm"Regs+28, MR3 ;clear PWM INT"\
"\n\t    ; load the current DAC value"\
"\n\t    MI16TOF32  MR3, @_Comp"#Comp"Regs+6 ; MR3 = Dac value"\
"\n\t    MMOVF32    MR2, #"#Delta"               ;MR2 = delata"\
"\n\t    ; Subtract the delta value from the dac and write it back"\
"\n\t    .loop"\
"\n\t    MADDF32    MR3, MR3, MR2"\
"\n\t    MF32TOUI16 MR1,MR3                     ;MR2 = int(Dac value)"\
"\n\t    MMOV16     @_Comp"#Comp"Regs+6, MR1    ;set Dac value"\
"\n\t    .eval x+1, x"\
"\n\t    .break x = "#Steps""\
"\n\t    .endloop"\
"\n\t    ; used for timing on comp3 GPIO3 - clr "\
"\n\t    ;MMOVXI     MR3, #261                  ;MR2 = B0"\
"\n\t    ;MMOV16     @_Comp2Regs+0, MR3"\
"\n\t    ;MEALLOW"\
"\n\t    MSTOP"\
"\n\t    MNOP"\
"\n\t    MNOP"\
"\n\t    MNOP"\
)

/*******************************************************************************
* MACRO         : CLA_2p2zIMode
* INPUT         : void Name
* INPUT         : void Adc
* INPUT         : void Cmp
* INPUT         : void A1
* INPUT         : void A2
* INPUT         : void B0
* INPUT         : void B1
* INPUT         : void B2
* INPUT         : void K
* INPUT         : void MiN
*                 Minimum number of ticks that the duty can be set to.
* INPUT         : void MaX
*                 Maximum number of ticks that the duty can be set to.
* RETURNS       : void
* DESCRIPTION   :
* This macro must be called at the top of the C file, before the main
* function begins.
*
* The values passed to the function call must be literals. Constants,
* variables or macros cannot be used.
*
* The function creates the CLA code for a 2p2z current mode controller.
*
* EXAMPLES
* Creates the CLA function called ClaTask. This reads the ADC value from
* ADC_MOD_7 and writes the value to CMP_MOD_3.
*
*   CLA_2p2zIMode( ClaTask, 7, 3,
*             +1.46818, -0.314933,
*              1.784224053, -1.629063952, -1.780916725
*              0.48, 0, 240 );
*
*******************************************************************************/
#define CLA_2p2zIMode( Name, Adc, Cmp, A1, A2, B0, B1, B2, K, MiN, MaX ) \
extern Uint32 Name; \
extern CLA_Ctrl     Name##Ctrl; \
extern CLA_2p2zData Name##Data; \
asm (\
"\n\t.global _Comp2Regs"\
"\n\t.global _AdcResult"\
"\n\t.global _Comp"#Cmp"Regs"\
"\n\t.global _"#Name"Ctrl"\
"\n\t.global _"#Name"Data"\
"\n\t.global _"#Name""\
"\n\t.align  2"\
"\n\t"\
"\n_"#Name"Ctrl .usect \"CpuToCla1MsgRAM\", 6" \
"\n_"#Name"Data .usect \"Cla1ToCpuMsgRAM\", 10" \
"\n\t"\
"\n\t .sect Cla1Prog"\
"\n_"#Name":"\
"\n\t    ;MMOVXI     MR3, #257"\
"\n\t    ;MMOV16     @_Comp2Regs+0, MR3"\
"\n\t"\
"\n\t    MI16TOF32  MR3, @_AdcResult+"#Adc"-1 ; MR3 = Adc"\
"\n\t"\
"\n\t    MI16TOF32  MR2, @_"#Name"Ctrl+1  ; MR2 = msb of Ref"\
"\n\t    MSUBF32    MR0, MR2, MR3         ;E0(MR0) = Ref-Adc"\
"\n\t    MMOVF32    MR2, #"#B0"           ;MR2 = B0"\
"\n\t    MMPYF32    MR1, MR0, MR2         ;MR1 = E0*B0"\
"\n\t    MMOV32     MR3, @_"#Name"Data+0  ;MR3 = PreValue"\
"\n\t"\
"\n\t    MADDF32    MR2, MR3, MR1         ;MR2 = Prevalue + E0*B0"\
"\n\t"\
"\n\t    MMOV32     @_"#Name"Data+2+(0*2),MR2 ;U0 = MR2"\
"\n\t"\
"\n\t    MMOVF32    MR1, #"#K"            ;K = 5.0"\
"\n\t    MMPYF32    MR2, MR2, MR1"\
"\n\t    MMINF32    MR2,#"#MaX"          ;MR2 = min(490, MR2)"\
"\n\t    MMAXF32    MR2,#"#MiN"          ;MR2 = max(10, MR2)"\
"\n\t    MF32TOUI16 MR2,MR2               ;MR2 = int(MR2)"\
"\n\t    MMOV16     @_Comp"#Cmp"Regs+6, MR2    ;Duty = MR2 .CMPA.half.CMPA"\
"\n\t"\
"\n\t    MMOV32     @_"#Name"Data+6+(0*2), MR0 ;E0 = MR0"\
"\n\t"\
"\n\t    MMOVF32    MR3,#0.0L"\
"\n\t    MMOVF32    MR2,#0.0L"\
"\n\t"\
"\n\t    MMOVF32    MR1, #"#A2"                ; MR2 = A2"\
"\n\t    MMOV32     MR0, @_"#Name"Data+2+(1*2) ; MR0 = U[1]"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = A2*U[1]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+2+(0*2) ; MR0 = U[0]"\
"\n\t    MMOVF32    MR1, #"#A1"                ; MR1 = A1"\
"\n\t    MMOV32     @_"#Name"Data+2+(1*2), MR0 ; U[1]= U[0]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = A1*U[0]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+6+(1*2) ; MR0 = E[1]"\
"\n\t    MMOVF32    MR1, #"#B2"                ; MR1 = B2"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = B2*E[1]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+6+(0*2) ; MR0 = E[0]"\
"\n\t    MMOVF32    MR1, #"#B1"                ; MR1 = B1"\
"\n\t    MMOV32     @_"#Name"Data+6+(1*2), MR0; E[1]= E[0]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = B1*E[0]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+6+0     ; dummy"\
"\n\t"\
"\n\t    MADDF32    MR3, MR3, MR2              ; MR3 += MR2"\
"\n\t    MMOV32     @_"#Name"Data+0, MR3       ; _PreValue = MR3"\
"\n\t"\
"\n\t    ;MEALLOW"\
"\n\t    ;MMOVXI     MR3, #261                  ;MR2 = B0"\
"\n\t    ;MMOV16     @_Comp2Regs+0, MR3"\
"\n\t"\
"\n\t"\
"\n\t    MSTOP"\
"\n\t    MNOP"\
"\n\t    MNOP"\
"\n\t    MNOP"\
);

/*******************************************************************************
* MACRO         : CLA_3p3zIMode
* INPUT         : void Name
* INPUT         : void Adc
*                 ADC module number.
* INPUT         : void Cmp
*                 Comp module number.
* INPUT         : void A1
* INPUT         : void A2
* INPUT         : void A3
* INPUT         : void B0
* INPUT         : void B1
* INPUT         : void B2
* INPUT         : void B3
* INPUT         : void K
* INPUT         : void MiN
*                 Minimum number of ticks that the duty can be set to.
* INPUT         : void MaX
*                 Maximum number of ticks that the duty can be set to.
* RETURNS       : void
* DESCRIPTION   :
* This macro must be called at the top of the C file, before the main
* function begins.
*
* The values passed to the function call must be literals. Constants,
* variables or macros cannot be used.
*
* The function creates the CLA code for a 3p3z current mode controller.
*
* EXAMPLES
* Creates the CLA function called ClaTask. This reads the ADC value from
* ADC_MOD_7 and writes the duty to CMP_MOD_3.
*
*   CLA_3p3zIMode( ClaTask, 7, 3,
*             +1.46818, -0.314933, -0.153248,
*              1.784224053, -1.629063952, -1.780916725, 1.632371281,
*              0.48, 0, 240 );
*
*******************************************************************************/
#define CLA_3p3zIMode( Name, Adc, Cmp, A1, A2, A3, B0, B1, B2, B3, K, MiN, MaX ) \
extern Uint32 Name; \
extern CLA_Ctrl     Name##Ctrl; \
extern CLA_3p3zData Name##Data; \
asm (\
"\n\t.global _Comp2Regs"\
"\n\t.global _AdcResult"\
"\n\t.global _Comp"#Cmp"Regs"\
"\n\t.global _"#Name"Ctrl"\
"\n\t.global _"#Name"Data"\
"\n\t.global _"#Name""\
"\n\t.align  2"\
"\n\t"\
"\n_"#Name"Ctrl .usect \"CpuToCla1MsgRAM\", 6" \
"\n_"#Name"Data .usect \"Cla1ToCpuMsgRAM\", 14" \
"\n\t"\
"\n\t .sect Cla1Prog"\
"\n_"#Name":"\
"\n\t    ;MMOVXI     MR3, #257"\
"\n\t    ;MMOV16     @_Comp2Regs+0, MR3"\
"\n\t"\
"\n\t    MI16TOF32  MR3, @_AdcResult+"#Adc"-1 ; MR3 = Adc"\
"\n\t"\
"\n\t    MI16TOF32  MR2, @_"#Name"Ctrl+1  ; MR2 = msb of Ref"\
"\n\t    MSUBF32    MR0, MR2, MR3         ;E0(MR0) = Ref-Adc"\
"\n\t    MMOVF32    MR2, #"#B0"           ;MR2 = B0"\
"\n\t    MMPYF32    MR1, MR0, MR2         ;MR1 = E0*B0"\
"\n\t    MMOV32     MR3, @_"#Name"Data+0  ;MR3 = PreValue"\
"\n\t"\
"\n\t    MADDF32    MR2, MR3, MR1         ;MR2 = Prevalue + E0*B0"\
"\n\t"\
"\n\t    MMOV32     @_"#Name"Data+2+(0*2),MR2 ;U0 = MR2"\
"\n\t"\
"\n\t    MMOVF32    MR1, #"#K"            ;K = 5.0"\
"\n\t    MMPYF32    MR2, MR2, MR1"\
"\n\t    MMINF32    MR2,#"#MaX"          ;MR2 = min(490, MR2)"\
"\n\t    MMAXF32    MR2,#"#MiN"          ;MR2 = max(10, MR2)"\
"\n\t    MF32TOUI16 MR2,MR2               ;MR2 = int(MR2)"\
"\n\t    MMOV16     @_Comp"#Cmp"Regs+6, MR2    ;Duty = MR2 dac comp"\
"\n\t"\
"\n\t    ;MMOVXI     MR3, #261                  ;MR2 = B0"\
"\n\t    ;MMOV16     @_Comp2Regs+0, MR3"\
"\n\t    ;MEALLOW"\
"\n\t"\
"\n\t    MMOV32     @_"#Name"Data+8+(0*2), MR0 ;E0 = MR0"\
"\n\t"\
"\n\t    MMOVF32    MR3,#0.0L"\
"\n\t    MMOVF32    MR2,#0.0L"\
"\n\t"\
"\n\t    MMOVF32    MR1, #"#A3"                ; MR2 = A3"\
"\n\t    MMOV32     MR0, @_"#Name"Data+2+(2*2) ; MR0 = U[2]"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = A3*U[2]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+2+(1*2) ; MR0 = U[1]"\
"\n\t    MMOVF32    MR1, #"#A2"                ; MR1 = A2"\
"\n\t    MMOV32     @_"#Name"Data+2+(2*2), MR0 ; U[2]= U[1]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = A2*U[1]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+2+(0*2) ; MR0 = U[0]"\
"\n\t    MMOVF32    MR1, #"#A1"                ; MR1 = A1"\
"\n\t    MMOV32     @_"#Name"Data+2+(1*2), MR0 ; U[1]= U[0]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = A1*U[0]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+8+(2*2) ; MR0 = E[2]"\
"\n\t    MMOVF32    MR1, #"#B3"                ; MR1 = B3"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = B3*E[2]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+8+(1*2) ; MR0 = E[1]"\
"\n\t    MMOVF32    MR1, #"#B2"                ; MR1 = B2"\
"\n\t    MMOV32     @_"#Name"Data+8+(2*2), MR0 ; E[2]= E[1]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = B2*E[1]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+8+(0*2) ; MR0 = E[0]"\
"\n\t    MMOVF32    MR1, #"#B1"                ; MR1 = B1"\
"\n\t    MMOV32     @_"#Name"Data+8+(1*2), MR0 ; E[1]= E[0]"\
"\n\t"\
"\n\t"\
"\n\t    MMACF32    MR3, MR2, MR2, MR1, MR0    ; MR3 += MR2; MR2 = B1*E[0]"\
"\n\t  ||MMOV32     MR0, @_"#Name"Data+8+0     ; dummy"\
"\n\t"\
"\n\t    MADDF32    MR3, MR3, MR2              ; MR3 += MR2"\
"\n\t    MMOV32     @_"#Name"Data+0, MR3       ; _PreValue = MR3"\
"\n\t"\
"\n\t"\
"\n\t    MSTOP"\
"\n\t    MNOP"\
"\n\t    MNOP"\
"\n\t    MNOP"\
);

/* public methods */
extern volatile Uint16* CLA_getVectorPtr( CLA_Module Mod );
extern void CLA_setCallback( CLA_Module Mod, INT_IsrAddr Func );
extern void CLA_softwareStart( CLA_Module Mod );
extern bool CLA_isRunning( CLA_Module Mod );
extern void CLA_softwareStartWait( CLA_Module Mod );
extern void CLA_config( CLA_Module Mod, Uint32* pFunc, CLA_IntMode Mode );
extern INT_PieId CLA_getPieId( CLA_Module Mod );
extern void CLA_ackInt( CLA_Module Mod );
extern void CLA_softStartConfig( CLA_Ctrl* Ptr, uint32_t RampMs,
                                 uint32_t UpdatePeriodNs );
extern void CLA_softStartUpdate( CLA_Ctrl* Ptr );
extern void CLA_softStartDirection( CLA_Ctrl* Ptr, int PowerUp );
extern void CLA_setRef( CLA_Ctrl* Ptr, uint16_t Ref );
extern void CLA_memSet( void* pAddr, uint16_t Data, int Count );

/********** USER MIDDLE SECTION ***********************************************/


/********** END ***************************************************************/
#endif

