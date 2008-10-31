#include <asm/linkage.h>
#include <asm/regdef.h>
#include <asm/asm-offsets.h>

ENTRY(__syscall)
	(* cycle 0 *)
	IF r1   iadd r0 r6 -> REG_SYSCALL_ARG1,                      
	IF r1   uimm(SYMBOL_NAME(__syscall)) -> REG_temp,                
	IF r1   iadd r0 r8 -> REG_SYSCALL_ARG3,                      
	IF r1   ld32d(24) r4 -> REG_SYSCALL_ARG6,                    
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 1 *)
	IF r1   writespc REG_temp,                           
	IF r1   iadd r0 r7 -> REG_SYSCALL_ARG2,                      
	IF r1   iadd r0 r5 -> REG_SYSCALL_NR,                      
	IF r1   ld32d(20) r4 -> REG_SYSCALL_ARG5,                    
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 2 *)
	IF r1   nop,
	IF r1   nop,                            
	IF r1   nop,
	IF r1   ld32d(16) r4 -> REG_SYSCALL_ARG4,                    
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 0 *)
	IF r1   imm32(0x400) -> REG_temp1,                     
	IF r1   iaddi(0x2) r0 -> REG_temp2,                    
	IF r1   imm32(__MMIO_base_init+1050656) -> REG_temp3,  
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 1 *)
	IF r1   writepcsw r0 REG_temp1,                        
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
	IF r1   h_st32d(0) REG_temp2 REG_temp3,                     
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 5 *)
	IF r1   writepcsw REG_temp1 REG_temp1,                     
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
	IF r1   ijmpt r1 r2,                        
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 9 *)
	IF r1   nop,
	IF r1   nop,
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
	IF r1   nop,
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
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;
