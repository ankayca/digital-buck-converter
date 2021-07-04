/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_cntrl_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Contains functions to execute a digital 3 pole 3 zero (3p3z) and 2 pole 2 zero
* (2p2z) algorithm for use in the control of switch mode power supplies (SMPS).
*
* The control structure must be declared and then initialized using
* CNTRL_3p3zInit()/CNTRL_2p2zInit before the control function is run.
*
* The control function has been optimized in Assembler for maximum speed. In
* standard C a 3p3z algorithm can take circa 170 cycles (1.7us based on a 10ns
* system clock).
*  CNTRL_3p3z()        71    .71us
* CNTRL_3p3zInline()  53    .53us
* CNTRL_2p2z()        64    .64us
* CNTRL_2p2zInline()  44    .44us
*
* The C wrapper contains a small time penalty when compared to pure assembly but
* it has the advantage that no knowledge of assembly is required.
*
* The values for the 3p3z algorithm must be determined through control theory
* analysis of the system. The poles and zeros in the analogue frequency domain
* can be converted to the digital domain using the tool provided on the Biricha
* Digital Power website <http://www.biricha.com/resources/converter.php?type=4>
*
* The arguments are passed as _iq26 numbers. The limits of these arguments are,
*
* Value                 Limit
* A0-A2, B0-B3         -32  < value < 31.999999985
* REF, MIN, MAX         0   < value < 1
*
* The argument REF is the value that is compared to the feedback value from the
* system under control. The user code reads the feedback value from the system
* and stores it within the structure during each cycle of the control loop. The
* CNTRL_3p3z() function is used to update the output value based on REF and this
* feedback value.
*
* EXAMPLES
* Initializes the 3p3z structure with the correct coefficients. It then sets the
* m_IQ feedback value to the IQ value FDBK. The output value is then updated by
* running the control algorithm. Note that it is also possible to set the
* feedback value as an integer using the m_Int property of the structure.
*
*   CNTRL_3p3zInit(&CNTL_3P3Z_1,           // Structure
*                  REF                     // Ref
*                  A0,A1,A2                // a0,a1,a2
*                  B0,B1,B2,B3             // b0,b1,b2,b3
*                  K,MIN,MAX               // K, min, max
*                  );
*
*   // Control
*   CNTL_3P3Z_1.Fdbk.m_IQ = _IQ15(FDBK);   // Set feedback value
*   CNTRL_3p3z(&CNTL_3P3Z_1 );             // Update
*
* LINKS
* file:///C:/tidcs/c28/CSL_C280x/v100/doc/CSL_C280x.pdf
*
* NOTES
* Due to a compiler bug in v5 you must enable "No DP Load optimizations" -md
* when you use #pragma DATA_ALIGN.
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_CNTRL_PUB_H
#define _CSL_CNTRL_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/
#include "IQmathLib.h"


/********** USER START SECTION ************************************************/
typedef long _iq31;

/********** FORWARD REFERENCES SECTION ****************************************/
typedef union   CNTRL_ARG               CNTRL_ARG;
typedef struct  CNTRL_3p3zData          CNTRL_3p3zData;
typedef struct  CNTRL_2p2zData          CNTRL_2p2zData;
typedef struct  CNTRL_3p3zDataFloat     CNTRL_3p3zDataFloat;

/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* UNION         : CNTRL_ARG
* DESCRIPTION   :
* Allows a int to be written directly in to a _iq varaible.
*******************************************************************************/
union CNTRL_ARG
{
    _iq15   m_IQ;
    int     m_Int;
};

/*******************************************************************************
* STRUCT        : CNTRL_3p3zData
* DESCRIPTION   :
* This is the 3 pole 3 zero control structure.
*******************************************************************************/
struct CNTRL_3p3zData
{
    CNTRL_ARG   Ref;    /* +0 This is a range of +1 */
    CNTRL_ARG   Fdbk;   /* +2 This is a range of +1 */
    CNTRL_ARG   Out;    /* +4 This is a range of +1 */
    long        temp;   /* +6 */
    _iq24       m_U1;   /* +8 */
    _iq24       m_U2;   /* +10 */
    _iq24       m_U3;   /* +12 */
    _iq31       m_E0;   /* +14 */
    _iq31       m_E1;   /* +16 */
    _iq31       m_E2;   /* +18 */
    _iq31       m_E3;   /* +20 */
    _iq26       m_B3;   /* +22 */
    _iq26       m_B2;   /* +24 */
    _iq26       m_B1;   /* +26 */
    _iq26       m_B0;   /* +28 */
    _iq26       m_A3;   /* +30 */
    _iq26       m_A2;   /* +32 */
    _iq26       m_A1;   /* +34 */
    _iq23       m_K;    /* +36 */
    _iq15       m_max;  /* +38 */
    _iq15       m_min;  /* +40 */
    int         m_PeriodCount;
    long        m_SoftRamp;
    long        m_SoftRef;
    long        m_SoftMax;
};

