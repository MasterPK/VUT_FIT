//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::Element_t *PriorityQueue::newElement(int value){
	Element_t *n=new Element_t;
	n->pPrev=NULL;
	n->pNext=NULL;
	n->value=value;
	return n;
}


PriorityQueue::PriorityQueue()
{
	m_pHead=NULL;
}

PriorityQueue::~PriorityQueue()
{
	Element_t *x=m_pHead;
	while(x!=NULL){
		Element_t *y=x;
		delete(y);
		x=x->pNext;
	}
	m_pHead=NULL;
}

void PriorityQueue::Insert(int value)
{

	Element_t *y=m_pHead;
	if(m_pHead==NULL){
		m_pHead=newElement(value);
	}
	while(y!=NULL){
		
		if(y->pPrev==NULL && y->pNext==NULL){
			
			if(value>y->value){
				Element_t *n=newElement(value);
				n->pPrev=y;
				y->pNext=n;
			}else{
				Element_t *n=newElement(value);
				m_pHead=n;
				n->pNext=y;
			}
			break;
		}
		if(y->pPrev!=NULL && y->pNext!=NULL){
			
			if(y->pPrev->value<=value && value<=y->value){
				
				Element_t *n=newElement(value);
				y->pPrev->pNext=n;
				n->pPrev=y->pPrev;
				n->pNext=y;
				y->pPrev=n;
				
				break;
			}

		}
		if(y->pPrev==NULL && y->pNext!=NULL){
			
			if(value<y->value){
				Element_t *n=newElement(value);
				n->pNext=y;
				m_pHead=n;
				y->pPrev=m_pHead;
				break;
			}
			
		}
		if(y->pPrev!=NULL && y->pNext==NULL){
			
			if(value>y->value){
				Element_t *n=newElement(value);
				n->pPrev=y;
				y->pNext=n;
			}else{
				Element_t *n=newElement(value);
				n->pPrev=y->pPrev;
				n->pNext=y;
				y->pPrev->pNext=n;
				y->pPrev=y->pPrev->pNext;
			}
			break;
		}
		y=y->pNext;
	}


}

bool PriorityQueue::Remove(int value)
{
	Element_t *y=Find(value);
	if(y==NULL)
		return false;


	if(y->pPrev==NULL && y->pNext==NULL){

		m_pHead=NULL;
		return true;
	}
	if(y->pPrev==NULL && y->pNext!=NULL){

		m_pHead=y->pNext;
		y->pNext->pPrev=NULL;
		return true;
	}
	if(y->pPrev!=NULL && y->pNext==NULL){

		y->pPrev->pNext=NULL;
		return true;
	}
	if(y->pPrev!=NULL && y->pNext!=NULL){

		Element_t *tmp;
		tmp=y;
		y->pPrev->pNext=tmp->pNext;
		y->pNext->pPrev=tmp->pPrev;
		return true;
	}
	return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
	Element_t *y=m_pHead;
	while(y!=NULL){
		if(y->value==value){
			return y;
		}
		y=y->pNext;
	}
	return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
	return m_pHead;
}


/*** Konec souboru tdd_code.cpp ***/
