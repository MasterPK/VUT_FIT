/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#include "code_gen.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

// Assumes a dstring variable named "program" in the current scope
#define ADD_CODE(src)	 				\
	do {								\
		if (!dstring_add(gen_target, src))\
			return false;				\
	} while (0)							\

// Assumes a dstring variable named "program" in the current scope
// Adds symbol's prefix to the code
#define ADD_FRAME_PREFIX(symbol)			\
	do {									\
		if (symbol->frame == SYM_GF)		\
			ADD_CODE("GF@");				\
		else if (symbol->frame == SYM_LF)	\
			ADD_CODE("LF@");				\
		else								\
			ADD_CODE("TF@");				\
	} while(0)

// Assumes a dstring variable named "program" in the current scope
#define ADD_SYMBOL(src)	 				\
	do {								\
		if (src->immediate)				\
		{								\
			if (!add_immediate_symbol(src))	\
				return false;			\
		}								\
		else							\
		{								\
			ADD_FRAME_PREFIX(src);		\
			ADD_CODE(src->id);			\
			ADD_CODE("\n");				\
		}								\
	} while (0)							\

// Stores program's code
dstring *gen_target;

static bool add_immediate_symbol(symbol *sym);

void gencontrol_redirect_output(dstring *new_target)
{
	gen_target = new_target;
}

bool gen_program_header()
{
	if (!dstring_init(gen_target))
		return false;

	ADD_CODE(".IFJcode18\n\n");
	ADD_CODE("# These global variables are used for expression evaluation\n");
	ADD_CODE("DEFVAR GF@$expr_a\n");
	ADD_CODE("DEFVAR GF@$expr_b\n");
	ADD_CODE("DEFVAR GF@$expr_result\n");
	ADD_CODE("DEFVAR GF@$expr_a_type\n");
	ADD_CODE("DEFVAR GF@$expr_b_type\n");
	ADD_CODE("DEFVAR GF@$expr_result_type\n");
	ADD_CODE("CREATEFRAME\n");
	ADD_CODE("PUSHFRAME\n");

	return true;
}

