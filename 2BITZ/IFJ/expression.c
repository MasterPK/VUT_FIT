/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#include "expression.h"
#include "esym_stack.h"
#include "parser.h" // used for get_next_token() and token_tmp
#include "code_gen.h"
#include "errors.h"

// These two symbols are used internally for expression evaluation
static symbol expr_result_sym = {.id="$expr_result", .frame=SYM_GF, .immediate=false};
static symbol expr_result_type_sym = {.id="$expr_result_type", .frame=SYM_GF, .immediate=false};
static symbol expr_a_sym = {.id="$expr_a", .frame=SYM_GF, .immediate=false};
static symbol expr_b_sym = {.id="$expr_b", .frame=SYM_GF, .immediate=false};

// These two numbers are used to generate unique code labels
static unsigned expression_id = ~0;
static unsigned subexpression_id = 0;

// This string holds a unique number made of expression_id and subexpression_id
static char unique_id_str[64];

typedef enum prec_table_rule_t {
	X,	// x ... error
	S,	// < ... shift
	R,	// > ... reduce
	E,	// = ... equal
} prec_table_rule;

typedef enum prec_table_input_t {
	PTI_PLUS_MINUS,	// + -
	PTI_MUL_DIV,	// * /
	PTI_LB,			// (
	PTI_RB,			// )
	PTI_ID,			// identifier
	PTI_REL,		// <, >, <=, >=, ==, !=
	PTI_END,		// $
	PTI_ERROR,
} prec_table_input;

// Precedence table:
//   +- */ ( ) i r $
// +- >  < < > < > >
// */ >  > < > < > >
// (  <  < < = < < x
// )  >  > x > x > >
// i  >  > x > x > >
// r  <  < < > < x >
// $  <  < < x < < x

#define PREC_TABLE_WIDTH 7
static prec_table_rule prec_table[PREC_TABLE_WIDTH][PREC_TABLE_WIDTH] = {
	{R, S, S, R, S, R, R},
	{R, R, S, R, S, R, R},
	{S, S, S, E, S, S, X},
	{R, R, X, R, X, R, R},
	{R, R, X, R, X, R, R},
	{S, S, S, R, S, X, R},
	{S, S, S, X, S, S, X}
};

// Converts token to expression symbol, then returns it
static expr_symbol expr_symbol_from_token(token *t);

// Converts expression symbol to precedence table input, then returns it
static prec_table_input pti_from_esym(expr_symbol *esym);

// Pops symbols from esym stack, return the rule they match (if they match any)
static int handle_expression_rule(esym_stack *st, symtable *current_symtable);

int expression_analysis(symtable *current_table)
{
	++expression_id;
	subexpression_id = 0;

	esym_stack st;
	esym_stack_init(&st);

	// Push '$' to the stack
	// note: { ... } used to keep the scope clean from tmp_sym
	{
		expr_symbol tmp_sym = {.type = ES_END};
		esym_stack_push(&st, &tmp_sym);
	}

	// Convert first token to expression symbol
	expr_symbol esym = expr_symbol_from_token(&token_tmp);

	while (esym.type != ES_ERROR && !(esym.type == ES_END && esym_stack_top_terminal_data(&st)->type == ES_END))
	{
		// Convert symbols to indices into precedence table
		prec_table_input stack_pti = pti_from_esym(esym_stack_top_terminal_data(&st));
		prec_table_input input_pti = pti_from_esym(&esym);

		// Find precedence table rule (shift/reduce/...) given the top of the stack and an input symbol
		prec_table_rule rule = prec_table[stack_pti][input_pti];

		// Error or end
		if (rule == X)
		{
			// TODO: tímto si nejsem jistý - zkontrolovat
			if (stack_pti == PTI_END && input_pti == PTI_END)
				RETURN_ERROR(NO_ERR);
			RETURN_ERROR(SYNTAX_ERR);
		}
		// Shift rule
		else if (rule == S)
		{
			// TODO: kontroly návratových hodnot
			expr_symbol left_stop_esym = {.type = ES_LS};
			esym_stack_insert_after_top_terminal(&st, &left_stop_esym);
			esym_stack_push(&st, &esym);
		}
		// Reduce rule
		else if (rule == R)
		{
			// Find the appropriate rule (e.g. E -> E+E  or  E -> i)
			int ret = handle_expression_rule(&st, current_table);

			// If there was an error, exit analysis
			if (ret != NO_ERR)
				return ret;

			continue; // MUST BE HERE - because this rule shouldn't read next input symbol
		}
		// Equal rule
		else if (rule == E)
		{
			esym_stack_push(&st, &esym);
		}

		// Load next token and covert it to expression symbol
		get_next_token();
		esym = expr_symbol_from_token(&token_tmp);
	}

	// Save expression's result into GF@$expr_result
	gen_pop(&expr_result_sym);

	// Save expression's type into GF@$expr_result_type (used in if/while/...)
	if (!gen_type(&expr_result_type_sym, &expr_result_sym))
		RETURN_ERROR(PROG_ERR);

	RETURN_ERROR(esym.type != ES_ERROR ? NO_ERR : SYNTAX_ERR);
}

