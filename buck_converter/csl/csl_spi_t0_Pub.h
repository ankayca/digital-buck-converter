/*******************************************************************************
* (c) Copyright 2010 Biricha Digital Power Limited
* FILE          : csl_spi_t0_Pub.h
* AUTHOR        : Originally written by Biricha Digital Power Ltd.
*                 http://www.biricha.com
* PROJECT       : Chip Support Libary
* DESCRIPTION COPY:
* Contains functions to set up the SPI module.
*
* SPI_config() must be called before any of the API functions can be called.
* The functions allow SPI module to be configured as a master using the FIFOs
* to transmit and receive data.
*
* EXAMPLES
* Sets up the SPI module and enables the internal loopback mode. The module
* transmits 0x55 and then the code waits until this is received back.
*
*   SPI_config( SPI_MOD_1, 4, SPI_DO_POS_DI_NEG );
*   SPI_setLoopback(SPI_MOD_1, 1);
*
*   for(;;)
*   {
*      // Transmit data
*      SPI_write(SPI_MOD_1,0x55);
*
*      // Wait until data is received
*      while(SPI_getRxCount(SPI_MOD_1) == 0);
*   }
*
* LINKS
* file:///C:/tidcs/c28/CSL_C280x/v100/doc/CSL_C280x.pdf
* http://focus.ti.com/lit/ug/spru059e/spru059e.pdf
* http://focus.ti.com/lit/ug/sprug71b/sprug71b.pdf (2802x, 2803x)
*
* HISTORY       :
*******************************************************************************/


#ifndef _CSL_SPI_T0_PUB_H
#define _CSL_SPI_T0_PUB_H

/********** INCLUDES GLOBAL SECTION *******************************************/
#define SPI_MOD_1_ALT ((SPI_Module)0x007720)

/********** USER START SECTION ************************************************/
typedef struct TL_SPI_MOD_SIZE
{
    uint16_t A[0x20];
}TL_SPI_MOD_SIZE;

/********** TYPES SECTION *****************************************************/

/*******************************************************************************
* COMPLEX       : SPI_MOD_X
* DESCRIPTION   :
* These values are used to specify the SPI module.
*******************************************************************************/
#if 1
#define SPI_MOD_1 (&SpiaRegs)
#define SPI_MOD_2 (&SpibRegs)
#define SPI_MOD_3 (&SpicRegs)
#define SPI_MOD_4 (&SpidRegs)
#endif

/*******************************************************************************
* COMPLEX       : SPI_Module
* DESCRIPTION   :
* This is used to map hardware register values to SPI_Module.
*******************************************************************************/
typedef volatile struct SPI_REGS* SPI_Module;

/*******************************************************************************
* ENUM          : SPI_ClockEdge
* DESCRIPTION   :
* This defines the different sampling/transmitting options for the SPI module.
*******************************************************************************/
typedef enum SPI_ClockEdge
{
    SPI_DO_POS_DI_NEG   = 0,    /* data out on positive clock edge and in on following negative clock edge */
    SPI_DO_NEG_DI_POS   = 1     /* data out on negative clock edge and in on following positive clock edge */
} SPI_ClockEdge;


/********** PROTOTYPES SECTIONS ***********************************************/

/*******************************************************************************
* MACRO         : SPI_getIndex
* INPUT         : SPI_Module Mod
*                 Selects the SPI module.
* RETURNS       : uint16_t
* DESCRIPTION   :
* Returns an index value for each SPI module.
*
* The possibilities are as follows,
*
*   SPI_MOD_1 = 0
*   SPI_MOD_2 = 1
*   SPI_MOD_3 = 2
*   SPI_MOD_4 = 3
*
* EXAMPLES
* Returns 1 for SPI 2.
*
*   uint16_t SPIIndex = SPI_getIndex( SPI_MOD_2 );
*
* NOTES
*******************************************************************************/
#define SPI_getIndex( Mod ) (Mod==SPI_MOD_1?0:(((TL_SPI_MOD_SIZE*)(Mod)-(TL_SPI_MOD_SIZE*)SPI_MOD_1_ALT)))

/*******************************************************************************
* MACRO         : SPI_getTxPieId
* INPUT         : SPI_Module Mod
*                 Selects the SPI module.
* RETURNS       : INT_PieId
* DESCRIPTION   :
* Returns the PIE Id for the Tx part of the specified SPI module.
*
* The possibilities are as follows,
*
*    SPI_MOD_1     INT_ID_SPITXA
*    SPI_MOD_2     INT_ID_SPITXB
*    SPI_MOD_3     INT_ID_SPITXC
*    SPI_MOD_4     INT_ID_SPITXD
*
* EXAMPLES
* Returns INT_ID_SCITXINTA for SPI module 1.
*
*   INT_PieId id = SPI_getTxPieId( SPI_MOD_1 );
*
*******************************************************************************/
#define SPI_getTxPieId( Mod ) ((INT_PieId)(SPI_getIndex(Mod)*2+(int)INT_ID_SPITXA))

/*******************************************************************************
* MACRO         : SPI_ackTxInt
* INPUT         : SPI_Module Mod
*                 Selects the SPI module.
* RETURNS       : void
* DESCRIPTION   :
* Used within an interrupt service routine to clear both the Tx interrupt flag
* and the PIE group acknowledgment flag.
*
* EXAMPLES
* Clears the Tx interrupt flag and the PIE group acknowledgment flag after the
* SPI module 1 generates a Tx interrupt and the PIE controller calls the ISR.
*
*   interrupt void isr_spi1_tx( void )
*   {
*      // User code
*
*      // Clear flags
*      SPI_ackTxInt( SPI_MOD_1 );
*   }
*
*******************************************************************************/
#define SPI_ackTxInt( Mod ) SPI_clrTxInt(Mod); INT_ackPieGroup(SPI_getTxPieId(Mod))

