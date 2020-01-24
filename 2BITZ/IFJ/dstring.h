/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef DSTRING_H
#define DSTRING_H
#include <stdbool.h>
#include <string.h>

typedef struct dstring_t {
	size_t cap, len;
	char *data;
} dstring;

bool dstring_init(dstring *str);
bool dstring_free(dstring *str);
bool dstring_add(dstring *dst, const char *src);
bool dstring_add_n(dstring *dst, const char *src, size_t n);
bool dstring_add_char(dstring *dst, char c);
bool dstring_add_dstring(dstring *dst, dstring *src);

#endif
