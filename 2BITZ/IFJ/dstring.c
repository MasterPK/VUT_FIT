/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#include "dstring.h"
#include <stdlib.h>
#include <assert.h>

#define EXPAND_BY_CONST 1024

bool dstring_init(dstring *str)
{
	assert(str != NULL);

	str->cap = str->len = 0;
	str->data = NULL;

	return true;
}

bool dstring_free(dstring *str)
{
	assert(str != NULL);

	free(str->data);

	str->cap = str->len = 0;
	str->data = NULL;

	return true;
}

bool dstring_add(dstring *dst, const char *src)
{
	assert(dst != NULL);
	assert(src != NULL);

	return dstring_add_n(dst, src, strlen(src));
}

bool dstring_add_dstring(dstring *dst, dstring *src)
{
	assert(dst != NULL);
	assert(src != NULL);

	return dstring_add_n(dst, src->data, strlen(src->data));
}

bool dstring_add_char(dstring *dst, char c)
{
	assert(dst != NULL);

	return dstring_add_n(dst, &c, 1);
}

bool dstring_add_n(dstring *dst, const char *src, size_t n)
{
	assert(dst != NULL);
	assert(src != NULL);

	// Expand dstring if needed
	if (dst->len + n + 1 > dst->cap)
	{
		// Currently expands to the closest multiple of EXPAND_BY_CONST larger that the size needed
		size_t new_cap = dst->len + (n / EXPAND_BY_CONST + 1) * EXPAND_BY_CONST;
		dst->data = realloc(dst->data, new_cap);
		if (!dst->data)
			return false;

		dst->cap = new_cap;
	}

	// Copy src into dst
	strncpy(&dst->data[dst->len], src, n);
	dst->data[dst->len + n] = '\0';
	dst->len += n;

	return true;
}