static expr_symbol expr_symbol_from_token(token *t)
{
	expr_symbol esym;

	switch (t->type)
	{
		case TOKEN_IDENTIFIER:
			esym.type = ES_ID;
			esym.string_val = t->string_val;
			break;

		case TOKEN_INT:
			esym.type = ES_INT;
			esym.int_val = t->int_val;
			break;
		case TOKEN_FLOAT:
			esym.type = ES_FLOAT;
			esym.float_val = t->float_val;
			break;
		case TOKEN_STRING:
			esym.type = ES_STRING;
			esym.string_val = t->string_val;
			break;

		case TOKEN_PLUS:
			esym.type = ES_PLUS;
			break;
		case TOKEN_MINUS:
			esym.type = ES_MINUS;
			break;
		case TOKEN_MUL:
			esym.type = ES_MUL;
			break;
		case TOKEN_DIV:
			esym.type = ES_DIV;
			break;

		case TOKEN_LB:
			esym.type = ES_LB;
			break;
		case TOKEN_RB:
			esym.type = ES_RB;
			break;

		case TOKEN_EOL:
		case TOKEN_EOF:
			esym.type = ES_END;
			break;

		case TOKEN_KEYWORD:
			if (t->keyword_val == KW_THEN || t->keyword_val == KW_DO || t->keyword_val == KW_ELSE)
				esym.type = ES_END;
			else if (t->keyword_val == KW_NIL)
				esym.type = ES_NIL;
			else
				esym.type = ES_ERROR;
			break;

		default:
			esym.type = ES_ERROR;
	}

	// Relational operators
	if (t->type >= TOKEN_EQ && t->type <= TOKEN_GE)
		esym.type = ES_EQ + (t->type - TOKEN_EQ);

	return esym;
}

static prec_table_input pti_from_esym(expr_symbol *esym)
{
	switch (esym->type)
	{
		case ES_ID:
		case ES_NIL:
		case ES_INT:
		case ES_FLOAT:
		case ES_STRING:
			return PTI_ID;

		case ES_PLUS:
		case ES_MINUS:
			return PTI_PLUS_MINUS;

		case ES_MUL:
		case ES_DIV:
			return PTI_MUL_DIV;

		case ES_EQ:
		case ES_NE:
		case ES_LT:
		case ES_GT:
		case ES_LE:
		case ES_GE:
			return PTI_REL;

		case ES_LB:
			return PTI_LB;

		case ES_RB:
			return PTI_RB;

		case ES_END:
			return PTI_END;

		default:
			return PTI_ERROR;
	}

	return PTI_ERROR;
}

