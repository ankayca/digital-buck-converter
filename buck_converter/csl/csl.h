/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION   :
* This is the main header file for the csl library. It pulls in all of the csl
* modules' header files and the DSP280x header files.
*
* Before you use any API from the csl library you must call SYS_int() which
* performs the initialisation.
*
* EXAMPLES
*
*   void main ( void )
*   {
*     SYS_init();
*
*     //your code
*   }
*
*
* LINKS
* file:C:\tidcs\c28\CSL_C280x\v100\doc\CSL_C280x.doc
*
*
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_H
#define _CSL_H

/********** INCLUDES GLOBAL SECTION *******************************************/
#if (defined CSL_C280X)
    #include "csl_c280x.h"
#elif (defined CSL_C2802X)
    #include "csl_c2802x.h"
#elif (defined CSL_C2833X)
    #include "csl_c2833x.h"
#elif (defined CSL_C2803X)
    #include "csl_c2803x.h"
#else
    #error Unknown device type
#endif

/********** USER START SECTION ************************************************/


/********** USER MIDDLE SECTION ***********************************************/


/********** END ***************************************************************/
#endif

