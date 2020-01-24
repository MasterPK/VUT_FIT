/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "token.h"
#include "symtable.h"

typedef enum expr_symbol_type_t {
	// Identifier
	ES_ID,

	// Literals
	ES_NIL,
	ES_INT,
	ES_FLOAT,
	ES_STRING,

	// IMPORTANT NOTE: Don't change the order of these enumerations (operators)
	// Arithmetic operators
	ES_PLUS,
	ES_MINUS,
	ES_MUL,
	ES_DIV,
	ES_LB,
	ES_RB,

	// Relational operatos
	ES_EQ, // equality			==
	ES_NE, // not equal			!=
	ES_LT, // less than			<
	ES_GT, // greater than		>
	ES_LE, // less or equal		<=
	ES_GE, // greater or equal	>=

	// End symbol
	ES_END,

	// Non-terminal (rule E)
	ES_NT,

	// Left stop, right stop
	ES_LS, // < (in the context of prec. analysis algorithm, not the same as "less than")
	ES_RS, // > (in the context of prec. analysis algorithm, not the same as "greater than")

	// Error
	ES_ERROR
} expr_symbol_type;

typedef struct expr_symbol_t {
	expr_symbol_type type;
	union {
		int int_val;
		double float_val;
		char *string_val; // used also for identifiers
	};
} expr_symbol;

// Returns analysis' error code
// NOTE: in an interpreted program, expression's result will always be on top of the data stack
int expression_analysis(symtable *current_table);

#endif
