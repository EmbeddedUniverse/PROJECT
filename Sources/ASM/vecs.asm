; ************************************************************************
; THIS PROGRAM IS PROVIDED "AS IS". TI MAKES NO WARRANTIES OR
; REPRESENTATIONS, EITHER EXPRESS, IMPLIED OR STATUTORY, 
; INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS 
; FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR 
; COMPLETENESS OF RESPONSES, RESULTS AND LACK OF NEGLIGENCE. 
; TI DISCLAIMS ANY WARRANTY OF TITLE, QUIET ENJOYMENT, QUIET 
; POSSESSION, AND NON-INFRINGEMENT OF ANY THIRD PARTY 
; INTELLECTUAL PROPERTY RIGHTS WITH REGARD TO THE PROGRAM OR 
; YOUR USE OF THE PROGRAM.
;
; IN NO EVENT SHALL TI BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
; CONSEQUENTIAL OR INDIRECT DAMAGES, HOWEVER CAUSED, ON ANY 
; THEORY OF LIABILITY AND WHETHER OR NOT TI HAS BEEN ADVISED 
; OF THE POSSIBILITY OF SUCH DAMAGES, ARISING IN ANY WAY OUT 
; OF THIS AGREEMENT, THE PROGRAM, OR YOUR USE OF THE PROGRAM. 
; EXCLUDED DAMAGES INCLUDE, BUT ARE NOT LIMITED TO, COST OF 
; REMOVAL OR REINSTALLATION, COMPUTER TIME, LABOR COSTS, LOSS 
; OF GOODWILL, LOSS OF PROFITS, LOSS OF SAVINGS, OR LOSS OF 
; USE OR INTERRUPTION OF BUSINESS. IN NO EVENT WILL TI'S 
; AGGREGATE LIABILITY UNDER THIS AGREEMENT OR ARISING OUT OF 
; YOUR USE OF THE PROGRAM EXCEED FIVE HUNDRED DOLLARS 
; (U.S.$500).
;
; Unless otherwise stated, the Program written and copyrighted 
; by Texas Instruments is distributed as "freeware".  You may, 
; only under TI's copyright in the Program, use and modify the 
; Program without any charge or restriction.  You may 
; distribute to third parties, provided that you transfer a 
; copy of this license to the third party and the third party 
; agrees to these terms by its first use of the Program. You 
; must reproduce the copyright notice and any other legend of 
; ownership on each copy or partial copy, of the Program.
;
; You acknowledge and agree that the Program contains 
; copyrighted material, trade secrets and other TI proprietary 
; information and is protected by copyright laws, 
; international copyright treaties, and trade secret laws, as 
; well as other intellectual property laws.  To protect TI's 
; rights in the Program, you agree not to decompile, reverse 
; engineer, disassemble or otherwise translate any object code 
; versions of the Program to a human-readable form.  You agree 
; that in no event will you alter, remove or destroy any 
; copyright notice included in the Program.  TI reserves all 
; rights not specifically granted under this license. Except 
; as specifically provided herein, nothing in this agreement 
; shall be construed as conferring by implication, estoppel, 
; or otherwise, upon you, any license or other right under any 
; TI patents, copyrights or trade secrets.
;
; You may not use the Program in non-TI devices.
; ************************************************************************ 
; FILENAME...... vecs.asm 
; DATE CREATED.. 02/22/2001 
; ************************************************************************ 
; Global symbols defined here and exported out of this file

  .global _vectors
  .global _vector0
  .global _vector1
  .global _vector2
  .global _vector3
  .global _vector4
  .global _vector5
  .global _vector6
  .global _vector7
  .global _c_int08 ; Hookup the c_int08 ISR in main() for EDMA
  .global _c_int09 ; Hookup the c_int09 ISR in main() for DMA
  .global _vector10
  .global _c_int11 ; Hookup the c_int09 ISR in main() for DMA
  .global _vector12
  .global _vector13
  .global _vector14
  .global _vector15

; ************************************************************************ 
; Global symbols referenced in this file but defined somewhere else.
; Remember that your interrupt service routines need to be referenced here.
; ************************************************************************ 

  .ref _c_int00

; ************************************************************************ 
; This is a mcros that instantiates one entry in the inetrrupt service table.
; ************************************************************************ 

VEC_ENTRY .macro addr
  STW 	B0,*--B15
  MVKL 	addr,B0
  MVKH 	addr,B0
  B 	B0
  LDW 	*B15++,B0
  NOP 	2
  NOP
  NOP
  .endm
  
; ************************************************************************ 
; This is a dummy interrupt service routine used to initialize the IST.
; ************************************************************************ 

_vec_dummy:
  B 	B3
  NOP 	5
  
; ************************************************************************ 
; This is the actual interrupt service table (IST). It is properly aligned and
; is located in the subsection .text:vecs. This means if you don’t explicitly
; specify this section in your linker command file, it will default and link 
; into the .text section. Remember to set the ISTP register to point to this 
; table. 
; ************************************************************************ 

  .sect ".text:vecs"
  .align 1024
  
_vectors:
_vector0: VEC_ENTRY _vec_dummy
_vector1: VEC_ENTRY _vec_dummy
_vector2: VEC_ENTRY _vec_dummy
_vector3: VEC_ENTRY _vec_dummy
_vector4: VEC_ENTRY _vec_dummy
_vector5: VEC_ENTRY _vec_dummy
_vector6: VEC_ENTRY _vec_dummy
_vector7: VEC_ENTRY _vec_dummy
_vector8: VEC_ENTRY _c_int08 ; Hookup the c_int08 ISR in main() for EDMA
_vector9: VEC_ENTRY _c_int09 ; Hookup the c_int09 ISR in main() for DMA
_vector10: VEC_ENTRY _vec_dummy
_vector11: VEC_ENTRY _c_int11 ; Hookup the c_int09 ISR in main() for DMA
_vector12: VEC_ENTRY _vec_dummy
_vector13: VEC_ENTRY _vec_dummy
_vector14: VEC_ENTRY _vec_dummy
_vector15: VEC_ENTRY _vec_dummy

; ************************************************************************ 
; *******************************End of vecs.asm**************************
; ************************************************************************ 