/*******************************************************************************
* COMPLEX       : UNKNOWN_B23853E2
* DESCRIPTION   :
* Stores the registers used by the 3p2z/2p2z inline function.
*******************************************************************************/
#if 1
#define CNTRL_inlineContextSave() \
asm("        PUSH    XAR7"\
    "\t\n    PUSH    XT"\
    "\t\n    PUSH    ACC"\
    )
#endif

/*******************************************************************************
* COMPLEX       : UNKNOWN_AFF11ED4
* DESCRIPTION   :
* Restores the registers used by the 3p2z/2p2z inline function.
*******************************************************************************/
#if 1
#define CNTRL_inlineContextRestore() \
asm("        POP    ACC"\
    "\t\n    POP    XT"\
    "\t\n    POP    XAR7"\
    )
#endif

/*******************************************************************************
* COMPLEX       : CNTRL_3p3zInline
* DESCRIPTION   :
* Performs the 3 pole 3 zero (3p3z) control algorithm using the information
* stored within the 2p2z control structure that is passed as a structure to
* this function.
*
* The structure should already have been declared and populated with
* coefficients using the function CNTRL_3p3zInit().
*
* The feedback value from the system being controlled must be read and stored
* within the 3p3z structure before this function is called.
*
* The result of the control algorithm is also stored within the structure
* in the Out.m_Int property.
*
* This function is inline assembly code and it is the responsability of the user
* to make sure the "C" context is saved between calls.
*
* There are CNTRL_inlineContextSave() and CNTRL_inlineContextRestore() which
* saves/restores the required context.
*
* EXAMPLES
* Reads the feedback value from the ADC, which will be >=0.0 and < 1.0 and
* calls the 3p3z control algorithm. The ePWM module 1 duty for channel A is
* updated using the output of the control algorithm.
*
*    // Control
*    CNTL_3P3Z_1.Fdbk.m_Int = ADC_getValue(ADC_MOD_1,3);  // Read feedback
*    CNTRL_inlineContextSave();
*    CNTRL_3p3zInline(CNTL_3P3Z_1 );                      // Run algorithm
*    CNTRL_inlineContextRestore();
*    PWM_setDutyA(PWM_MOD_1, CNTL_3P3Z_1.Out.m_Int );     // Set new output
*
*******************************************************************************/
#if 1
#define CNTRL_3p3zInline(x) \
asm("        MOVW    DP, #_"#x"+0        ;CNTRL_3p3z"\
    "\t\n    MOVL    XAR7,#_"#x"+22      ;(COEFF) Local coefficient pointer (XAR7)"\
\
    "\t\n    SETC    SXM,OVM"\
    "\t\n    MOV     ACC,@0              ;(Ref)Q15"\
    "\t\n    SUB     ACC,@2              ;(Fdbk)Q15"\
    "\t\n    LSL     ACC,#16             ;Q31"\
\
    "\t\n    ; Diff equation"\
    "\t\n    MOVL    @8+6,ACC            ;(DBUFF+6)"\
    "\t\n    MOVL    XT,@8+12            ;(DBUFF+12) XT=e(n-3),Q31"\
    "\t\n    QMPYL   ACC,XT,*XAR7++      ; b3*e(n-3),Q26*Q31(64-bit result)"\
\
    "\t\n    MOVDL   XT,@8+10            ;(DBUFF+10) XT=e(n-2), e(n-3)=e(n-2)"\
    "\t\n    QMPYL   P,XT,*XAR7++        ; ACC=b3*e(n-3)+b2*e(n-2) P=b1*e(n-1),Q26*Q31(64-bit result)"\
    "\t\n    ADDL    ACC,P               ; 64-bit result in Q57, So ACC is in Q25"\
\
    "\t\n    MOVDL   XT,@8+8             ;(DBUFF+10) XT=e(n-1), e(n-2)=e(n-1)"\
    "\t\n    QMPYL   P,XT,*XAR7++        ; ACC=b2*e(n-2) P=b1*e(n-1),Q26*Q31(64-bit result)"\
    "\t\n    ADDL    ACC,P               ; 64-bit result in Q57, So ACC is in Q25"\
\
    "\t\n    MOVDL   XT,@8+6             ;(DBUFF+6) XT=e(n), e(n-1)=e(n)"\
    "\t\n    QMPYL   P,XT,*XAR7++        ; ACC=b3*e(n-3)+b2*e(n-2)+b1*e(n-1), P=b0*e(n),Q26*Q31(64-bit result)"\
    "\t\n                                ; 64-bit result in Q57, So ACC is in Q25"\
    "\t\n    ADDL    ACC,P               ; ACC=b3*e(n-3)+b2*e(n-2)+b1*e(n-1)+b0*e(n), Q25"\
    "\t\n    SFR     ACC,#1"\
    "\t\n    MOVL    @6,ACC              ;(temp) Q24"\
\
    "\t\n    MOVL    XT,@8+4             ;(DBUFF+4) XT=u(n-3),Q24"\
    "\t\n    QMPYL   P,XT,*XAR7++        ; P=a3*u(n-3), Q26*Q24(64-bit result)"\
\
    "\t\n    MOVDL   XT,@8+2             ;(DBUFF+2) XT=u(n-2), u(n-3)=u(n-2),Q24"\
    "\t\n    QMPYL   ACC,XT,*XAR7++      ; ACC=a2*u(n-2)"\
    "\t\n                                ; 64-bit result in Q50, So ACC is in Q18"\
    "\t\n    ADDL    ACC,P               ; ACC=a1*u(n-1)+a2*u(n-2)+a3*u(n-3),ACC in Q18"\
\
    "\t\n    MOVDL   XT,@8+0             ;(DBUFF+0) XT=u(n-1), u(n-2)=u(n-1),Q24"\
    "\t\n    QMPYL   P,XT,*XAR7++        ; P=a2*u(n-2)"\
    "\t\n                                ; 64-bit result in Q50, So ACC is in Q18"\
    "\t\n    ADDL    ACC,P               ; ACC=a1*u(n-1)+a2*u(n-2)+a3*u(n-3),ACC in Q18"\
\
    "\t\n    LSL     ACC,#5              ; Q23"\
    "\t\n    ADDL    ACC,ACC             ; Q24"\
    "\t\n    ADDL    ACC,@6              ;(temp) Q24,ACC=a1*u(n-1)+a2*u(n-2)+b2*e(n-2)+b1*e(n-1)+b0*e(n)"\
    "\t\n    MOVL    @8+0,ACC            ; (DBUFF+0) ACC=u(n)(Q24)"\
\
    "\t\n    MOVL    XT,ACC              ; XT = ACC iq24"\
    "\t\n    QMPYL   ACC,XT,*XAR7++      ; ACC = XT * K(23) >> 32 => iq15"\
\
    "\t\n    MINL    ACC,*XAR7++         ; Saturate the result [0,1]"\
    "\t\n    MAXL    ACC,*XAR7++"\
\
    "\t\n    MOV     @4, AL ;(Out)")