static int handle_expression_rule(esym_stack *st, symtable *current_table)
{
	expr_symbol esym1 = *esym_stack_top_data(st);
	esym_stack_pop(st);

	// IMPORTANT: Back to front analysis (popping from stack)
	// Example:
	// stack = $<E+<E*E
	// first handled symbol will be the last E, then +, then E, then left stop (<)

	// 1) Rule E -> (E)
	if (esym1.type == ES_RB)
	{
		expr_symbol esym2 = *esym_stack_top_data(st);
		esym_stack_pop(st);
		if (esym2.type != ES_NT)
			RETURN_ERROR(SYNTAX_ERR);

		expr_symbol esym3 = *esym_stack_top_data(st);
		esym_stack_pop(st);
		if (esym3.type != ES_LB)
			RETURN_ERROR(SYNTAX_ERR);

		expr_symbol esym4 = *esym_stack_top_data(st);
		esym_stack_pop(st);
		if (esym4.type != ES_LS)
			RETURN_ERROR(SYNTAX_ERR);

		// Generate code and exit this function
		// RULE: E -> (E)
		// NOTE: this rule doesn't generate any code?

		// Add non-terminal onto stack
		expr_symbol nt_symbol = {.type=ES_NT};
		esym_stack_push(st, &nt_symbol);

		RETURN_ERROR(NO_ERR);
	}

	// 2) Rules E -> i and E -> E op E:

	// Last symbol must always be identifier/literal/non-terminal
	if (esym1.type != ES_ID && esym1.type != ES_INT && esym1.type != ES_FLOAT &&
		esym1.type != ES_STRING && esym1.type != ES_NIL && esym1.type != ES_NT)
		RETURN_ERROR(SYNTAX_ERR);

	// This symbol is either an operator (rule E -> E+E, this symbol would represent the +) or left stop (rule E -> i)
	expr_symbol esym2 = *esym_stack_top_data(st);
	esym_stack_pop(st);
	if (esym2.type == ES_LS)
	{
		// Must have been identifier/literal, can't be non-terminal
		if (esym1.type == ES_NT)
			RETURN_ERROR(SYNTAX_ERR);

		// Generate code and exit this function
		// RULE: E -> i

		// 1) Is symbol immediate? If so, generate code, then exit function
		// 2) If non-immediate, find it in symtable, if not found, error exit
		// 3) If found, set sym->frame to SYM_LF/SYM_GF depending on the table that it was found in
		// 4) Generate code

		// 1) If immediate symbol, generate code
		if (esym1.type != ES_ID)
		{
			symbol imm_sym = {.immediate=true};
			switch (esym1.type)
			{
				case ES_NIL:
					imm_sym.type = SYM_NONE;
					break;

				case ES_INT:
					imm_sym.type = SYM_INT;
					imm_sym.int_val = esym1.int_val;
					break;

				case ES_FLOAT:
					imm_sym.type = SYM_FLOAT;
					imm_sym.float_val = esym1.float_val;
					break;

				case ES_STRING:
					imm_sym.type = SYM_STRING;
					imm_sym.string_val = esym1.string_val;
					break;
				default:
					RETURN_ERROR(SYNTAX_ERR);
			}

			// Add non-terminal onto stack
			expr_symbol nt_symbol = {.type=ES_NT};
			esym_stack_push(st, &nt_symbol);

			gen_push(&imm_sym);
			RETURN_ERROR(NO_ERR);
		}

		// 2,3) Find in symtable(s), set SYM_LF/SYM_GF
		//symtable_print(current_table);
		symbol *sym = symtable_find(current_table, esym1.string_val); // look into local symtable
		if (!sym)
		{
			RETURN_ERROR(SEM_DEF_ERR); // undefined symbol

			// TODO: zakomentováno, protože v globální tabulce by se neměly nacházet žádné proměnné
			/*sym = symtable_find(&table_global, esym1->string_val); // look into local symtable
			if (!sym)
				RETURN_ERROR(SEM_DEF_ERR); // undefined symbol
			sym->frame = SYM_GF;*/
		}
		else
			sym->frame = SYM_LF;

		// Add non-terminal onto stack
		expr_symbol nt_symbol = {.type=ES_NT};
		esym_stack_push(st, &nt_symbol);

		// 4) Generate code
		gen_push(sym);

		RETURN_ERROR(NO_ERR);
	}
	else if (esym2.type < ES_PLUS || esym2.type > ES_GE)
		RETURN_ERROR(SYNTAX_ERR);

	// This symbol must be identifier/literal (rule E -> E+E, this symbol would represent the first E)
	expr_symbol esym3 = *esym_stack_top_data(st);
	esym_stack_pop(st);
	if (esym3.type != ES_NT)
		RETURN_ERROR(SYNTAX_ERR);

	// This symbol must be left stop
	expr_symbol esym4 = *esym_stack_top_data(st);
	esym_stack_pop(st);
	if (esym4.type != ES_LS)
		RETURN_ERROR(SYNTAX_ERR);

	// Generate code and exit this function
	// RULE: E -> E op E

	// Create new unique string used for expression's code labels
	snprintf(unique_id_str, 64, "%d_%d", expression_id, subexpression_id);
	++subexpression_id;

	// Pop operands into GF@$expr_a and GF@$expr_b so that their types can be checked
	gen_pop(&expr_b_sym);
	gen_pop(&expr_a_sym);
	// Convert operands if possible, exit program otherwise
	gen_call_prepare();
	gen_call("$conv_ops");

	// If the operator is not == or !=, exit if operands are incompatible
	if (esym2.type != ES_EQ && esym2.type != ES_NE)
		gen_exit_if_incompatible_or_nil(unique_id_str);

	switch (esym2.type)
	{
		case ES_PLUS:
			gen_add(unique_id_str);
			break;
		case ES_MINUS:
			gen_sub(unique_id_str);
			break;
		case ES_MUL:
			gen_mul(unique_id_str);
			break;
		case ES_DIV:
			gen_div(unique_id_str);
			break;
		case ES_EQ:
			gen_eq(unique_id_str);
			break;
		case ES_NE:
			gen_eq(unique_id_str);
			gen_not();
			break;
		case ES_LT:
			gen_lt();
			break;
		case ES_GT:
			gen_gt();
			break;
		case ES_LE:
			gen_gt();
			gen_not();
			break;
		case ES_GE:
			gen_lt();
			gen_not();
			break;
		default:
			RETURN_ERROR(SYNTAX_ERR);
	}

	// Add non-terminal onto stack
	expr_symbol nt_symbol = {.type=ES_NT};
	esym_stack_push(st, &nt_symbol);

	RETURN_ERROR(NO_ERR);
}
