;****************************************************************************************
; APPEL :
; int IIR_secOrde(int x, int w[], const short C[]);
;
; A4 : Échantillon courant
; B4 : Tampon de valeurs intermédiaires
; A6 : Coefficients du filtre
;
; Return : l'amplitude de l'échantillon filtré
;
;****************************************************************************************
	.def _IIR_secOrder

_IIR_secOrder:

	MVC CSR, B5

	;
	; Saturation sur 25 bits
	;

	; Plus grand
	MVKL 0xFFFFFF, A2
	MVKH 0xFFFFFF, A2

	CMPGT A4, A2, A1
	[A1] MV A2, A1

	; Plus petit
	MVKL -0x1000000, A2
	MVKH -0x1000000, A2

	CMPLT A4, A2, A1
	[A1] MV A2, A4

	;
	; a0*x(n)
	;

	; C en 32 bits
	LDH *+A6[3], A5
	NOP 4

	MPYID A4, A5, A3:A2
	NOP 9

	; memA dans B0
	SHR A3:A2, 13, A3:A2

	MV A2, B0

	;
	; a1*w(n-1)
	;

	; C en 32 bits
	LDH *+A6[4], A5
	LDW *+B4[1], A7
	NOP 4

	MPYID A7, A5, A3:A2
	NOP 9

	; memB dans B1
	SHR A3:A2, 13, A3:A2

	MV A2, B1

	;
	; a0*x(n)-a1*w(n-1)
	;

	; memC dans B2
	SUB B0, B1, B2

	;
	; a2*w(n-2)
	;

	; C en 32 bits
	LDH *+A6[5], A5
	LDW *+B4[2], A7
	NOP 4

	MPYID A7, A5, A3:A2
	NOP 9

	; memB dans B1
	SHR A3:A2, 13, A3:A2
	MV A2, B1

	;
	; a0*x(n)-a1*w(n-1)-a2*w(n-2)
	;

	; memA dans B0
	SUB B2, B1, B0

	;
	; Saturation sur 25 bits	memB = sat_25bits(memA)
	;

	; Plus grand
	MVKL 0xFFFFFF, A2
	MVKH 0xFFFFFF, A2

	CMPGT B0, A2, A1
	[A1] MV A2, B1
	[!A1] MV B0, B1

	; Plus petit
	MVKL -0x1000000, A2
	MVKH -0x1000000, A2

	CMPLT B0, A2, A1
	[A1] MV A2, B1

	; w[0] = memB
	STW B1, *B4

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;			y(n) = b0*w(n)+b1*w(n-1)+b2*w(n-2)
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;
	; b0*w(n)
	;

	; C en 32 bits
	LDH *A6, A5
	LDW *B4, A7
	NOP 4

	MPYID A7, A5, A3:A2
	NOP 9

	; memA dans B0
	SHR A3:A2, 13, A3:A2
	MV A2, B0

	;
	; b1*w(n-1)
	;

	; C en 32 bits
	LDH *+A6[1], A5
	LDW *+B4[1], A7
	NOP 4

	MPYID A7, A5, A3:A2
	NOP 9

	; memB dans B1
	SHR A3:A2, 13, A3:A2
	MV A2, B1


	;
	; b0*w(n)+b1*w(n-1)
	;

	; memC dans B2
	ADD B0, B1, B2

	;
	; b2*w(n-2)
	;

	; C en 32 bits
	LDH *+A6[2], A5
	LDW *+B4[2], A7
	NOP 4

	MPYID A7, A5, A3:A2
	NOP 9

	; memA dans B0
	SHR A3:A2, 13, A3:A2
	MV A2, B0

	;
	; b0*w(n)+b1*w(n-1)+b2*w(n-2)
	;
	ADD B2,B0,B1
	MV B1, A4

	;
	;Décalage
	;

	LDW *+B4[1],B0
	NOP 4
	STW B0, *+B4[2]
	LDW *+B4[0],B0
	NOP 4
	STW B0, *+B4[1]

	MVC B5, CSR

	B B3
	NOP 5

	.end