/*end of code macro*/
#endif

/*******************************************************************************
* STRUCT        : CNTRL_2p2zData
* DESCRIPTION   :
* This is the 2 pole 2 zero control structure.
*******************************************************************************/
struct CNTRL_2p2zData
{
    CNTRL_ARG   Ref;    /* +0 This is a range of +1 */
    CNTRL_ARG   Fdbk;   /* +2 This is a range of +1 */
    CNTRL_ARG   Out;    /* +4 This is a range of +1 */
    long        temp;   /* +6 */
    _iq24       m_U1;   /* +8 */
    _iq24       m_U2;   /* +10 */
    _iq31       m_E0;   /* +12 */
    _iq31       m_E1;   /* +14 */
    _iq31       m_E2;   /* +16 */
    _iq26       m_B2;   /* +18 */
    _iq26       m_B1;   /* +20 */
    _iq26       m_B0;   /* +22 */
    _iq26       m_A2;   /* +24 */
    _iq26       m_A1;   /* +26 */
    _iq23       m_K;    /* +28 */
    _iq15       m_max;  /* +30 */
    _iq15       m_min;  /* +32 */
    int         m_PeriodCount;
    long        m_SoftRamp;
    long        m_SoftRef;
    long        m_SoftMax;
};

/*******************************************************************************
* COMPLEX       : CNTRL_2p2zInline
* DESCRIPTION   :
* Performs the 2 pole 2 zero (2p2z) control algorithm using the information
* stored within the 2p2z control structure that is passed as a structure to
* this function.
*
* The structure should already have been declared and populated with
* coefficients using the function CNTRL_2p2zInit().
*
* The feedback value from the system being controlled must be read and stored
* within the 2p2z structure before this function is called.
*
* The result of the control algorithm is also stored within the structure
* in the Out.m_Int property.
*
* This function is inline assembly code and it is the responsability of the user
* to make sure the "C" context is saved between calls.
*
* There are CNTRL_inlineContextSave() and CNTRL_inlineContextRestore() which
* saves/restores the required context.
*
* EXAMPLES
* Reads the feedback value from the ADC, which will be >=0.0 and < 1.0 and
* calls the 2p2z control algorithm. The ePWM module 1 duty for channel A is
* updated using the output of the control algorithm.
*
*    // Control
*    CNTL_2P2Z_1.Fdbk.m_Int = ADC_getValue(ADC_MOD_1,3);  // Read feedback
*    CNTRL_inlineContextSave();
*    CNTRL_2p2zInline(CNTL_2P2Z_1 );                      // Run algorithm
*    CNTRL_inlineContextRestore();
*    PWM_setDutyA(PWM_MOD_1, CNTL_2P2Z_1.Out.m_Int );     // Set new output
*
*******************************************************************************/
#if 1
#define CNTRL_2p2zInline(x) \
asm("        MOVW    DP, #_"#x"+0        ;CNTRL_2p2z"\
    "\t\n    MOVL    XAR7,#_"#x"+18      ;(COEFF) Local coefficient pointer (XAR7)"\
\
    "\t\n    SETC    SXM,OVM"\
    "\t\n    MOV     ACC,@0              ;(Ref)Q15"\
    "\t\n    SUB     ACC,@2              ;(Fdbk)Q15"\
    "\t\n    LSL     ACC,#16             ;Q31"\
\
    "\t\n    ; Diff equation"\
    "\t\n    MOVL    @8+4,ACC            ;(DBUFF+4)"\
    "\t\n    MOVL    XT,@8+8             ;(DBUFF+8) XT=e(n-2),Q31"\
    "\t\n    QMPYL   ACC,XT,*XAR7++      ; b2*e(n-2),Q26*Q31(64-bit result)"\
\
    "\t\n    MOVDL   XT,@8+6             ;(DBUFF+6) XT=e(n-1), e(n-2)=e(n-1)"\
    "\t\n    QMPYL   P,XT,*XAR7++        ; ACC=b3*e(n-3)+b2*e(n-2) P=b1*e(n-1),Q26*Q31(64-bit result)"\
    "\t\n    ADDL    ACC,P               ; 64-bit result in Q57, So ACC is in Q25"\
\
    "\t\n    MOVDL   XT,@8+4             ;(DBUFF+10) XT=e(n-0), e(n-1)=e(n-0)"\
    "\t\n    QMPYL   P,XT,*XAR7++        ; ACC=b2*e(n-2) P=b1*e(n-1),Q26*Q31(64-bit result)"\
\
    "\t\n    ADDL    ACC,P               ; ACC=b2*e(n-2)+b1*e(n-1)+b0*e(n-0), Q25"\
    "\t\n    SFR     ACC,#1"\
    "\t\n    MOVL    @6,ACC              ;(temp) Q24"\
\
    "\t\n    MOVL    XT,@8+2             ;(DBUFF+2) XT=u(n-2),Q24"\
    "\t\n    QMPYL   ACC,XT,*XAR7++      ; ACC=a2*u(n-2), Q26*Q24(64-bit result)"\
\
    "\t\n    MOVDL   XT,@8+0             ;(DBUFF+0) XT=u(n-1), u(n-2)=u(n-1),Q24"\
    "\t\n    QMPYL   P,XT,*XAR7++        ; P=a1*u(n-1)"\
    "\t\n                                ; 64-bit result in Q50, So ACC is in Q18"\
    "\t\n    ADDL    ACC,P               ; ACC=a1*u(n-1)+a2*u(n-2),ACC in Q18"\
\
    "\t\n    LSL     ACC,#5              ; Q23"\
    "\t\n    ADDL    ACC,ACC             ; Q24"\
    "\t\n    ADDL    ACC,@6              ;(temp) Q24,ACC=a1*u(n-1)+a2*u(n-2)+b2*e(n-2)+b1*e(n-1)+b0*e(n)"\
    "\t\n    MOVL    @8+0,ACC            ; (DBUFF+0) ACC=u(n)(Q24)"\
\
    "\t\n    MOVL    XT,ACC              ; XT = ACC iq24"\
    "\t\n    QMPYL   ACC,XT,*XAR7++      ; ACC = XT * K(23) >> 32 => iq15"\
\
    "\t\n    MINL    ACC,*XAR7++         ; Saturate the result [0,1]"\
    "\t\n    MAXL    ACC,*XAR7++"\
\
    "\t\n    MOV     @4, AL ;(Out)")

