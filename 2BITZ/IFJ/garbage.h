/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef GARBAGE_H
#define GARBAGE_H

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct GList{
	void *ptr;
	struct GList *RPtr;
} *GListPtr;



void* GMalloc(unsigned int size);
void* GRealloc(void* pointer,unsigned int size);
void GFreeAll();




#endif