//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class EmptyTree:public ::testing::Test{
protected:
	BinaryTree x;
};

class NonEmptyTree:public ::testing::Test{
protected:
	BinaryTree x;
	virtual void SetUp(){
		for(int i=0;i<5;i++){
			x.InsertNode(i);
		}
	}
};

class TreeAxioms:public ::testing::Test{
protected:
	BinaryTree x;
	virtual void SetUp(){
		for(int i=0;i<=2;i++){
			x.InsertNode(i);
		}
	}
};

TEST_F(EmptyTree,DeleteNode){
	EXPECT_FALSE(x.DeleteNode(1));
}

TEST_F(EmptyTree,InsertNode){
	std::pair<bool, BinaryTree::Node_t *> y;
	//1
	int cislo =1;
	y=x.InsertNode(cislo);
	EXPECT_TRUE(y.first);
	EXPECT_EQ(y.second->key, cislo);
	ASSERT_TRUE(y.second != NULL);
	EXPECT_EQ(y.second->color, BLACK);
	ASSERT_TRUE(y.second->pParent == NULL);
	ASSERT_TRUE(y.second->pLeft != NULL);
	ASSERT_TRUE(y.second->pRight != NULL);

	//Pleft
	ASSERT_TRUE(y.second->pLeft->pParent != NULL);
	EXPECT_EQ(y.second->pLeft->color, BLACK);
	EXPECT_EQ(y.second->pLeft->pParent->key, cislo);
	EXPECT_TRUE(y.second->pLeft->pLeft == NULL);
	EXPECT_TRUE(y.second->pLeft->pRight == NULL);
	//Pright
	ASSERT_TRUE(y.second->pRight->pParent != NULL);
	EXPECT_EQ(y.second->pRight->color, BLACK);
	EXPECT_EQ(y.second->pRight->pParent->key, cislo);
	EXPECT_TRUE(y.second->pRight->pLeft == NULL);
	EXPECT_TRUE(y.second->pRight->pRight == NULL);

	//1 znovu
	y=x.InsertNode(1);
	ASSERT_FALSE(y.first);

	//2
	cislo =2;
	y=x.InsertNode(cislo);
	EXPECT_TRUE(y.first);
	EXPECT_EQ(y.second->key, cislo);
	ASSERT_TRUE(y.second != NULL);
	EXPECT_EQ(y.second->color, RED);
	ASSERT_TRUE(y.second->pParent != NULL);
	ASSERT_TRUE(y.second->pLeft != NULL);
	ASSERT_TRUE(y.second->pRight != NULL);

	//Pleft
	ASSERT_TRUE(y.second->pLeft->pParent != NULL);
	EXPECT_EQ(y.second->pLeft->color, BLACK);
	EXPECT_EQ(y.second->pLeft->pParent->key, cislo);
	EXPECT_TRUE(y.second->pLeft->pLeft == NULL);
	EXPECT_TRUE(y.second->pLeft->pRight == NULL);
	//Pright
	ASSERT_TRUE(y.second->pRight->pParent != NULL);
	EXPECT_EQ(y.second->pRight->color, BLACK);
	EXPECT_EQ(y.second->pRight->pParent->key, cislo);
	EXPECT_TRUE(y.second->pRight->pLeft == NULL);
	EXPECT_TRUE(y.second->pRight->pRight == NULL);


}

TEST_F(EmptyTree,FindNode){
	EXPECT_TRUE(x.FindNode(1) == NULL);
	EXPECT_TRUE(x.FindNode(-42) == NULL);
}


TEST_F(NonEmptyTree,DeleteNode){
	for(int i=0;i<5;i++){
			EXPECT_TRUE(x.DeleteNode(i));
		}
	EXPECT_FALSE(x.DeleteNode(100));
}

TEST_F(NonEmptyTree,InsertNode){
	std::pair<bool, BinaryTree::Node_t *> y;
	//1
	int cislo =5;
	y=x.InsertNode(cislo);
	EXPECT_TRUE(y.first);
	EXPECT_EQ(y.second->key, cislo);
	ASSERT_TRUE(y.second != NULL);
	EXPECT_EQ(y.second->color, RED);
	ASSERT_TRUE(y.second->pParent != NULL);
	ASSERT_TRUE(y.second->pLeft != NULL);
	ASSERT_TRUE(y.second->pRight != NULL);

	//Pleft
	ASSERT_TRUE(y.second->pLeft->pParent != NULL);
	EXPECT_EQ(y.second->pLeft->color, BLACK);
	EXPECT_EQ(y.second->pLeft->pParent->key, cislo);
	EXPECT_TRUE(y.second->pLeft->pLeft == NULL);
	EXPECT_TRUE(y.second->pLeft->pRight == NULL);
	//Pright
	ASSERT_TRUE(y.second->pRight->pParent != NULL);
	EXPECT_EQ(y.second->pRight->color, BLACK);
	EXPECT_EQ(y.second->pRight->pParent->key, cislo);
	EXPECT_TRUE(y.second->pRight->pLeft == NULL);
	EXPECT_TRUE(y.second->pRight->pRight == NULL);

	//1 znovu
	y=x.InsertNode(1);
	ASSERT_FALSE(y.first);

	//2
	cislo =6;
	y=x.InsertNode(cislo);
	EXPECT_TRUE(y.first);
	EXPECT_EQ(y.second->key, cislo);
	ASSERT_TRUE(y.second != NULL);
	EXPECT_EQ(y.second->color, RED);
	ASSERT_TRUE(y.second->pParent != NULL);
	ASSERT_TRUE(y.second->pLeft != NULL);
	ASSERT_TRUE(y.second->pRight != NULL);

	//Pleft
	ASSERT_TRUE(y.second->pLeft->pParent != NULL);
	EXPECT_EQ(y.second->pLeft->color, BLACK);
	EXPECT_EQ(y.second->pLeft->pParent->key, cislo);
	EXPECT_TRUE(y.second->pLeft->pLeft == NULL);
	EXPECT_TRUE(y.second->pLeft->pRight == NULL);
	//Pright
	ASSERT_TRUE(y.second->pRight->pParent != NULL);
	EXPECT_EQ(y.second->pRight->color, BLACK);
	EXPECT_EQ(y.second->pRight->pParent->key, cislo);
	EXPECT_TRUE(y.second->pRight->pLeft == NULL);
	EXPECT_TRUE(y.second->pRight->pRight == NULL);

}

TEST_F(NonEmptyTree,FindNode){
	EXPECT_TRUE(x.FindNode(1) != NULL);
	EXPECT_TRUE(x.FindNode(100) == NULL);
	EXPECT_TRUE(x.FindNode(-42) == NULL);
}

//axiomy

TEST_F(TreeAxioms,Axiom1){
	std::vector<BinaryTree::Node_t *> uzly;
	x.GetLeafNodes(uzly);
	for(int i=0;i<uzly.size();i++){
		EXPECT_EQ(uzly[i]->color, BLACK);
	}
}


/*** Konec souboru black_box_tests.cpp ***/
