/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef DSTRING_STACK_H
#define DSTRING_STACK_H
#include "dstring.h"

typedef struct dstring_stack_node_t {
	dstring data;
	struct dstring_stack_node_t *next;
} dstring_stack_node;

typedef struct dstring_stack_t {
	dstring_stack_node *top;
} dstring_stack;

void dstring_stack_init(dstring_stack *st);
bool dstring_stack_empty(dstring_stack *st);
bool dstring_stack_push_new(dstring_stack *st);
void dstring_stack_pop(dstring_stack *st);
dstring* dstring_stack_top_data(dstring_stack *st);

#endif
