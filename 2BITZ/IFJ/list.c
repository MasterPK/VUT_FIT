/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#include "list.h"


void Error(){
	printf("Error! Ilegal operation!\n");
}

//Initialization of list
void InitList(List *L){
	L->First = NULL;
	L->Active = NULL;
	L->Last = NULL;
}

//Delete and free all objects in list
void DisposeList(List *L){
	L->Active = L->First;

	while(L->Active != NULL){
		L->First = L->Active->ptr;
		free(L->Active);
		L->Active = L->First;
	}
}

//Insert object at the end of the list
void ListInsert (List *L, char* data){

	if (L->First == NULL)
	{
		ElementPtr newItem = NULL;
		newItem = malloc(sizeof(struct Element));

		newItem->data = data;
		newItem->ptr = NULL;
		L->First = newItem;
		return;
	}

	L->Active = L->First;
	ElementPtr tmp = NULL;

	//Controling if list does not contain same as curently inserted data
	while(L->Active != NULL){
		if (strcmp(L->Active->data, data) == 0)
		{
			Error();
			return;
		}
		tmp = L->Active;
		L->Active = L->Active->ptr;
	}

	ElementPtr newItem = NULL;
	newItem = malloc(sizeof(struct Element));

	newItem->data = data;
	newItem->ptr = NULL;
	tmp->ptr = newItem;

	L->Active = newItem;
	L->Last=L->Active;
}

//Inserting token to list
void ListInsertLast (List *L, token data){

	if (L->First == NULL)
	{
		ElementPtr newItem = NULL;
		newItem = malloc(sizeof(struct Element));

		newItem->token_tmp = data;
		newItem->ptr = NULL;
		L->First = newItem;
		return;
	}

	L->Active = L->First;
	ElementPtr tmp = NULL;


	while(L->Active != NULL){
		tmp = L->Active;
		L->Active = L->Active->ptr;
	}

	ElementPtr newItem = NULL;
	newItem = malloc(sizeof(struct Element));

	newItem->token_tmp = data;
	newItem->ptr = NULL;
	tmp->ptr = newItem;

	L->Active = newItem;
	L->Last=L->Active;
}

//Push tokent to list
void ListPush (List *L, token data)
{

	if (L->First == NULL)
	{
		ElementPtr newItem = NULL;
		newItem = malloc(sizeof(struct Element));

		newItem->token_tmp = data;
		newItem->ptr = NULL;
		L->First = newItem;
		L->Last=L->First;
		return;
	}

	ElementPtr tmp = L->First;

	ElementPtr newItem;
	newItem=malloc(sizeof(struct Element));
	newItem->token_tmp=data;
	newItem->ptr=tmp;
	L->First=newItem;
}

//Pop token from list
token ListPop(List *L)
{
	ElementPtr tmp=L->First->ptr;
	

	token tmp2 = L->First->token_tmp;
	free(L->First);
	L->First=tmp;
	return tmp2;


}

//Searching if list contains some data 
bool ListSearch(List *L, char* data){

	L->Active = L->First;
	while(L->Active != NULL){
		if (strcmp(L->Active->data, data) == 0)
		{
			return true;
		}
		L->Active = L->Active->ptr;
	}
	return false;
}

//Lenght of list
int ListLen(List *L){

	L->Active = L->First;

	int c = 0;
	while(L->Active != NULL){
		c++;
		L->Active = L->Active->ptr;
	}
	return c;
}