bool gen_builtin_functions()
{
	ADD_CODE("\n");
	ADD_CODE("# -------------------------------\n");
	ADD_CODE("# ----- BUILT-IN  FUNCTIONS -----\n");
	ADD_CODE("# -------------------------------\n");

	gen_function_start("inputs");
	ADD_CODE("READ GF@$expr_result string\n");
	gen_function_end("inputs");

	gen_function_start("inputi");
	ADD_CODE("READ GF@$expr_result int\n");
	gen_function_end("inputi");

	gen_function_start("inputf");
	ADD_CODE("READ GF@$expr_result float\n");
	gen_function_end("inputf");

	// length()
	gen_function_start("length");
	ADD_CODE("STRLEN GF@$expr_result LF@s\n");
	gen_function_end("length");

	// ord()
	gen_function_start("ord");
	ADD_CODE("DEFVAR LF@size\n");
	ADD_CODE("DEFVAR LF@comp_res\n");
	ADD_CODE("STRLEN LF@size LF@s\n");
	ADD_CODE("LT LF@comp_res LF@i LF@size\n");
	ADD_CODE("JUMPIFEQ $ord_nil LF@comp_res bool@false\n");
	ADD_CODE("GT LF@comp_res LF@i int@-1\n");
	ADD_CODE("JUMPIFEQ $ord_nil LF@comp_res bool@false\n");
	ADD_CODE("STRI2INT GF@$expr_result LF@s LF@i\n");
	ADD_CODE("JUMP $ord_end\n");
	ADD_CODE("LABEL $ord_nil\n");
	ADD_CODE("MOVE GF@$expr_result nil@nil\n");
	ADD_CODE("LABEL $ord_end\n");
	gen_function_end("ord");

	// substr()
	gen_function_start("substr");
	ADD_CODE("DEFVAR LF@comp_res\n");
	ADD_CODE("DEFVAR LF@len\n");
	ADD_CODE("STRLEN LF@len LF@s\n");

	ADD_CODE("# Test arguments\n");
	ADD_CODE("LT LF@comp_res LF@i int@0\n");
	ADD_CODE("JUMPIFEQ $substr_nil LF@comp_res bool@true\n");
	ADD_CODE("GT LF@comp_res LF@i LF@len\n");
	ADD_CODE("JUMPIFEQ $substr_nil LF@comp_res bool@true\n");
	ADD_CODE("LT LF@comp_res LF@n int@0\n");
	ADD_CODE("JUMPIFEQ $substr_nil LF@comp_res bool@true\n");
	ADD_CODE("JUMP $substr_ok\n");

	ADD_CODE("# Arguments lead to nil@nil result\n");
	ADD_CODE("LABEL $substr_nil\n");
	ADD_CODE("MOVE GF@$expr_result nil@nil\n");
	ADD_CODE("RETURN\n");

	ADD_CODE("# Arguments are fine\n");
	ADD_CODE("LABEL $substr_ok\n");
	ADD_CODE("ADD LF@n LF@n LF@i\n");
	ADD_CODE("GT LF@comp_res LF@n LF@len\n");
	ADD_CODE("JUMPIFEQ $substr_skip_move_n_len LF@comp_res bool@false\n");
	ADD_CODE("MOVE LF@n LF@len\n");

	ADD_CODE("LABEL $substr_skip_move_n_len\n");
	ADD_CODE("MOVE GF@$expr_result string@\n");

	ADD_CODE("DEFVAR LF@char\n");

	ADD_CODE("# Cycle\n");
	ADD_CODE("LABEL $substr_while\n");
	ADD_CODE("EQ LF@comp_res LF@i LF@n\n");
	ADD_CODE("JUMPIFEQ $substr_end_while LF@comp_res bool@true\n");

	ADD_CODE("# Cycle's body - concatenate string with one character\n");
	ADD_CODE("GETCHAR LF@char LF@s LF@i\n");
	ADD_CODE("CONCAT GF@$expr_result GF@$expr_result LF@char\n");
	ADD_CODE("ADD LF@i LF@i int@1\n");
	ADD_CODE("JUMP $substr_while\n");
	ADD_CODE("LABEL $substr_end_while\n\n");
	gen_function_end("substr");

	// chr()
	gen_function_start("chr");
	ADD_CODE("DEFVAR LF@comp_res\n");
	ADD_CODE("LT LF@comp_res LF@i int@0\n");
	ADD_CODE("JUMPIFEQ $chr_err LF@comp_res bool@true\n");
	ADD_CODE("GT LF@comp_res LF@i int@255\n");
	ADD_CODE("JUMPIFEQ $chr_err LF@comp_res bool@true\n");
	ADD_CODE("INT2CHAR GF@$expr_result LF@i\n");
	ADD_CODE("JUMP $chr_end\n");
	ADD_CODE("LABEL $chr_err\n");
	ADD_CODE("EXIT int@4\n");
	ADD_CODE("LABEL $chr_end\n");
	gen_function_end("chr");

	// Converts both stack operands to correct type or exits the program
	// NOTE: Returns 0 if both int, 1 if both float, 2 if both string, 3 if both nil
	ADD_CODE("# This built-in function either successfully converts operands, or it exits the program");
	gen_function_start("$conv_ops");
	const char *conv_ops_body =
	"TYPE GF@$expr_a_type GF@$expr_a\n"
	"TYPE GF@$expr_b_type GF@$expr_b\n"
	"JUMPIFEQ $conv_ops_i GF@$expr_a_type string@int\n"
	"JUMPIFEQ $conv_ops_f GF@$expr_a_type string@float\n"
	"JUMPIFEQ $conv_ops_s GF@$expr_a_type string@string\n"
	"JUMPIFEQ $conv_ops_n GF@$expr_a_type string@nil\n"

	"\n# Jump according to the type of the second operand\n"
	"LABEL $conv_ops_i\n"
	"JUMPIFEQ $conv_ops_ii GF@$expr_b_type string@int\n"
	"JUMPIFEQ $conv_ops_if GF@$expr_b_type string@float\n"
	//"EXIT int@4\n"
	"JUMP $conv_ops_xy\n"
	"LABEL $conv_ops_f\n"
	"JUMPIFEQ $conv_ops_fi GF@$expr_b_type string@int\n"
	"JUMPIFEQ $conv_ops_ff GF@$expr_b_type string@float\n"
	//"EXIT int@4\n"
	"JUMP $conv_ops_xy\n"
	"LABEL $conv_ops_s\n"
	"JUMPIFEQ $conv_ops_ss GF@$expr_b_type string@string\n"
	//"EXIT int@4\n"
	"JUMP $conv_ops_xy\n"
	"LABEL $conv_ops_n\n"
	"JUMPIFEQ $conv_ops_nn GF@$expr_b_type string@nil\n"
	//"EXIT int@4\n"
	"JUMP $conv_ops_xy\n"

	"\n# Convert and return\n"
	"LABEL $conv_ops_if\n"
	"PUSHS GF@$expr_a\n"
	"INT2FLOATS\n"
	"POPS GF@$expr_a\n"	// aktualizace $expr_a
	"PUSHS GF@$expr_a\n"	// návrat $expr_a na zásobník
	"PUSHS GF@$expr_b\n"
	"MOVE GF@$expr_result int@1\n"
	"JUMP $conv_ops_end\n"
	"LABEL $conv_ops_fi\n"
	"PUSHS GF@$expr_a\n"
	"PUSHS GF@$expr_b\n"
	"INT2FLOATS\n"
	"POPS GF@$expr_b\n"	// aktualizace $expr_b
	"PUSHS GF@$expr_b\n"	// návrat $expr_b na zásobník
	"MOVE GF@$expr_result int@1\n"
	"JUMP $conv_ops_end\n"

	"\n# Return\n"
	"LABEL $conv_ops_ii\n"	// int int
	"PUSHS GF@$expr_a\n"
	"PUSHS GF@$expr_b\n"
	"MOVE GF@$expr_result int@0\n"
	"JUMP $conv_ops_end\n"
	"LABEL $conv_ops_ff\n"	// float float
	"PUSHS GF@$expr_a\n"
	"PUSHS GF@$expr_b\n"
	"MOVE GF@$expr_result int@1\n"
	"JUMP $conv_ops_end\n"
	"LABEL $conv_ops_ss\n"	// string string
	"PUSHS GF@$expr_a\n"
	"PUSHS GF@$expr_b\n"
	"MOVE GF@$expr_result int@2\n"
	"JUMP $conv_ops_end\n"
	"LABEL $conv_ops_nn\n"	// nil nil
	"PUSHS GF@$expr_a\n"
	"PUSHS GF@$expr_b\n"
	"MOVE GF@$expr_result int@3\n"
	"JUMP $conv_ops_end\n"
	"LABEL $conv_ops_xy\n"	// incompatible
	"PUSHS GF@$expr_a\n"
	"PUSHS GF@$expr_b\n"
	"MOVE GF@$expr_result int@4\n"
	//"JUMP $conv_ops_end\n"
	"LABEL $conv_ops_end\n";
	ADD_CODE(conv_ops_body);
	gen_function_end("$conv_ops");

	// NOTE: print is handled separately in parser, no need to define it here

	ADD_CODE("# -------------------------------\n");
	ADD_CODE("# -- END OF BUILT-IN FUNCTIONS --\n");
	ADD_CODE("# -------------------------------\n");
	ADD_CODE("\n");

	return true;
}

