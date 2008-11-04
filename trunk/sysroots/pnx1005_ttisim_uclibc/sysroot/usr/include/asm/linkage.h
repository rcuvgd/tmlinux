#ifndef _ASM_TRIMEDIA_LINKAGE_H
#define _ASM_TRIMEDIA_LINKAGE_H

#undef SYMBOL_NAME
#undef ENTRY
#undef GLOBAL_DATA

#define SYMBOL_NAME(name) 	_##name

#define ENTRY(name)         		\
		.##text  		\
	        .##global _##name       \
        _##name:

#define GLOBAL_DATA(name) 		\
		.##data 		\
		.##align 4 		\
		.##global _##name    	\
	_##name:

#endif /* _ASM_TRIMEDIA_LINKAGE_H */
