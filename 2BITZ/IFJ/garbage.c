/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#include "garbage.h"

GListPtr PtrList =NULL;

void* GMalloc(unsigned int size)
{
	void *tmp = malloc(size);

	if(PtrList==NULL){
		PtrList=malloc(sizeof(struct GList));
		PtrList->ptr=tmp;
		PtrList->RPtr=NULL;
	}else{
		GListPtr tmp2 = PtrList;
		while(tmp2->RPtr!=NULL){
			tmp2 = tmp2->RPtr;
		}
		tmp2->RPtr=malloc(sizeof(struct GList));
		tmp2=tmp2->RPtr;
		tmp2->ptr=tmp;
		tmp2->RPtr=NULL;
	}

	return tmp;
}

void* GRealloc(void* pointer,unsigned int size)
{
	if(pointer==NULL){
		return NULL;
	}

	void *tmp = realloc(pointer,size);

	if(PtrList==NULL){
		PtrList=malloc(sizeof(struct GList));
		PtrList->ptr=tmp;
		PtrList->RPtr=NULL;
	}else{
		GListPtr tmp2 = PtrList;
		while(tmp2->RPtr!=NULL){
			if(tmp2==pointer){
				break;
			}else{
				return NULL;
			}
			tmp2 = tmp2->RPtr;
		}
		tmp2->ptr=tmp;
	}

	return tmp;
}

void GFreeAll()
{
	GListPtr tmp = PtrList;
	while(tmp!=NULL){
		GListPtr tmp2 = tmp;
		tmp = tmp->RPtr;
		free(tmp2->ptr);
		free(tmp2);
	}
}