bool gen_program_exit(int exit_code)
{
	char exit_code_str[32] = "0";
	snprintf(exit_code_str, 32, "%d", exit_code);

	ADD_CODE("EXIT int@");
	ADD_CODE(exit_code_str);
	ADD_CODE("\n");

	return true;
}

bool gen_function_start(const char *func_id)
{
	ADD_CODE("\n");
	// Don't execute the function unless it has been called [doc 4.2.]
	ADD_CODE("JUMP $jump_over_function_");
	ADD_CODE(func_id);
	ADD_CODE("\n");

	ADD_CODE("# Function \"");
	ADD_CODE(func_id);
	ADD_CODE("\"\n");

	ADD_CODE("LABEL ");
	ADD_CODE(func_id);
	ADD_CODE("\n");

	return true;
}

bool gen_function_end(const char *func_id)
{
	ADD_CODE("POPFRAME\n");
	ADD_CODE("RETURN\n");

	ADD_CODE("# End of function \"");
	ADD_CODE(func_id);
	ADD_CODE("\"\n");

	// Label used to jump over function's body definition
	ADD_CODE("LABEL $jump_over_function_");
	ADD_CODE(func_id);
	ADD_CODE("\n\n");

	return true;
}

bool gen_call_prepare()
{
	ADD_CODE("CREATEFRAME\n");

	return true;
}

bool gen_call_arg(char *arg_name, symbol *src)
{
	// Define variable to hold argument's value (on temporary frame)
	ADD_CODE("DEFVAR TF@");
	ADD_CODE(arg_name);
	ADD_CODE("\n");

	// Assign argument's value to variable
	symbol arg_sym = {.id=arg_name, .frame=SYM_TF, .immediate=false};
	gen_assignment(&arg_sym, src);

	return true;
}

