/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include "binary_tree.h"
#include "garbage.h"
#include "scanner.h"
#include "token.h"
#include "symtable.h"
#include "errors.h"
#include "code_gen.h"
#include "list.h"
#include "expression.h"

extern token token_tmp;
extern symtable table_global;
extern symtable table_local;

void print_elements_of_list(List TL);

int parser();

int first_run();
int second_run();

int get_next_token();

void debug(char* string);
void debugN(double number);
void debugnN(double number);
void debugn(char* string);

int prog_first(int depth);
int prog_second();

int prog_def();
int prog_def_2();

int param_list_first(List* list, symtable *table);
int param_list(List* list, symtable *table);

int statement(symtable *table);
int statement_list(symtable *table);
int statement_id(symtable *table);
int statement_id_func(symtable *table, symbol *fce, symbol* id);
int statement_func(symtable *table, symbol *fce);
int statement_if(symtable *table);
int statement_kw(symtable *table);
int statement_while(symtable *table);
int statement_print(symtable *table);

int expression();

void add_internal_functions(symtable *table);
void add_one_internal_function(symtable *table, char* name, List *params);

int arg_list_first(symbol *fce, symtable *table,bool brackets);
int arg_list(symbol *fce, symtable *table, int count,bool brackets, ElementPtr tmp);

int print_arg_list_first(symtable *table);
int print_arg_list(symtable *table);

#endif
