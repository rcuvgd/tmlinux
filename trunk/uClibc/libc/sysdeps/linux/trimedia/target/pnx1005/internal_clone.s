#include <asm/linkage.h>
#include <asm/regdef.h>

ENTRY(__internal_clone)
____internal_clone_DT_0:
	(* cycle 0 *)
	IF r1   ijmpi(___syscall_clone),                
	IF r1   h_st32d(-8) r11 r4,                     
	IF r1   h_st32d(-12) r10 r4,                    
	IF r1   iadd r0 r5 -> r10,                      
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 1 *)
	IF r1   iadd r0 r7 -> r5,                       
	IF r1   h_st32d(-16) r9 r4,                     
	IF r1   iadd r0 r8 -> r9,                       
	IF r1   isubi(0x18) r4 -> r4,                   
	IF r1   iadd r0 r2 -> r12,                      
	IF r1   imm32(____internal_clone_DT_1) -> r2,   
	IF r1   h_st32d(-4) r12 r4,                     
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
	IF r1   nop,
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
	IF r1   nop,
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

____internal_clone_DT_1:
	(* cycle 0 *)
	IF r1   iadd r0 r10 -> r6,                      
	IF r1   iadd r0 r9 -> r33,                      
	IF r1   ineqi(0) r5 -> r7,                      
	IF r1   ieqli(0) r5 -> r8,                      
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 1 *)
	IF r7   ijmpt r1 r12,                           
	IF r1   imm32(____internal_clone_DT_3) -> r35,  
	IF r8   ijmpt r1 r6,                            
	IF r7   ld32d(16) r4 -> r11,                    
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 2 *)
	IF r8   iadd r0 r35 -> r2,                      
	IF r8   iadd r0 r33 -> r5,                      
	IF r8   iadd r0 r0 -> r11,                      
	IF r7   ld32d(20) r4 -> r34,                    
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 3 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r7   ld32d(8) r4 -> r9,                      
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 4 *)
	IF r7   iaddi(0x18) r4 -> r4,                   
	IF r1   nop,
	IF r1   nop,
	IF r7   ld32d(12) r4 -> r10,                   
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 5 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 6 *)
	IF r7   iadd r0 r34 -> r12,                   
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

____internal_clone_DT_3:
	(* cycle 0 *)
	IF r1   ijmpi(__exit),                          
	IF r1   imm32(____internal_clone_DT_4) -> r2,  
	IF r1   nop,
	IF r1   nop,
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
	IF r1   nop,
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
	IF r1   nop,
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

____internal_clone_DT_4:
	(* cycle 0 *)
	IF r1   ijmpt r1 r12,                           
	IF r1   iadd r0 r11 -> r5,                      
	IF r1   super_ld32d(16) r4 -> r11 r6,           
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 1 *)
	IF r1   iaddi(0x18) r4 -> r4,                   
	IF r1   nop,
	IF r1   super_ld32d(8) r4 -> r9 r10,            
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
	IF r1   iadd r0 r6 -> r12,                     
	IF r1   nop,
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
	IF r1   nop,
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
