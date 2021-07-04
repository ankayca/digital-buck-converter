/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_uart_t0_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Contains functions for configuring the UART module of the DSP.
*
* The UART_config() function must be called before using any of the API
* functions.
*
* By default the module will always use the Rx and Tx FIFOs.
*
* EXAMPLES
* Configures and opens a serial port. The code then enters a loop which will
* echo back any received characters.
*
*   UART_config( UART_MOD_1, GPIO_28, GPIO_29,
*                UART_baudToTicks(115200),
*                UART_DATA_8, UART_PARITY_NONE, UART_STOP_2 );
*   UART_setLoopback( true );
*
*   for(;;)
*   {
*      // Wait for incoming character to be received
*      while(UART_getRxCount(UART_MOD_1)==0 );
*      UART_putc(UART_MOD_1, UART_getc(UART_MOD_1) );
*   }
*
*  LINKS
* file:///C:/tidcs/c28/CSL_C280x/v100/doc/CSL_C280x.pdf
* http://focus.ti.com/lit/ug/spru051b/spru051b.pdf
*
* HISTORY       :
* 2008/06/12 hossa1c Created.
*******************************************************************************/


#ifndef _CSL_UART_T0_PUB_H
#define _CSL_UART_T0_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/


/********** USER START SECTION ************************************************/
typedef struct TL_UART_MOD_SIZE
{
    uint16_t A[0x700];
}TL_UART_MOD_SIZE;

/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* COMPLEX       : UART_MOD_X
* DESCRIPTION   :
* These values are used to specify the UART module.
*******************************************************************************/
#if 1
#define UART_MOD_1 (&SciaRegs)
#define UART_MOD_2 (&ScibRegs)
#endif

/*******************************************************************************
* COMPLEX       : UART_Module
* DESCRIPTION   :
* This is used to map hardware register values to UART_Module.
*******************************************************************************/
typedef volatile struct SCI_REGS* UART_Module;

/*******************************************************************************
* ENUM          : UART_DataBits
* DESCRIPTION   :
* These are the values for the number of data bits sent.
*******************************************************************************/
typedef enum UART_DataBits
{
    UART_DATA_5     = 4,
    UART_DATA_6     = 5,
    UART_DATA_7     = 6,
    UART_DATA_8     = 7
} UART_DataBits;

/*******************************************************************************
* ENUM          : UART_Parity
* DESCRIPTION   :
* These are the values for the type of parity used.
*******************************************************************************/
typedef enum UART_Parity
{
    UART_PARITY_NONE    = 0,
    UART_PARITY_ODD     = 2,
    UART_PARITY_EVEN    = 3
} UART_Parity;

/*******************************************************************************
* ENUM          : UART_StopBits
* DESCRIPTION   :
* These are the values for the stops bits.
*******************************************************************************/
typedef enum UART_StopBits
{
    UART_STOP_1     = 0,
    UART_STOP_2     = 1
} UART_StopBits;


/********** PROTOTYPES SECTIONS ***********************************************/

/*******************************************************************************
* MACRO         : UART_baudToTicks
* INPUT         : int baud
*                 Selects the baud rate.
* RETURNS       : uint16_t
* DESCRIPTION   :
* Converts the required baud rate to the number of UART clock ticks using the
* following formula.
*
*                      SYS_CLK_HZ
* Ticks =  ------------------------------  - 1
*            USR_PER_LSP_DIV * baud x 8
*
* EXAMPLES
* Returns the UART ticks required for a baud rate of 115200.
*
*   uint16_t BaudTicks = UART_baudToTicks( 115200 );
*
* NOTES
* When USR_PER_LSP_DIV and baud are large the result Ticks can be small, which
* due to rounding errors can give the incorrect value
*
*  SYS_setPerhiperalClk(SYS_PER_CLK_DIV_2, SYS_PER_CLK_DIV_14);
*  6 = UART_baudToTicks( 115200 ); instead of 6.750
*
* For a 100Mhz system clock the value of 6 fails to work, while a value of 7
* works correctly.
* The code has therefore been changed to add 0.5 to the result before converting
* to an integer value.
*******************************************************************************/
#define UART_baudToTicks( baud ) ((((10L*SYS_CLK_HZ)/(SYS_LIT_VALUE(USR_PER_LSP_DIV)*8L*baud))-5)/10)

/*******************************************************************************
* MACRO         : UART_ackRxInt
* INPUT         : UART_Module Mod
*                 Selects the UART module.
* RETURNS       : void
* DESCRIPTION   :
* Used within an interrupt service routine to clear both the Rx interrupt flag
* and the PIE group acknowledgment flag.
*
* EXAMPLES
* Clears the Rx interrupt flag and the PIE group acknowledgment flag after the
* UART module 1 generates an Rx interrupt and the PIE controller calls the ISR.
* First the character is read from the Rx FIFO buffer and then the flags are
* cleared.
*
*   interrupt void isr_uart1_rx( void )
*   {
*      // Get character
*      ch = UART_getc( UART_MOD_1 );
*
*      // Clear flags
*      UART_ackRxInt( UART_MOD_1 );
*   }
*
*******************************************************************************/
#define UART_ackRxInt( Mod ) UART_clrRxInt(Mod); INT_ackGroup(INT_GROUP_9)

