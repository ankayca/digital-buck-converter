/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_err_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* STATIC        : ERR_Id ERR_Value
* DESCRIPTION COPY:
* Protected functions used for error handling.
*
* When an error happens within the csl code you can look at the value of
* ERR_Value to what went wrong.
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_ERR_PUB_H
#define _CSL_ERR_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/


/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* ENUM          : ERR_Id
* DESCRIPTION   :
* These are a list of error conditions provided by the CSL.
*******************************************************************************/
typedef enum ERR_Id
{
    ERR_ERR_OK,
    ERR_ADC_MOD_1_2_INVALID,            /* You can not use ADC_MOD_1/2 since you are using ADC_MOD_3 */
    ERR_ADC_MOD_3_INVALID,              /* You can not use ADC_MOD_3 since you are using ADC_MOD_1/2 */
    ERR_ADC_MOD_X_INVALID,              /* Invalid ADC_MOD_X */
    ERR_ADC_MOD_1_BUSY,                 /* ADC_MOD_1 is busy */
    ERR_ADC_MOD_2_BUSY,                 /* ADC_MOD_1 is busy */
    ERR_ADC_CONV_GR_8,                  /* Only 8 samples per ADC module in non-casade mode */
    ERR_ADC_CONV_GR_16,                 /* Only 16 samples per ADC module in non-casade mode */
    ERR_ADC_CHAN_INVALID,               /* The channelOnly 16 samples per ADC module in non-casade mode */
    ERR_GPIO_PIN_INVALID,               /* The GPIO pin is invalid */
    ERR_GPIO_NOT_ACQUIRED,              /* The GPIO pin is being used before it has been acquired */
    ERR_GPIO_ALREADY_ACQUIRED,          /* The GPIO pin is already in use */
    ERR_GPIO_LIMIT_REACHED,             /* The free version has acquire too many GPIO pins. Please buy the full version */
    ERR_I2C_MOD_X_INVALID,              /* Invalid I2C_MOD_x */
    ERR_INT_ISR_DEFAULT,                /* The default ISR has been called */
    ERR_INT_GROUP_INVALID,              /* The group is is not valid */
    ERR_PWM_ALREADY_ACQUIRED,           /* The PWM is already in use */
    ERR_PWM_COUNT_MODE_INVALID,         /* The PWM cound mode is invalid */
    ERR_PWM_NOT_ACQUIRED,               /* The PWM is being used before it has been acquired */
    ERR_SPI_MOD_X_INVALID,              /* Invalid SPI_MOD_X */
    ERR_SPI_TICK_INVALID,               /* An invalid value for the spi baud rate has been used */
    ERR_SYS_STACK_OVERFLOW,             /* A overflow in the stack has been detected */
    ERR_SYS_PERIPHERAL_INVALID,         /* The required Peripheral not supported on this device */
    ERR_UART_RX_PIN_INVALID,            /* the selected GPIO pin is not valid for RX */
    ERR_UART_TX_PIN_INVALID,            /* the selected GPIO pin is not valid for TX */
    ERR_UART_MOD_X_INVALID,             /* Invalid UART_MOD_X */
    ERR_WDG_ISR_DEFAULT,                /* The default ISR for the watch dog has been called */
    ERR_RESERVED_INTERNAL_1,            /* Reserved for internal errors */
    ERR_ADC_NOT_INIT,                   /* the ad isn't set up yet */
    ERR_HIRES_CABILRATION               /* something went wrong calibrating the HiRes PWM */
} ERR_Id;


/********** CLASS SECTION *****************************************************/

extern ERR_Id   ERR_Value;

/********** USER MIDDLE SECTION ***********************************************/


/********** END ***************************************************************/
#endif

