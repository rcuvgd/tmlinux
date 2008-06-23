/*
 * bsearch.c
 */

#include <stdlib.h>

void *bsearch(const void *key, const void *base, size_t nmemb,
	      size_t size, int (*cmp) (const void *, const void *))
{
	while (nmemb) {
		size_t mididx = nmemb / 2;
#ifndef __TCS__
		const void *midobj = base + mididx * size;
#else
		const unsigned char* midobj = (unsigned char*)base + mididx * size;
#endif 
		int diff = cmp(key, midobj);

		if (diff == 0)
			return (void *)midobj;

		if (diff > 0) {
			base = midobj + size;
			nmemb -= mididx + 1;
		} else
			nmemb = mididx;
	}

	return NULL;
}
