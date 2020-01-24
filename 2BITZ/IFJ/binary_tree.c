/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */

#include "binary_tree.h"


// Initialize tree
void BTInit (BTNodePtr* tree){
	(*tree)=NULL;

}



// Insert node with key and data, ordered by key by alphabet
int BTInsertNode (BTNodePtr *node, char *key, void* data){
	if((*node) == NULL){
		(*node)=GMalloc(sizeof(struct BTNode));
		(*node)->key=GMalloc(sizeof(char)*strlen(key)+1); // +1 because of '\0' terminator
		strcpy((*node)->key,key);
		(*node)->data = data;
		//printf("dolni:%s\n",(*node)->key);
		(*node)->LPtr=NULL;
		(*node)->RPtr=NULL;
		return TRUE;
	}else{
		if(strcmp((*node)->key,key)<0){	
			if(BTInsertNode(&(*node)->RPtr,key,data)){	
				return TRUE;
			}else{
				return FALSE;
			}
		}else if(strcmp((*node)->key,key)>0){
			if(BTInsertNode(&(*node)->LPtr,key,data)){
				return TRUE;
			}else{
				return FALSE;
			}
		}else{
			return TRUE;
		}
	}
}

//Search node by key, return true if exists, else false
int BTSearchNodeByKey (BTNodePtr *node, char *key){
	if((*node) == NULL){
		return FALSE;
	}else{
		if(strcmp((*node)->key,key)<0){	
			if(BTSearchNodeByKey(&(*node)->RPtr,key)){	
				return TRUE;
			}else{
				return FALSE;
			}
		}else if(strcmp((*node)->key,key)>0){
			if(BTSearchNodeByKey(&(*node)->LPtr,key)){
				return TRUE;
			}else{
				return FALSE;
			}
		}else{
			return TRUE;
		}
	}
}

//Search node by key, return pointer to node in tree, else NULL
BTNodePtr BTSearchNodeByKeyPtr (BTNodePtr *node, char *key){
	if((*node) == NULL){
		return NULL;
	}else{
		if(strcmp((*node)->key,key)<0){	
			BTNodePtr tmp=BTSearchNodeByKeyPtr(&(*node)->RPtr,key);
			if(tmp==NULL){
				return NULL;
			}else{
				return tmp;
			}
		}else if(strcmp((*node)->key,key)>0){
			BTNodePtr tmp=BTSearchNodeByKeyPtr(&(*node)->LPtr,key);
			if(tmp==NULL){
				return NULL;
			}else{
				return tmp;
			}
		}else{
			return (*node);
		}
	}
}

// Edit node by key, at this moment changes key because of testing but in future will change only data of node
int BTEditNodeByKey(BTNodePtr *node, char *key, void *data){
	/*if((*node) == NULL){
		return FALSE;
	}else{
		if(strcmp((*node)->key,key)==0){
			strcpy((*node)->key,new_key);
			return TRUE;
		}else if(strcmp((*node)->key,key)>0){	
			return(BTSearchNodeByKey(&(*node)->RPtr,key));

		}else if(strcmp((*node)->key,key)<0){
			return(BTSearchNodeByKey(&(*node)->LPtr,key));
		}else{
			return FALSE;
		}
	}
	(void) new_key;

*/
	BTNodePtr tmp = BTSearchNodeByKeyPtr(&(*node),key);
	if(tmp == NULL){
		return FALSE;
	}else{
		//strcpy(tmp->key,new_key);//change to void *data
		(*node)->data=data;
		return TRUE;
	}

	
}

//Replace key and data of parameter node_to_replace by MostRight element in tree
void BTReplaceMostRight(BTNodePtr *node, BTNodePtr *node_to_replace){
	if((*node) == NULL){
		return;
	}else{
		if((*node)->RPtr!=NULL){
			BTReplaceMostRight(&(*node)->RPtr,node_to_replace);
		}
		else{
			strcpy((*node_to_replace)->key,(*node)->key);
			(*node_to_replace)->data=(*node)->data;
			return;
		}
	}
}

// Return MostRight element in tree
BTNodePtr BTFindMostRight(BTNodePtr *node){
	if((*node) == NULL){
		return NULL;
	}else{
		if((*node)->RPtr!=NULL){
			return BTFindMostRight(&(*node)->RPtr);
		}
		else{
			return (*node);
		}
	}
}

// Return MostLeft element in tree
BTNodePtr BTFindMostLeft(BTNodePtr *node){
	if((*node) == NULL){
		return NULL;
	}else{
		if((*node)->LPtr!=NULL){
			return BTFindMostLeft(&(*node)->LPtr);
		}
		else{
			return (*node);
		}
	}
}


// Recursive delete of all tree, state as BTInit
void BTDeleteTree(BTNodePtr *node){
	if((*node) == NULL){
		return;
	}else{
		if((*node)->LPtr!=NULL){
			BTDeleteTree(&(*node)->LPtr);
		}
		if((*node)->RPtr!=NULL){
			BTDeleteTree(&(*node)->RPtr);
		}

		if((*node)->LPtr==NULL && (*node)->RPtr==NULL){
			//BTNodePtr tmp=(*node);
			(*node) = NULL;
			//free(tmp);
		}

	}

}

// Find Node by key and delete element from tree
void BTDeleteNodeByKey(BTNodePtr *node, char *key){
	BTNodePtr tmp;
	if((*node) == NULL){
		return;
	}else{
		if(strcmp((*node)->key,key)<0){
			BTDeleteNodeByKey(&(*node)->RPtr,key);
			return;
		}else if(strcmp((*node)->key,key)>0){
			BTDeleteNodeByKey(&(*node)->LPtr,key);
			return;
		}else{
			tmp=(*node);
			if (((*node)->LPtr == NULL) &&((*node)->RPtr) == NULL)
			{
				(*node) = NULL;
				//free(tmp);
			}
			else if (((*node)->LPtr == NULL) &&((*node)->RPtr) != NULL)
			{
				(*node) = (*node)->RPtr;
				//free(tmp);
			}
			else if (((*node)->LPtr != NULL) &&((*node)->RPtr) == NULL)
			{
				(*node) = (*node)->LPtr;
				//free(tmp);
			}
			else{
				BTReplaceMostRight(&(*node)->LPtr,&(*node));
				BTDeleteNodeByKey(&(*node)->LPtr,tmp->key);
			}
		}
	}
}

void bt_print_inorder(BTNodePtr node, void (*print_func)(void *sym))
{
	if (node)
	{
		bt_print_inorder(node->LPtr, print_func);
		print_func(node->data);
		bt_print_inorder(node->RPtr, print_func);
	}
}

void bt_list_all_inorder(BTNodePtr node, void (*list_all_func)(void *sym))
{
	if (node)
	{
		bt_list_all_inorder(node->LPtr, list_all_func);
		list_all_func(node->data);
		bt_list_all_inorder(node->RPtr, list_all_func);
	}
}
