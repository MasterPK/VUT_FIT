/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#include "esym_stack.h"
#include <stdlib.h>
#include <assert.h>

void esym_stack_init(esym_stack *st)
{
	st->top = NULL;
}

bool esym_stack_empty(esym_stack *st)
{
	return st->top == NULL;
}

bool esym_stack_push(esym_stack *st, expr_symbol *esym)
{
	esym_stack_node *new_node = (esym_stack_node*)malloc(sizeof(esym_stack_node));
	if (!new_node)
		return false;

	// Initialize esym
	new_node->data = *esym;

	// Put new_node on top of the stack
	esym_stack_node *tmp = st->top;
	st->top = new_node;
	new_node->next = tmp;

	return true;
}

void esym_stack_pop(esym_stack *st)
{
	esym_stack_node *tmp = st->top;

	// TODO: možná bude potřeba uvolnit string_val, podle způsobu použití

	// Remove dstring_node from stack
	st->top = st->top->next;
	free(tmp);
}

expr_symbol* esym_stack_top_data(esym_stack *st)
{
	return &st->top->data;
}

bool esym_stack_insert_after_top_terminal(esym_stack *st, expr_symbol *esym)
{
	esym_stack_node *node = st->top;
	if (node == NULL)
		return false;

	// Non-terminal on top of the stack
	if (node->data.type != ES_NT)
		return esym_stack_push(st, esym);

	// Non-terminal inside of the stack
	while (node->next != NULL) // note: || == ES_LS || == ES_RS ... probably not necessary
	{
		if (node->next->data.type != ES_NT)
		{
			esym_stack_node *new_node = (esym_stack_node*)malloc(sizeof(esym_stack_node));
			if (!new_node)
				return false;

			// Initialize esym
			new_node->data = *esym;

			// Insert symbol
			new_node->next = node->next;
			node->next = new_node;

			return true;
		}
		else
			node = node->next;
	}

	return node->next != NULL;
}

expr_symbol *esym_stack_top_terminal_data(esym_stack *st)
{
	esym_stack_node *node = st->top;
	while (node != NULL && node->data.type == ES_NT) // note: || == ES_LS || == ES_RS ... probably not necessary
		node = node->next;

	assert(node != NULL); // TODO: smazat, prozatím jistější

	if (node == NULL)
		return NULL;

	return &node->data;
}
