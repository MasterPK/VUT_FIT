/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H
#include "dstring.h"
#include "symtable.h"
#include <stdbool.h>

// Pointer to target of code generator's functions, defined in code_gen.c
extern dstring *gen_target;

// Redirects output of gen_* functions into dstring *new_target
void gencontrol_redirect_output(dstring *new_target);

// Generate program's header (.IFJcode18)
bool gen_program_header();
bool gen_builtin_functions();

// Exit the program with a given exit code
bool gen_program_exit(int exit_code);

// Define a function
bool gen_function_start(const char *func_id);
bool gen_function_end(const char *func_id);

// Prepare call to function by creating a temporary frame
bool gen_call_prepare();
// Define (using arg_name) and initialize (using symbol src) an argument on temporary frame
bool gen_call_arg(char *arg_name, symbol *src);
// Push temporary frame to frame stack, then call func
bool gen_call(const char *func_id);

// Define a variable
bool gen_definition(symbol *sym);

// Assign a value to variable "dst" using symbol "src"
bool gen_assignment(symbol *dst, symbol *src);

// Assign type of symbol "src" to variable "dst"
bool gen_type(symbol *dst, symbol *src);

// Evaluating expressions
bool gen_push(symbol *sym);	// symbol 'sym' -> top of stack
bool gen_pop(symbol *sym);	// top of stack -> variable 'sym'
// NOTE: all arithmetic instructions require both operands to be of the same type
bool gen_add(const char *unique_str);
bool gen_sub(const char *unique_str);
bool gen_mul(const char *unique_str);
bool gen_div(const char *unique_str);
// Relation operators
bool gen_lt();
bool gen_gt();
bool gen_eq(const char *unique_str);
// Logical operators
bool gen_and();
bool gen_or();
bool gen_not();
// Called after conversions ONLY IF the next instruction is not EQ/NEQ
bool gen_exit_if_incompatible_or_nil(const char *unique_str);

// Control structures
bool gen_if(char *label);
bool gen_then(char *label);
bool gen_else(char *label);
bool gen_end_if(char *label);
bool gen_while(char *label);
bool gen_do(char *label);
bool gen_end_while(char *label);

// Input/Output
bool gen_write(symbol *sym);

// Others
bool gen_debug_info();

#endif