bool gen_push(symbol *sym)
{
	ADD_CODE("PUSHS ");
	ADD_SYMBOL(sym);

	return true;
}

bool gen_pop(symbol *sym)
{
	assert(!sym->immediate);

	ADD_CODE("POPS ");
	ADD_SYMBOL(sym);

	return true;
}

bool gen_add(const char *unique_str)
{
	ADD_CODE("JUMPIFEQ $add_strings_");
	ADD_CODE(unique_str);
	ADD_CODE(" GF@$expr_result int@2\n");
	ADD_CODE("JUMP $add_nums_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("LABEL $add_strings_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("CONCAT GF@$expr_a GF@$expr_a GF@$expr_b\n");
	ADD_CODE("POPS GF@$expr_b\n");  // Remove strings from data stack
	ADD_CODE("POPS GF@$expr_b\n");  // Remove strings from data stack
	ADD_CODE("PUSHS GF@$expr_a\n"); // Push result onto the data stack
	ADD_CODE("JUMP $add_end_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("LABEL $add_nums_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("ADDS\n");
	ADD_CODE("LABEL $add_end_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");

	return true;
}

bool gen_sub(const char *unique_str)
{
	ADD_CODE("JUMPIFEQ $sub_strings_");
	ADD_CODE(unique_str);
	ADD_CODE(" GF@$expr_result int@2\n");
	ADD_CODE("JUMP $sub_nums_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("LABEL $sub_strings_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("EXIT int@4\n");
	ADD_CODE("LABEL $sub_nums_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("SUBS\n");

	return true;
}

bool gen_mul(const char *unique_str)
{
	ADD_CODE("JUMPIFEQ $mul_strings_");
	ADD_CODE(unique_str);
	ADD_CODE(" GF@$expr_result int@2\n");
	ADD_CODE("JUMP $mul_nums_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("LABEL $mul_strings_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("EXIT int@4\n");
	ADD_CODE("LABEL $mul_nums_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("MULS\n");

	return true;
}

bool gen_div(const char *unique_str)
{
	ADD_CODE("JUMPIFEQ $div_strings_");
	ADD_CODE(unique_str);
	ADD_CODE(" GF@$expr_result int@2\n");
	ADD_CODE("JUMPIFEQ $div_floats_");
	ADD_CODE(unique_str);
	ADD_CODE(" GF@$expr_result int@1\n");
	ADD_CODE("JUMP $div_ints_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");

	ADD_CODE("LABEL $div_strings_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("EXIT int@4\n");

	ADD_CODE("LABEL $div_floats_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	// Check division by zero
	ADD_CODE("JUMPIFEQ $div_zero_");
	ADD_CODE(unique_str);
	ADD_CODE(" GF@$expr_b float@0x0p+0\n");
	// Divide
	ADD_CODE("DIVS\n");
	ADD_CODE("JUMP $div_end_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");

	ADD_CODE("LABEL $div_ints_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	// Check division by zero
	ADD_CODE("JUMPIFEQ $div_zero_");
	ADD_CODE(unique_str);
	ADD_CODE(" GF@$expr_b int@0\n");
	// Divide
	ADD_CODE("IDIVS\n");
	ADD_CODE("JUMP $div_end_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");

	ADD_CODE("LABEL $div_zero_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("EXIT int@9\n");

	ADD_CODE("LABEL $div_end_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");

	return true;
}

bool gen_lt()
{
	ADD_CODE("LTS\n");
	return true;
}

bool gen_gt()
{
	ADD_CODE("GTS\n");
	return true;
}

bool gen_eq(const char *unique_str)
{
	ADD_CODE("JUMPIFNEQ $compatible_eq_");
	ADD_CODE(unique_str);
	ADD_CODE(" GF@$expr_result int@4\n");

	ADD_CODE("POPS GF@$expr_b\n");
	ADD_CODE("POPS GF@$expr_b\n");
	ADD_CODE("PUSHS bool@false\n");

	ADD_CODE("JUMP $end_of_eq_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");

	ADD_CODE("LABEL $compatible_eq_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("EQS\n");

	ADD_CODE("LABEL $end_of_eq_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");

	return true;
}

bool gen_and()
{
	ADD_CODE("ANDS\n");
	return true;
}

bool gen_or()
{
	ADD_CODE("ORS\n");
	return true;
}

bool gen_not()
{
	ADD_CODE("NOTS\n");
	return true;
}

bool gen_exit_if_incompatible_or_nil(const char *unique_str)
{
	ADD_CODE("JUMPIFEQ $incompatible_or_nil_");
	ADD_CODE(unique_str);
	ADD_CODE(" GF@$expr_result int@4\n");
	ADD_CODE("JUMPIFEQ $incompatible_or_nil_");
	ADD_CODE(unique_str);
	ADD_CODE(" GF@$expr_result int@3\n");
	ADD_CODE("JUMP $incompatible_or_nil_ok_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");

	ADD_CODE("LABEL $incompatible_or_nil_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");
	ADD_CODE("EXIT int@4\n");

	ADD_CODE("LABEL $incompatible_or_nil_ok_");
	ADD_CODE(unique_str);
	ADD_CODE("\n");

	return true;
}

bool gen_call(const char *func_id)
{
	// Push arguments from TF@ to LF@
	ADD_CODE("PUSHFRAME\n");

	// Call function
	ADD_CODE("CALL ");
	ADD_CODE(func_id);
	ADD_CODE("\n");

	return true;
}

bool gen_definition(symbol *sym)
{
	ADD_CODE("DEFVAR ");
	ADD_FRAME_PREFIX(sym);
	ADD_CODE(sym->id);
	ADD_CODE("\n");
	ADD_CODE("MOVE ");
	ADD_FRAME_PREFIX(sym);
	ADD_CODE(sym->id);
	ADD_CODE(" nil@nil");
	ADD_CODE("\n");

	return true;
}

bool gen_assignment(symbol *dst, symbol *src)
{
	ADD_CODE("MOVE ");
	ADD_FRAME_PREFIX(dst);
	ADD_CODE(dst->id);
	ADD_CODE(" ");
	ADD_SYMBOL(src);

	return true;
}

bool gen_type(symbol *dst, symbol *src)
{
	ADD_CODE("TYPE ");
	ADD_FRAME_PREFIX(dst);
	ADD_CODE(dst->id);
	ADD_CODE(" ");
	ADD_SYMBOL(src);

	return true;
}

bool gen_write(symbol *sym)
{
	ADD_CODE("WRITE ");
	ADD_SYMBOL(sym);

	return true;
}

// POZNÁMKA: pokud if nemá else, pak přesto v parseru zavolat gen_else přímo před gen_end_if

bool gen_if(char *label)
{
	ADD_CODE("# if statement ");
	ADD_CODE(label);
	ADD_CODE("\n");
	return true;
}

bool gen_then(char *label)
{
	ADD_CODE("# then ");
	ADD_CODE(label);
	ADD_CODE("\n");
	ADD_CODE("JUMPIFEQ $else_label_");
	ADD_CODE(label);
	ADD_CODE(" GF@$expr_result_type string@nil\n");
	ADD_CODE("JUMPIFNEQ $then_label_");
	ADD_CODE(label);
	ADD_CODE(" GF@$expr_result_type string@bool\n");
	ADD_CODE("JUMPIFEQ $else_label_");
	ADD_CODE(label);
	ADD_CODE(" GF@$expr_result bool@false\n");
	ADD_CODE("LABEL $then_label_");
	ADD_CODE(label);
	ADD_CODE("\n");
	return true;
}

bool gen_else(char *label)
{
	ADD_CODE("# else ");
	ADD_CODE(label);
	ADD_CODE("\n");
	ADD_CODE("JUMP $end_if_label_");
	ADD_CODE(label);
	ADD_CODE("\n");
	ADD_CODE("LABEL $else_label_");
	ADD_CODE(label);
	ADD_CODE("\n");
	return true;
}

bool gen_end_if(char *label)
{
	ADD_CODE("# end if ");
	ADD_CODE(label);
	ADD_CODE("\n");
	ADD_CODE("LABEL $end_if_label_");
	ADD_CODE(label);
	ADD_CODE("\n");
	return true;
}

bool gen_while(char *label)
{
	ADD_CODE("# while ");
	ADD_CODE(label);
	ADD_CODE("\n");
	ADD_CODE("LABEL $while_label_");
	ADD_CODE(label);
	ADD_CODE("\n");
	return true;
}

bool gen_do(char *label)
{
	ADD_CODE("# do ");
	ADD_CODE(label);
	ADD_CODE("\n");
	ADD_CODE("JUMPIFEQ $end_while_label_");
	ADD_CODE(label);
	ADD_CODE(" GF@$expr_result_type string@nil\n");
	ADD_CODE("JUMPIFNEQ $do_label_");
	ADD_CODE(label);
	ADD_CODE(" GF@$expr_result_type string@bool\n");
	ADD_CODE("JUMPIFEQ $end_while_label_");
	ADD_CODE(label);
	ADD_CODE(" GF@$expr_result bool@false\n");
	ADD_CODE("LABEL $do_label_");
	ADD_CODE(label);
	ADD_CODE("\n");
	return true;
}

bool gen_end_while(char *label)
{
	ADD_CODE("# end while ");
	ADD_CODE(label);
	ADD_CODE("\n");
	ADD_CODE("JUMP $while_label_");
	ADD_CODE(label);
	ADD_CODE("\n");
	ADD_CODE("LABEL $end_while_label_");
	ADD_CODE(label);
	ADD_CODE("\n");
	ADD_CODE("MOVE GF@$expr_result nil@nil\n"); // [doc 4.4. - while]
	return true;
}

bool gen_debug_info()
{
	ADD_CODE("BREAK\n");
	return true;
}

// Converts a string in format "abc\ndd ee\\abc" to format "abc\010dd\032ee\092abc" and adds it to program's source code
static bool add_string_literal(const char *str);

bool add_immediate_symbol(symbol *sym)
{
	char literal_str[32];
	switch (sym->type)
	{
		case SYM_NONE:
			ADD_CODE("nil@nil");
			break;
		case SYM_INT:
			ADD_CODE("int@");
			snprintf(literal_str, 32, "%d", sym->int_val);
			ADD_CODE(literal_str);
			break;
		case SYM_FLOAT:
			ADD_CODE("float@");
			snprintf(literal_str, 32, "%a", sym->float_val);
			ADD_CODE(literal_str);
			break;
		case SYM_STRING:
			ADD_CODE("string@");
			if (!add_string_literal(sym->string_val))
				return false;
			break;
		default:
			ADD_CODE("nil@nil");
			break;
	}
	ADD_CODE("\n");

	return true;
}

bool add_string_literal(const char *str)
{
	size_t old_found = -1;
	size_t new_found = -1;

	do {
		// Find first character to convert
		for (new_found = new_found+1; new_found < strlen(str); ++new_found)
		{
			int ord_val = str[new_found];
			// Values of special characters [doc 10.3.]
			if (ord_val <= 32 || ord_val == 35 || ord_val == 92)
				break;
		}

		// Add the part of "str" that doesn't need to be converted
		dstring_add_n(gen_target, str+(old_found+1), new_found-old_found-1);

		// Add a character that needs to be converted (if it exists)
		if (new_found != strlen(str))
		{
			char conv[4] = "\\0";

			// Loaded backslash, check for \", \n, \t, \s, \\ by loading another char
			if (((int)str[new_found]) == 92)
			{
				char new_char = str[++new_found];
				if (new_char == '"') new_char = '"';
				else if (new_char == 'n') new_char = '\n';
				else if (new_char == 't') new_char = '\t';
				else if (new_char == 's') new_char = ' ';
				else if (new_char == '\\') new_char = '\\';

				// Convert from hexadecimal number to a character
				if (new_char == 'x')
				{
					int value = 0;
					new_char = tolower(str[++new_found]);
					if (new_char >= '0' && new_char <= '9')
						value = new_char - '0';
					else if (new_char >= 'a' && new_char <= 'f')
						value = 10 + (new_char - 'a');

					new_char = tolower(str[++new_found]);
					if (new_char >= '0' && new_char <= '9')
						value = value * 16 + new_char - '0';
					else if (new_char >= 'a' && new_char <= 'f')
						value = value * 16 + 10 + (new_char - 'a');
					else
						--new_found; // TODO: nejspíš zde má být, ověřit po změně scanneru

					conv[1] = '0' + (((int)value) / 100) % 10;
					conv[2] = '0' + (((int)value) / 10) % 10;
					conv[3] = '0' + ((int)value) % 10;
				}
				// Convert '\n', '\t' etc. to a two digit number
				else
				{
					conv[2] = '0' + (((int)new_char) / 10) % 10;
					conv[3] = '0' + ((int)new_char) % 10;
				}
			}
			// Loaded a special character that is not a backslash
			else
			{
				conv[2] = '0' + (str[new_found] / 10) % 10;
				conv[3] = '0' + str[new_found] % 10;
			}

			dstring_add_n(gen_target, conv, 4);
		}

		// Move starting point
		old_found = new_found;

	} while(new_found != strlen(str));

	return true;
}
