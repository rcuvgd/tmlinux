/*
 * alloca.h
 *
 * Just call the builtin alloca() function
 */

#ifndef _ALLOCA_H
#define _ALLOCA_H

extern void* __builtin_alloca(unsigned long size);
#define alloca(size) __builtin_alloca(size)

#endif				/* _ALLOCA_H */
