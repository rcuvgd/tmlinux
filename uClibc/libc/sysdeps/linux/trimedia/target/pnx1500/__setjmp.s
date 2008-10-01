#include <asm/linkage.h>
#include <asm/regdef.h>

ENTRY(__setjmp)
	(* cycle 0 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   readdpc -> REG_temp1,                     
	IF r1   h_st32d(124) r31 r5,                
	IF r1   h_st32d(128) r32 r5;                

	(* cycle 1 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(116) r29 r5;                

	(* cycle 2 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(108) r27 r5,                
	IF r1   h_st32d(120) r30 r5;                

	(* cycle 3 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(100) r25 r5,                
	IF r1   h_st32d(112) r28 r5;                

	(* cycle 4 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(92) r23 r5,                
	IF r1   h_st32d(104) r26 r5;               

	(* cycle 5 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(84) r21 r5,                
	IF r1   h_st32d(96) r24 r5;                

	(* cycle 6 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(76) r19 r5,                
	IF r1   h_st32d(88) r22 r5;                

	(* cycle 7 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(68) r17 r5,                
	IF r1   h_st32d(80) r20 r5;                

	(* cycle 8 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(60) r15 r5,                
	IF r1   h_st32d(72) r18 r5;                

	(* cycle 9 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(52) r13 r5,                
	IF r1   h_st32d(64) r16 r5;                

	(* cycle 10 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(44) r11 r5,                
	IF r1   h_st32d(56) r14 r5;                

	(* cycle 11 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(36) r9 r5,                 
	IF r1   h_st32d(48) r12 r5;                

	(* cycle 12 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   readpcsw -> REG_temp3,                   
	IF r1   h_st32d(28) r7 r5,                 
	IF r1   h_st32d(40) r10 r5;                

	(* cycle 13 *)
	IF r1   nop,
	IF r1   ijmpt r1 r2,                       
	IF r1   nop,
	IF r1   h_st32d(20) REG_temp3 r5,               
	IF r1   h_st32d(32) r8 r5;                

	(* cycle 14 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   readspc -> REG_temp2,                   
	IF r1   h_st32d(12) REG_temp1 r5,               
	IF r1   h_st32d(24) r6 r5;                

	(* cycle 15 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(4) r3 r5,                 
	IF r1   h_st32d(16) REG_temp2 r5;               

	(* cycle 16 *)
	IF r1   iadd r0 r0 -> r5,                 
	IF r1   nop,
	IF r1   nop,
	IF r1   h_st32d(0) r2 r5,                 
	IF r1   h_st32d(8) r4 r5;                 
