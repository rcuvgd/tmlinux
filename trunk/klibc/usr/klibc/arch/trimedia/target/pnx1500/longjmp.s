#include <asm/linkage.h>
#include <asm/regdef.h>

ENTRY(longjmp)
	(* cycle 0 *)
	IF r1   isubi(0x1) r0 -> REG_temp5,                  
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(20) r5 -> REG_temp1,                   
	IF r1   ld32d(12) r5 -> REG_temp2;                   

	(* cycle 1 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(16) r5 -> REG_temp3,                   
	IF r1   ld32d(0) r5 -> REG_temp4;                    

	(* cycle 2 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(4) r5 -> r3,                    
	IF r1   ld32d(8) r5 -> r4;                    

	(* cycle 3 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   writepcsw REG_temp1 REG_temp5,                    
	IF r1   nop,
	IF r1   nop;

	(* cycle 4 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(28) r5 -> r7;                   

	(* cycle 5 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(36) r5 -> r9,                   
	IF r1   nop;                   

	(* cycle 6 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(44) r5 -> r11,                 
	IF r1   ld32d(32) r5 -> r8;                  

	(* cycle 7 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(52) r5 -> r13,                 
	IF r1   ld32d(40) r5 -> r10;                 

	(* cycle 8 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(60) r5 -> r15,                 
	IF r1   ld32d(48) r5 -> r12;                 

	(* cycle 9 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(68) r5 -> r17,                 
	IF r1   ld32d(56) r5 -> r14;                 

	(* cycle 10 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(76) r5 -> r19,                 
	IF r1   ld32d(64) r5 -> r16;                

	(* cycle 11 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(84) r5 -> r21,                
	IF r1   ld32d(72) r5 -> r18;                

	(* cycle 12 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(92) r5 -> r23,                
	IF r1   ld32d(80) r5 -> r20;                

	(* cycle 13 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(100) r5 -> r25,               
	IF r1   ld32d(88) r5 -> r22;                

	(* cycle 14 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   writespc REG_temp3,                       
	IF r1   ld32d(108) r5 -> r27,               
	IF r1   ld32d(96) r5 -> r24;               

	(* cycle 15 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   writedpc REG_temp2,                      
	IF r1   ld32d(116) r5 -> r29,              
	IF r1   ld32d(104) r5 -> r26;              

	(* cycle 16 *)
	IF r1   nop,
	IF r1   nop,                       
	IF r1   nop,
	IF r1   ld32d(124) r5 -> r31,              
	IF r1   ld32d(112) r5 -> r28;              

	(* cycle 17 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   ld32d(128) r5 -> r32,              
	IF r1   ld32d(120) r5 -> r30;              

	(* cycle 18 *)
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 19 *)
	IF r1   iadd r0 REG_temp4 -> r2,                
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* To wait r2 become valid*)
	nop,nop,nop,nop,nop;
	nop,nop,nop,nop,nop;
	nop,nop,nop,nop,nop;

	(* cycle 0 *)
	IF r1   ieqli(0) r6 -> REG_temp1, 
	IF r1   ijmpt r1 r2,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;

	(* cycle 1 *)
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
	IF r1   nop;

	(* cycle 3 *)
	IF r1   iadd REG_temp1 r6 -> r5,          
	IF r1   nop,
	IF r1   nop,
	IF r1   nop,
	IF r1   nop;
