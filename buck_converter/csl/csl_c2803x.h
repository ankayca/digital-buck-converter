/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_c2803x.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION   :
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_C2803X_H
#define _CSL_C2803X_H

/********** INCLUDES GLOBAL SECTION *******************************************/
#ifdef WIN32
#define interrupt
#endif

#ifdef CSL_C2803X
#undef CSL_C2803X
#endif
#define CSL_C2803X

#include "DSP2803x_Device.h"     // Header file Include File
#include "DSP2803x_Examples.h"   // Examples Include File
#include "DSP2803x_SysCtrl.h"

#include "csl_c2000_Pub.h"

/********** USER START SECTION ************************************************/


/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* COMPLEX       : MOD_COUNT
* DESCRIPTION   :
* This can be use to determine the number of modules per peripheral type used by
* the csl.
*******************************************************************************/
#if 1
#define PWM_MOD_COUNT    7
#define UART_MOD_COUNT   1
#define SPI_MOD_COUNT    2
#define TIM_MOD_COUNT    3
#define I2C_MOD_COUNT    1
#define CMP_MOD_COUNT    3
#define CAP_MOD_COUNT    1
#endif

/*******************************************************************************
* COMPLEX       : UART_FIFO_DEPTH
* DESCRIPTION   :
* This is the size of the FIFO used by the module.
*******************************************************************************/
#define UART_FIFO_DEPTH (4)

/*******************************************************************************
* COMPLEX       : SPI_FIFO_DEPTH
* DESCRIPTION   :
* This is the size of the FIFO used by the module.
*******************************************************************************/
#define SPI_FIFO_DEPTH (4)


/********** USER MIDDLE SECTION ***********************************************/

#include "csl_sys_c2803x_Pub.h"
#include "csl_int_t0_Pub.h"
#include "csl_gpio_t0_Pub.h"

#include "csl_adc_t3_Pub.h"
#include "csl_cmp_t0_Pub.h"
#include "csl_cla_t0_Pub.h"
#include "csl_pwm_t1_Pub.h"

#include "csl_i2c_t0_Pub.h"
#include "csl_spi_t0_Pub.h"
#include "csl_tim_t0_Pub.h"
#include "csl_wdg_t0_Pub.h"
#include "csl_uart_t0_Pub.h"
#include "csl_cap_t0_Pub.h"

/********** END ***************************************************************/
#endif

