/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdbool.h>
#include "binary_tree.h"
#include "token.h"
#include "list.h"

typedef enum symbol_type_t {
	SYM_NONE,
	SYM_INT,
	SYM_FLOAT,
	SYM_STRING,
	SYM_FUNC
} symbol_type;

typedef enum symbol_frame_t {
	SYM_TF,
	SYM_LF,
	SYM_GF
} symbol_frame;

typedef struct symbol_t {
	char *id;
	bool defined;
	symbol_type type;
	int depth; // used by parser to remove symbols from local symtable after if/while scope ends

	union {
		int int_val;
		double float_val;
		char *string_val;
		List *params;
	};

	symbol_frame frame; // makes calls to code generator easier
	bool immediate;		// makes calls to code generator easier
} symbol;

typedef struct symtable_t {
	BTNodePtr root;
	int symbol_count;
} symtable;

// Key used to access symbols in symtable
typedef char* symkey;

bool symtable_init(symtable *symtable);
bool symtable_free(symtable *symtable);
bool symtable_add(symtable *symtable, symkey key, symbol *sym);
bool symtable_remove(symtable *symtable, symkey key);
symbol *symtable_find(symtable *symtable, symkey key);
void symtable_print(symtable *symtable);

// Returns a symbol constructed from token t
// Note: use only on literals (maybe?)
symbol symbol_from_token(token *t);

#endif
