/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_i2c_t0_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Sets up the I2C module. The module is currently under development and has
* limited functionality.
*
* It can be configured as an 8-bit master only. The FIFOs are always used and
* interrupts are not yet supported.
*
* EXAMPLES
* Sets up the I2C module 1 and writes two bytes, 0x55 and 0x66, to the device at
* address Addr. Following this, five bytes are read back and stored in the array
* 'data'. The process is repeated.
*
*   I2C_config( I2C_MOD_1, 9, 10, 5 );
*
*   for(;;)
*   {
*      uint8_t data[5] = { 0x55, 0x66 };
*      I2C_write( I2C_MOD_1, Addr, 2, data );
*      I2C_read( I2C_MOD_1, Addr, 5, data );
*   }
*
* LINKS
* file:///C:/tidcs/c28/CSL_C280x/v100/doc/CSL_C280x.pdf
* http://focus.ti.com/lit/ug/spru721a/spru721a.pdf
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_I2C_T0_PUB_H
#define _CSL_I2C_T0_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/
/* Error Messages */



/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* ENUM          : I2C_Status
* DESCRIPTION   :
* These are the status values returned from the I2C module.
*******************************************************************************/
typedef enum I2C_Status
{
    I2C_STATUS_ERROR                = (int)0xFFFE,
    I2C_STATUS_ARB_LOST_ERROR       = 0x0001,
    I2C_STATUS_NACK_ERROR           = 0x0002,
    I2C_STATUS_TIME_OUT             = 0x0003,
    I2C_STATUS_BUS_BUSY_ERROR       = 0x1000,
    I2C_STATUS_STP_NOT_READY_ERROR  = 0x5555,
    I2C_STATUS_NO_FLAGS             = (int)0xAAAA,
    I2C_STATUS_SUCCESS              = 0x0000
} I2C_Status;

/*******************************************************************************
* COMPLEX       : I2C_MOD_1
* DESCRIPTION   :
* These values are used to specify the I2C module.
*******************************************************************************/
#define I2C_MOD_1 (&I2caRegs)

/*******************************************************************************
* COMPLEX       : I2C_Module
* DESCRIPTION   :
* This is used to map hardware register values to I2C Module.
*******************************************************************************/
typedef volatile struct I2C_REGS* I2C_Module;


/********** PROTOTYPES SECTIONS ***********************************************/

/* public methods */
extern void I2C_config( I2C_Module Mod, uint16_t IPSC, uint16_t ICCL,
                        uint16_t ICCH );
extern I2C_Status I2C_write( I2C_Module Mod, uint16_t Address, uint16_t Argc,
                             const uint8_t* Argi );
extern I2C_Status I2C_read( I2C_Module Mod, uint16_t Address, uint16_t Argc,
                            uint8_t* Argi );
extern I2C_Status I2C_writeAddr( I2C_Module Mod, uint16_t Address, uint16_t Argc );
extern I2C_Status I2C_writeData( I2C_Module Mod, uint16_t Argc,
                                 const uint8_t* Argi );
extern I2C_Status I2C_writeEnd( I2C_Module Mod );

/********** USER MIDDLE SECTION ***********************************************/


/********** END ***************************************************************/
#endif

