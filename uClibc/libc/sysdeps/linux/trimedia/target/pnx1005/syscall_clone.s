#include <asm/linkage.h>
#include <asm/regdef.h>
#include <asm/asm-offsets.h>

ENTRY(__syscall_clone)
	(* cycle 0 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   isubi(0x18) r6 -> r7,                   
	IF r1   ld32d(0) r4 -> r8,                      
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 1 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 2 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 3 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 4 *)
	IF r1   nop,
	IF r1   h_st32d(-24) r8 r6,                    
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 5 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(4) r4 -> r8,                    
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 6 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 7 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 8 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 9 *)
	IF r1   nop,
	IF r1   h_st32d(-20) r8 r6,                     
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 10 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(8) r4 -> r8,                     
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 11 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 12 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 13 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 14 *)
	IF r1   nop,
	IF r1   h_st32d(-16) r8 r6,                     
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 15 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(12) r4 -> r8,                    
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 16 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 17 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 18 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 19 *)
	IF r1   nop,
	IF r1   h_st32d(-12) r8 r6,                     
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 20 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(16) r4 -> r8,                     
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 21 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 22 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 23 *)
	IF r1   ijmpi(SYMBOL_NAME(__syscall)),                      
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 24 *)
	IF r1   nop,
	IF r1   h_st32d(-8) r8 r6,                      
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 25 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(20) r4 -> r8,                     
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 26 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 27 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 28 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 29 *)
	IF r1   iadd r0 r5 -> r6,                       
	IF r1   h_st32d(-4) r8 r6,                     
	IF r1   imm32(__NR_clone) -> r5,              
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;