/*end of code macro*/
#endif

/*******************************************************************************
* STRUCT        : CNTRL_3p3zDataFloat
* DESCRIPTION   :
*******************************************************************************/
struct CNTRL_3p3zDataFloat
{
    uint16_t    m_Ref;
    uint16_t    m_Fdbk;
    float       m_A1;
    float       m_A2;
    float       m_A3;
    float       m_B0;
    float       m_B1;
    float       m_B2;
    float       m_B3;
    float       m_E[4];
    float       m_U[4];
    float       m_K;
    uint16_t    m_Out;
    uint16_t    m_Min;
    uint16_t    m_Max;
};


/********** PROTOTYPES SECTIONS ***********************************************/

/* public methods */
extern void CNTRL_3p3zInit( CNTRL_3p3zData* Ptr, _iq15 Ref, _iq26 A1, _iq26 A2,
                            _iq26 A3, _iq26 B0, _iq26 B1, _iq26 B2, _iq26 B3,
                            _iq23 K, _iq15 Min, _iq15 Max );
#ifndef HEADER_ONLY
extern void CNTRL_3p3z( CNTRL_3p3zData* Ptr );
#endif /* HEADER_ONLY */
extern void CNTRL_softStartConfig( CNTRL_3p3zData* Ptr, uint32_t RampMs,
                                   uint32_t UpdatePeriodNs );
