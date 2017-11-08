/*******************************************************************/
/* THIS PROGRAM IS PROVIDED "AS IS". TI MAKES NO WARRANTIES OR     */
/* REPRESENTATIONS, EITHER EXPRESS, IMPLIED OR STATUTORY,          */
/* INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS    */
/* FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR          */
/* COMPLETENESS OF RESPONSES, RESULTS AND LACK OF NEGLIGENCE.      */
/* TI DISCLAIMS ANY WARRANTY OF TITLE, QUIET ENJOYMENT, QUIET      */
/* POSSESSION, AND NON-INFRINGEMENT OF ANY THIRD PARTY             */
/* INTELLECTUAL PROPERTY RIGHTS WITH REGARD TO THE PROGRAM OR      */
/* YOUR USE OF THE PROGRAM.                                        */
/*                                                                 */
/* IN NO EVENT SHALL TI BE LIABLE FOR ANY SPECIAL, INCIDENTAL,     */
/* CONSEQUENTIAL OR INDIRECT DAMAGES, HOWEVER CAUSED, ON ANY       */
/* THEORY OF LIABILITY AND WHETHER OR NOT TI HAS BEEN ADVISED      */
/* OF THE POSSIBILITY OF SUCH DAMAGES, ARISING IN ANY WAY OUT      */
/* OF THIS AGREEMENT, THE PROGRAM, OR YOUR USE OF THE PROGRAM.     */
/* EXCLUDED DAMAGES INCLUDE, BUT ARE NOT LIMITED TO, COST OF       */
/* REMOVAL OR REINSTALLATION, COMPUTER TIME, LABOR COSTS, LOSS     */
/* OF GOODWILL, LOSS OF PROFITS, LOSS OF SAVINGS, OR LOSS OF       */
/* USE OR INTERRUPTION OF BUSINESS. IN NO EVENT WILL TI'S          */
/* AGGREGATE LIABILITY UNDER THIS AGREEMENT OR ARISING OUT OF      */
/* YOUR USE OF THE PROGRAM EXCEED FIVE HUNDRED DOLLARS             */
/* (U.S.$500).                                                     */
/*                                                                 */
/* Unless otherwise stated, the Program written and copyrighted    */
/* by Texas Instruments is distributed as "freeware".  You may,    */
/* only under TI's copyright in the Program, use and modify the    */
/* Program without any charge or restriction.  You may             */
/* distribute to third parties, provided that you transfer a       */
/* copy of this license to the third party and the third party     */
/* agrees to these terms by its first use of the Program. You      */
/* must reproduce the copyright notice and any other legend of     */
/* ownership on each copy or partial copy, of the Program.         */
/*                                                                 */
/* You acknowledge and agree that the Program contains             */
/* copyrighted material, trade secrets and other TI proprietary    */
/* information and is protected by copyright laws,                 */
/* international copyright treaties, and trade secret laws, as     */
/* well as other intellectual property laws.  To protect TI's      */
/* rights in the Program, you agree not to decompile, reverse      */
/* engineer, disassemble or otherwise translate any object code    */
/* versions of the Program to a human-readable form.  You agree    */
/* that in no event will you alter, remove or destroy any          */
/* copyright notice included in the Program.  TI reserves all      */
/* rights not specifically granted under this license. Except      */
/* as specifically provided herein, nothing in this agreement      */
/* shall be construed as conferring by implication, estoppel,      */
/* or otherwise, upon you, any license or other right under any    */
/* TI patents, copyrights or trade secrets.                        */
/*                                                                 */
/* You may not use the Program in non-TI devices.                  */
/*******************************************************************/
/* link.cmd                                                        */
/* MEMORY MAP                                                      */
/*******************************************************************/

-c
-heap 0x2000
-stack 0x0100
/*-l rts6700.lib*/
/*-l csl6711.lib*/
-l rts6700.lib
-l csl6713.lib


MEMORY
{
    IVECS:   o = 00000000h      l = 0x00000400	/* reset & interrupt vectors     */
    IRAM:    o = 00000400h      l = 0x0002FC00	/* intended for initialization   */
    L2RAM    o = 0x00030000  	l = 0x00010000  /* 64kB - Internal RAM/CACHE */
    SDRAM:   o = 80000000h      l = 0x10000000 /* SDRAM in 6713 DSK */
    EMIFCE1  o = 0x90000000  	l = 0x10000000  /* Flash/CPLD in 6713 DSK */
    EMIFCE2  o = 0xA0000000  	l = 0x10000000  /* Daughterboard in 6713 DSK */
	EMIFCE3  o = 0xB0000000 	l = 0x10000000 /* Daughterboard in 6713 DSK */
}

SECTIONS
{
    .vectors    >       IVECS
    "xmit_buf"  >       IRAM
    "recv_buf"  >       IRAM
    .text          >  IRAM
    .stack         >  IRAM
    .bss           >  IRAM
    .cio           >  IRAM
    .const         >  IRAM
    .data          >  IRAM
    .switch        >  IRAM
    .sysmem        >  IRAM
    .far           >  IRAM
    .args          >  IRAM
    .ppinfo        >  IRAM
    .ppdata        >  IRAM

    /* COFF sections */
    .pinit         >  IRAM
    .cinit         >  IRAM

    /* EABI sections */
    .binit         >  IRAM
    .init_array    >  IRAM
    .neardata      >  IRAM
    .fardata       >  IRAM
    .rodata        >  IRAM
    .c6xabi.exidx  >  IRAM
	.c6xabi.extab > IRAM
}