/*******************************************************************************
* MACRO         : SPI_getRxPieId
* INPUT         : SPI_Module Mod
*                 Selects the SPI module.
* RETURNS       : void
* DESCRIPTION   :
* Returns the PIE Id for the Rx part of the specified SPI module.
*
* The possibilities are as follows,
*
*    SPI_MOD_1     INT_ID_SPIRXA
*    SPI_MOD_2     INT_ID_SPIRXB
*    SPI_MOD_3     INT_ID_SPIRXC
*    SPI_MOD_4     INT_ID_SPIRXD
*
* EXAMPLES
* Returns INT_ID_SCIRXINTA for SPI module 1.
*
*   INT_PieId id = SPI_getRxPieId( SPI_MOD_1 );
*
*******************************************************************************/
#define SPI_getRxPieId( Mod ) ((INT_PieId)(SPI_getIndex(Mod)*2+(int)INT_ID_SPIRXA))

/*******************************************************************************
* MACRO         : SPI_ackRxInt
* INPUT         : SPI_Module Mod
*                 Selects the SPI module.
* RETURNS       : void
* DESCRIPTION   :
* Used within an interrupt service routine to clear both the Rx interrupt flag
* and the PIE group acknowledgment flag.
*
* EXAMPLES
* Clears the Rx interrupt flag and the PIE group acknowledgment flag after the
* SPI module 1 generates an Rx interrupt and the PIE controller calls the ISR.
* The word is read from the Rx FIFO buffer first and then the flags are
* cleared.
*
*   interrupt void isr_spi1_rx( void )
*   {
*      // Get character
*      word = SPI_read( SPI_MOD_1 );
*
*      // Clear flags
*      SPI_ackRxInt( SPI_MOD_1 );
*   }
*
*******************************************************************************/
#define SPI_ackRxInt( Mod ) SPI_clrRxInt(Mod); INT_ackPieGroup(SPI_getRxPieId(Mod))

/*******************************************************************************
* MACRO         : SPI_baudToTicks
* INPUT         : int baud
*                 Selects the baud rate.
* RETURNS       : uint16_t
* DESCRIPTION   :
* Converts the required baud rate to the number of SPI clock ticks using the
* following formula.
*
*                      SYS_CLK_HZ
* Ticks =  ------------------------------ - 1
*               USR_PER_LSP_DIV * baud
*
* EXAMPLES
* Returns the SPI ticks required for a baud rate of 115200.
*
*   uint16_t BaudTicks = SPI_baudToTicks( 115200 );
*
* NOTES
* This function only works if the value returned is not 0,1 or 2 due to the
* forumal below
*  for Ticks = 0, 1, or 2
*
*                      SYS_CLK_HZ
* baud =  ------------------------------
*               USR_PER_LSP_DIV * 4
*
*******************************************************************************/
#define SPI_baudToTicks( baud ) ((((10L*SYS_CLK_HZ)/(SYS_LIT_VALUE(USR_PER_LSP_DIV)*baud))-5)/10)

/* public methods */
extern void SPI_config( SPI_Module Spi, uint16_t BRR, SPI_ClockEdge Mode );
extern void SPI_setTxCallback( SPI_Module Spi, INT_IsrAddr Func,
                               uint16_t TxLevel );
extern void SPI_flush( SPI_Module Spi );
extern void SPI_reset( SPI_Module Spi );
#ifdef LIB_FUNC
extern void SPI_clrTxInt( SPI_Module Spi );
extern void SPI_write( SPI_Module Spi, uint16_t Value );
extern uint16_t SPI_getRxCount( SPI_Module Spi );
extern uint16_t SPI_read( SPI_Module Spi );
#endif /* LIB_FUNC */
extern void SPI_setLoopback( SPI_Module Mod, int Value );
extern void SPI_setRxCallback( SPI_Module Spi, INT_IsrAddr Func,
                               uint16_t RxLevel );
#ifdef LIB_FUNC
extern void SPI_clrRxInt( SPI_Module Spi );
#endif /* LIB_FUNC */

/********** USER MIDDLE SECTION ***********************************************/
#ifndef LIB_FUNC
#define SPI_clrTxInt( Spi )         MACRO_SPI_clrTxInt( Spi )
#define SPI_clrRxInt( Spi )         MACRO_SPI_clrRxInt( Spi )
#define SPI_write( Spi, Value )     MACRO_SPI_write( Spi, Value )
#define SPI_getRxCount( Spi )       MACRO_SPI_getRxCount( Spi )
#define SPI_read( Spi )             MACRO_SPI_read( Spi )
#endif

#define MACRO_SPI_clrTxInt( Spi )       Spi->SPIFFTX.bit.TXFFINTCLR = 1
#define MACRO_SPI_clrRxInt( Spi )       Spi->SPIFFRX.bit.RXFFINTCLR = 1
#define MACRO_SPI_write( Spi, Value )   Spi->SPITXBUF = Value
#define MACRO_SPI_getRxCount( Spi )     Spi->SPIFFRX.bit.RXFFST
#define MACRO_SPI_read( Spi )           Spi->SPIRXBUF


/********** END ***************************************************************/
#endif

