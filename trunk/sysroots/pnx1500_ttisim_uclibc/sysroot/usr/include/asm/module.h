/*
 * FIXME: TriMedia do not know how to implement kernel module.
 */
#ifndef _ASM_TRIMEDIA_MODULE_H
#define _ASM_TRIMEDIA_MODULE_H

struct mod_arch_specific { char __dummy; };
#define Elf_Shdr Elf32_Shdr
#define Elf_Sym Elf32_Sym
#define Elf_Ehdr Elf32_Ehdr

#endif /* _ASM_TRIMEDIA_MODULE_H */

