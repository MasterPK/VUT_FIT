/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#include "symtable.h"
#include <stdio.h>

bool symtable_init(symtable *symtable)
{
	symtable->root = NULL;
	symtable->symbol_count = 0;

	return true;
}

bool symtable_free(symtable *symtable)
{
	BTDeleteTree(&symtable->root);
	symtable->root = NULL;
	symtable->symbol_count = 0;

	return true;
}

bool symtable_add(symtable *symtable, symkey key, symbol *sym)
{
	// TODO: pokud už existuje, nezvyšovat symbol_count
	++symtable->symbol_count;
	return BTInsertNode(&symtable->root, key, sym);
}

bool symtable_remove(symtable *symtable, symkey key)
{
	// TODO: pokud už existuje, nesnižovat symbol_count
	--symtable->symbol_count;
	BTDeleteNodeByKey(&symtable->root, key);

	return true;
}

symbol *symtable_find(symtable *symtable, symkey key)
{
	BTNodePtr node_ptr = BTSearchNodeByKeyPtr(&symtable->root, key);

	if (!node_ptr)
		return NULL;
	else
		return node_ptr->data;
}

static void print_func(void *sym);

void symtable_print(symtable *symtable)
{
	fprintf(stderr,"symtable\n");
	bt_print_inorder(symtable->root, &print_func);
	fprintf(stderr,"end of symtable\n");
}

static void print_func(void *_sym)
{
	symbol *sym = (symbol*)_sym;
	fprintf(stderr,"  %s - ", sym->id);
	fprintf(stderr,"%s - ", sym->defined ? "defined" : "undefined");

	const char* type_str;
	switch(sym->type)
	{
		case SYM_NONE: type_str = "type:undefined"; break;
		case SYM_INT: type_str = "type:int "; break;
		case SYM_FLOAT: type_str = "type:float "; break;
		case SYM_STRING: type_str = "type:string "; break;
		case SYM_FUNC: type_str = "type:function "; break;
	}
	fprintf(stderr,"%s - ", type_str);

	fprintf(stderr,"value:");
	switch(sym->type)
	{
		case SYM_NONE: fprintf(stderr,"undefined"); break;
		case SYM_INT: fprintf(stderr,"%d", sym->int_val); break;
		case SYM_FLOAT: fprintf(stderr,"%f", sym->float_val); break;
		case SYM_STRING: fprintf(stderr,"%s", sym->string_val); break;
		case SYM_FUNC: fprintf(stderr,"TODO"); break;
	}
	fprintf(stderr," - ");

	if (sym->frame == SYM_GF)
		fprintf(stderr,"global frame");
	else if (sym->frame == SYM_LF)
		fprintf(stderr,"local frame");
	else
		fprintf(stderr,"temporary frame");

	fprintf(stderr,"\n");
}

symbol symbol_from_token(token *t)
{
	symbol sym = {.id=NULL, .immediate = true};

	switch(t->type)
	{
		case TOKEN_INT:
			sym.type = SYM_INT;
			sym.int_val = t->int_val;
			break;
		case TOKEN_FLOAT:
			sym.type = SYM_FLOAT;
			sym.float_val = t->float_val;
			break;
		case TOKEN_STRING:
			sym.type = SYM_STRING;
			sym.string_val = t->string_val;
			break;
		case TOKEN_IDENTIFIER:
			sym.type = SYM_NONE;
			sym.id = t->string_val;
			sym.immediate = false;
			break;
		default:
			sym.type = SYM_NONE;
	}

	return sym;
}
