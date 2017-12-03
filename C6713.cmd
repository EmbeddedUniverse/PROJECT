/****************************************************************************/
/*  C6713.cmd                                                               */
/*  Copyright (c) 2010 Texas Instruments Incorporated                       */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on an TMS320C6713        */
/*                 device.  Use it as a guideline.  You will want to        */
/*                 change the memory layout to match your specific C6xxx    */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

MEMORY
{
    IVECS:   o = 00000000h   l = 0x00000400	 /* reset & interrupt vectors     */
    IRAM:    o = 00000400h   l = 0x0002FC00	 /* intended for initialization   */
    SDRAM	 o = 0x80000000  l = 0x00800000
    //EMIFCE0  o = 0x80000000  l = 0x10000000  /* SDRAM in 6713 DSK */
    EMIFCE1  o = 0x90000000  l = 0x10000000  /* Flash/CPLD in 6713 DSK */
    EMIFCE2  o = 0xA0000000  l = 0x10000000  /* Daughterboard in 6713 DSK */
    EMIFCE3  o = 0xB0000000  l = 0x10000000  /* Daughterboard in 6713 DSK */
}

SECTIONS
{
	.vectors       >  IVECS
    .text          >  IRAM
    .stack         >  IRAM
    .bss           >  IRAM
    .cio           >  IRAM
    .const         >  IRAM
    .data          >  IRAM
    .switch        >  IRAM
    .sysmem        >  IRAM
    .far           >  SDRAM
    .args          >  IRAM
    .ppinfo        >  IRAM
    .ppdata        >  IRAM
  
    /* COFF sections */
    .pinit         >  IRAM
    .cinit         >  SDRAM
  
    /* EABI sections */
    .binit         >  IRAM
    .init_array    >  IRAM
    .neardata      >  IRAM
    .fardata       >  IRAM
    .rodata        >  IRAM
    .c6xabi.exidx  >  IRAM
    .c6xabi.extab  >  IRAM
}
