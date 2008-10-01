/*
 * This file lifted in toto from 'Dlibs' on the atari ST  (RdeBath)
 *
 * 
 *    Dale Schumacher                         399 Beacon Ave.
 *    (alias: Dalnefre')                      St. Paul, MN  55104
 *    dal@syntel.UUCP                         United States of America
 *  "It's not reality that's important, but how you perceive things."
 */

#include <string.h>
#include <stdio.h>
#include <search.h>

#ifdef L_lfind

void *lfind(const void *key, const void *base, size_t *nmemb,
	size_t size, int (*compar)(const void *, const void *))
{
	register int n = *nmemb;

	while (n--) {
		if ((*compar) (base, key) == 0)
			return ((void*)base);
#ifndef __TCS__
		base += size;
#else
		base = (void*)((unsigned char*)base + size);
#endif 
	}
	return (NULL);
}

#endif

#ifdef L_lsearch

void *lsearch(const void *key, void *base, size_t *nmemb, 
	size_t size, int (*compar)(const void *, const void *))
{
	register char *p;

	if ((p = lfind(key, base, nmemb, size, compar)) == NULL) {
#ifndef __TCS__
		p = __memcpy((base + (size * (*nmemb))), key, size);
#else
		p = __memcpy(((unsigned char*)base + (size * (*nmemb))), key, size);
#endif 
		++(*nmemb);
	}
	return (p);
}

#endif
