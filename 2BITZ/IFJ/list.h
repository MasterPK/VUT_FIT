/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef LIST_H
#define LIST_H

#include<stdio.h>
#include <stdbool.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include "token.h"
#include "garbage.h"

typedef struct Element
{
	struct Element *ptr;
	char *data;
	token token_tmp;
} *ElementPtr;


typedef struct {
	ElementPtr Active;
	ElementPtr First;
	ElementPtr Last;
} List;

void Error();
void InitList(List *);
void DisposeList(List *);
void ListInsert(List *, char*);
void ListInsertLast (List *, token);
void ListPush(List *, token);
token ListPop(List *);
bool ListSearch(List *, char*);
int ListLen(List *);

#endif