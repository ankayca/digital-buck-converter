MEMORY
{

PAGE 0 : /* FLASH Memory */
   FLASH  : origin = 0x3E8000, length = 0x00FFF0     /* on-chip FLASH */

PAGE 1:    /* PIE Memory as defined by DSP280x_Headers_nonBIOS.cmd */
   CLA1_MSGRAMLOW       : origin = 0x001480, length = 0x000080
   CLA1_MSGRAMHIGH      : origin = 0x001500, length = 0x000080

PAGE 2 : /* ROM */
   /* BEGIN is used for the "boot to SARAM" bootloader mode   */

   BOOTROM              : origin = 0x3FF27C, length = 0x000D44
   RESET                : origin = 0x3FFFC0, length = 0x000002
   VECTORS              : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM  */
   IQTABLES             : origin = 0x3FE000, length = 0x000B50     /* IQ Math Tables in Boot ROM */
   IQTABLES2            : origin = 0x3FEB50, length = 0x00008C     /* IQ Math Tables in Boot ROM */
   IQTABLES3            : origin = 0x3FEBDC, length = 0x0000AA	 /* IQ Math Tables in Boot ROM */

PAGE 3 : /* RAM */

   RAMM                 : origin = 0x000050, length = 0x0007B0  /* on-chip RAM block M1 & M0 */
   RAML                 : origin = 0x008000, length = 0x001000


PAGE 4 : /* Reserved RAM */
   BEGIN                : origin = 0x000000, length = 0x00004
   BOOT_RSVD            : origin = 0x000004, length = 0x00004E     /* Part of M0, BOOT rom will use this for stack */

   CLA1_PROG            : origin = 0x009000, length = 0x001000
}


SECTIONS
{
   /* Allocate uninitalized data sections: */
   .stack           : PAGE = 3
   .ebss            : PAGE = 3
   .esysmem         : PAGE = 3
   .sysmem          : PAGE = 3
   .cio             : PAGE = 3
   
   IQmathTables     : > IQTABLES,  PAGE = 2, TYPE = NOLOAD
   bootrom          : > BOOTROM    PAGE = 2, TYPE = DSECT
   .reset           : > RESET,     PAGE = 2, TYPE = DSECT /* not used, */
    vectors         : > VECTORS    PAGE = 2, TYPE = DSECT

   codestart        : > BEGIN,     PAGE = 4
   ramfuncs         : PAGE = 0

   dummyramfuncs    :   LOAD = RAMM, PAGE = 3
                      	RUN  = RAMM, PAGE = 3
                      	LOAD_START(_RamfuncsLoadStart),
                      	LOAD_END(_RamfuncsLoadEnd),
                      	RUN_START(_RamfuncsRunStart)

   .cinit           : PAGE = 3
   .pinit           : PAGE = 3
   .text            : >> RAMM | RAML     PAGE = 3 
   .econst          : PAGE = 3
   .switch          : PAGE = 3
   IQmath           : PAGE = 3


   Cla1Prog         : LOAD = RAMM,      PAGE = 3
                      RUN  = CLA1_PROG, PAGE = 4
                      LOAD_START(_Cla1funcsLoadStart),
                      LOAD_END(_Cla1funcsLoadEnd),
                      RUN_START(_Cla1funcsRunStart)

   Cla1ToCpuMsgRAM  : > CLA1_MSGRAMLOW,   PAGE = 1
   CpuToCla1MsgRAM  : > CLA1_MSGRAMHIGH,  PAGE = 1

  
}

