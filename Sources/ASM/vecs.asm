  .global _vectors
  .global _vector0
  .global _vector1
  .global _vector2
  .global _vector3
  .global _vector4
  .global _vector5
  .global _vector6
  .global _vector7
  .global _c_int08 ; Serial port interrupt
  .global _vector9
  .global _vector10
  .global _vector11
  .global _vector12
  .global _vector13
  .global _vector14
  .global _vector15

  .ref _c_int00

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
  

_vec_dummy:
  B 	B3
  NOP 	5
  


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
_vector8: VEC_ENTRY _c_int08
_vector9: VEC_ENTRY _vec_dummy
_vector11: VEC_ENTRY _vec_dummy
_vector12: VEC_ENTRY _vec_dummy
_vector13: VEC_ENTRY _vec_dummy
_vector14: VEC_ENTRY _vec_dummy
_vector15: VEC_ENTRY _vec_dummy

