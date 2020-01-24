/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef SCANNER_H
#define SCANNER_H

#include "garbage.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include <stdbool.h>
#include "list.h"
#include "errors.h"
#include "dstring.h"

extern int run;
extern List tokensList;


// Returns next token by reference (token *t)
//bool get_token(token *t); //parser funguje jinak
token get_token();


#endif