/*******************************************************************************
* MACRO         : UART_getIndex
* INPUT         : UART_Module Mod
*                 Selects the UART module.
* RETURNS       : int
* DESCRIPTION   :
* Returns an index value for the UART module.
*
* EXAMPLES
* Returns the index 0 for UART 1.
*
*   int index = UART_getIndex(UART_MOD_1);
*
* NOTES
*    UART_MOD_1    0
*    UART_MOD_2    1
*
*******************************************************************************/
#define UART_getIndex( Mod ) ((TL_UART_MOD_SIZE*)(Mod)-(TL_UART_MOD_SIZE*)UART_MOD_1)

/*******************************************************************************
* MACRO         : UART_getRxPieId
* INPUT         : UART_Module Mod
*                 Selects the UART module.
* RETURNS       : INT_PieId
* DESCRIPTION   :
* Returns the PIE Id for the Rx UART module.
*
* EXAMPLES
* Returns the PieId for UART 1.
*
*   INT_PieId id = UART_getRxPieId( UART_MOD_1 );
*
* NOTES
*    UART_MOD_1     INT_ID_SCIRXINTA
*    UART_MOD_2     INT_ID_SCIRXINTB
*
*******************************************************************************/
#define UART_getRxPieId( Mod ) ((INT_PieId)(UART_getIndex(Mod)*2+(int)INT_ID_SCIRXINTA))

/*******************************************************************************
* MACRO         : UART_getTxPieId
* INPUT         : UART_Module Mod
*                 Selects the UART module.
* RETURNS       : INT_PieId
* DESCRIPTION   :
* Returns the PIE Id for the Tx UART module.
*
* EXAMPLES
* Returns INT_ID_SCITXINTA for UART 1.
*
*   INT_PieId id = UART_getTxPieId( UART_MOD_1 );
*
* NOTES
*    UART_MOD_1     INT_ID_SCITXINTA
*    UART_MOD_2     INT_ID_SCITXINTB
*
*******************************************************************************/
#define UART_getTxPieId( Mod ) ((INT_PieId)(UART_getIndex(Mod)*2+(int)INT_ID_SCITXINTA))

/*******************************************************************************
* MACRO         : UART_ackTxInt
* INPUT         : UART_Module Mod
*                 Selects the UART module.
* RETURNS       : void
* DESCRIPTION   :
* Used within an interrupt service routine to clear both the Tx interrupt flag
* and the PIE group acknowledgment flag.
*
* EXAMPLES
* Clears the Tx interrupt flag and the PIE group acknowledgment flag after the
* UART module 1 generates a Tx interrupt and the PIE controller calls the ISR.
* First a character is written to the Tx FIFO buffer and then the flags are
* cleared.
*
*   interrupt void isr_uart1_tx( void )
*   {
*      // Write character
*      UART_putc( UART_MOD_1, 'a');
*
*      // Clear flags
*      UART_ackTxInt( UART_MOD_1 );
*   }
*
*******************************************************************************/
#define UART_ackTxInt( Mod ) UART_clrTxInt(Mod); INT_ackGroup(INT_GROUP_9)

/* public methods */
extern void UART_config( UART_Module Mod, GPIO_Pin Rx, GPIO_Pin Tx,
                         uint16_t Ticks, UART_DataBits DataBits,
                         UART_Parity Parity, UART_StopBits StopBits );
extern void UART_flush( UART_Module Mod );
extern void UART_flushRx( UART_Module Mod );
extern void UART_flushTx( UART_Module Mod );
extern void UART_putc( UART_Module Mod, int a );
extern void UART_puts( UART_Module Mod, const char* str );
extern int UART_getRxCount( UART_Module Mod );
extern char UART_getc( UART_Module Mod );
extern int UART_isRxOverFlow( UART_Module Mod );
extern void UART_clrRxOverFlow( UART_Module Mod );
extern void UART_setRxCallback( UART_Module Mod, INT_IsrAddr Func, int RxLevel );
extern void UART_setTxCallback( UART_Module Mod, INT_IsrAddr Func, int TxLevel );
extern void UART_enableRxInt( UART_Module Mod, int Enable );
extern void UART_enableTxInt( UART_Module Mod, int Enable );
extern void UART_setTicks( UART_Module Mod, uint16_t Ticks );
extern void UART_setLoopback( UART_Module Mod, int Value );
#ifdef LIB_FUNC
extern void UART_clrRxInt( UART_Module Mod );
extern void UART_clrTxInt( UART_Module Mod );
#endif /* LIB_FUNC */

/********** USER MIDDLE SECTION ***********************************************/
#ifndef LIB_FUNC
#define UART_clrRxInt( Mod ) MACRO_UART_clrRxInt( Mod )
#define UART_clrTxInt( Mod ) MACRO_UART_clrTxInt( Mod )
#endif /* LIB_FUNC */


#define MACRO_UART_clrRxInt( Mod ) Mod->SCIFFRX.bit.RXFFINTCLR = 1
#define MACRO_UART_clrTxInt( Mod ) Mod->SCIFFTX.bit.TXFFINTCLR = 1



/********** END ***************************************************************/
#endif