extern void CNTRL_softStartUpdate( CNTRL_3p3zData* Ptr );
extern void CNTRL_softStartDirection( CNTRL_3p3zData* Ptr, int PowerUp );
extern void CNTRL_2p2zInit( CNTRL_2p2zData* Ptr, _iq15 Ref, _iq26 A1, _iq26 A2,
                            _iq26 B0, _iq26 B1, _iq26 B2, _iq23 K, _iq15 Min,
                            _iq15 Max );
#ifndef HEADER_ONLY
extern void CNTRL_2p2z( CNTRL_2p2zData* Ptr );
#endif /* HEADER_ONLY */
extern void CNTRL_2p2zSoftStartConfig( CNTRL_2p2zData* Ptr, uint32_t RampMs,
                                       uint32_t UpdatePeriodNs );
extern void CNTRL_2p2zSoftStartUpdate( CNTRL_2p2zData* Ptr );
extern void CNTRL_2p2zSoftStartDirection( CNTRL_2p2zData* Ptr, int PowerUp );
extern void CNTRL_3p3zFloatInit( CNTRL_3p3zDataFloat* Ptr, uint16_t Ref,
                                 float a1, float a2, float a3, float b0,
                                 float b1, float b2, float b3, float k,
                                 uint16_t Min, uint16_t Max );
extern void CNTRL_3p3zFloat( CNTRL_3p3zDataFloat* Ptr );

/********** USER MIDDLE SECTION ***********************************************/


/********** END ***************************************************************/
#endif

