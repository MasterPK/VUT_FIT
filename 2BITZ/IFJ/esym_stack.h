#ifndef ESYM_STACK_H
#define ESYM_STACK_H
#include "expression.h"
#include <stdbool.h>

// NOTE: does not need its own test program, has almost the same
// implementation as dstring_stack (differs in push function)

typedef struct esym_stack_node_t {
	expr_symbol data;
	struct esym_stack_node_t *next;
} esym_stack_node;

typedef struct esym_stack_t {
	esym_stack_node *top;
} esym_stack;

void esym_stack_init(esym_stack *st);
bool esym_stack_empty(esym_stack *st);
bool esym_stack_push(esym_stack *st, expr_symbol *esym);
void esym_stack_pop(esym_stack *st);
expr_symbol *esym_stack_top_data(esym_stack *st);

// Special functions
bool esym_stack_insert_after_top_terminal(esym_stack *st, expr_symbol *esym);
expr_symbol *esym_stack_top_terminal_data(esym_stack *st);

#endif
