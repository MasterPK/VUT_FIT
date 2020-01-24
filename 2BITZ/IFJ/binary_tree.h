/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include "dstring.h"
#include <assert.h>
#include <string.h>
#include "garbage.h"

#define TRUE 1
#define FALSE 0


//prvek stromu
typedef struct BTNode{
	struct BTNode *LPtr;
    struct BTNode *RPtr;
    void *data;
    int type;
    char *key;
} *BTNodePtr;

/*typedef struct {
	BTNode root;
	BTNode last;
} BTStruct;*/



void BTInit (BTNodePtr*);
int BTInsertNode (BTNodePtr* ,char *, void*);
void Print_tree2(BTNodePtr TempTree, char* sufix, char fromdir);
void Print_tree(BTNodePtr TempTree);
int BTSearchNodeByKey (BTNodePtr *node, char *key);
BTNodePtr BTSearchNodeByKeyPtr (BTNodePtr *node, char *key);
int BTEditNodeByKey(BTNodePtr *node, char *key, void *data);
void BTDeleteNodeByKey(BTNodePtr *node, char *key);
BTNodePtr BTFindMostRight(BTNodePtr *node);
BTNodePtr BTFindMostLeft(BTNodePtr *node);
void BTDeleteTree(BTNodePtr *node);
void bt_print_inorder(BTNodePtr node, void (*print_func)(void *sym));
void bt_list_all_inorder(BTNodePtr node, void (*list_all_func)(void *sym));

#endif
