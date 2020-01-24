#include "dstring_stack.h"
#include <stdlib.h>

void dstring_stack_init(dstring_stack *st)
{
	st->top = NULL;
}

bool dstring_stack_empty(dstring_stack *st)
{
	return st->top == NULL;
}

bool dstring_stack_push_new(dstring_stack *st)
{
	dstring_stack_node *new_node = (dstring_stack_node*)malloc(sizeof(dstring_stack_node));
	if (!new_node)
		return false;

	// Initialize dstring
	dstring_init(&new_node->data);

	// Put new_node on top of the stack
	dstring_stack_node *tmp = st->top;
	st->top = new_node;
	new_node->next = tmp;

	return true;
}

void dstring_stack_pop(dstring_stack *st)
{
	dstring_stack_node *tmp = st->top;

	// Free dstring
	dstring_free(&tmp->data);

	// Remove dstring_node from stack
	st->top = st->top->next;
	free(tmp);
}

dstring* dstring_stack_top_data(dstring_stack *st)
{
	return &st->top->data;
}
