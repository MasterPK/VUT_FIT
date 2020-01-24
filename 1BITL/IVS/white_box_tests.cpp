//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

class Matrix_5x5:public ::testing::Test{
protected:
	int col=5;
	int row=5;
	Matrix x = Matrix (row,col);
	virtual void SetUp(){
		for(int r=0;r<5;r++){
			for(int c=0;c<5;c++){
				if(!x.set(r,c,r))
					break;
			}
		}
	}
};

class Matrix_3x2:public ::testing::Test{
protected:
	int col=2;
	int row=3;
	Matrix x = Matrix (row,col);
	virtual void SetUp(){
		for(int r=0;r<row;r++){
			for(int c=0;c<col;c++){
				if(!x.set(r,c,r+1))
					break;
			}
		}
	}
};


TEST_F(Matrix_5x5,Get_One){
	/*for(int r=0;r<5;r++){
		for(int c=0;c<5;c++){
			printf("[%d,%d]:%f\n",r,c,x.get(r,c));
		}
	}*/
	EXPECT_EQ(x.get(0,0),0);
	EXPECT_EQ(x.get(2,3),2);
	EXPECT_EQ(x.get(4,4),4);
	//EXPECT_ANY_THROW(x.get(4,15)); //ZJISTIT


}

TEST_F(Matrix_5x5,Set_One){
	EXPECT_EQ(x.get(2,2),2);
	EXPECT_EQ(x.get(2,3),2);
	EXPECT_EQ(x.get(2,4),2);

	x.set(2,3,10);
	EXPECT_EQ(x.get(2,2),2);
	EXPECT_EQ(x.get(2,3),10);
	EXPECT_EQ(x.get(2,4),2);

	EXPECT_FALSE(x.set(5,9,1));

}

TEST_F(Matrix_5x5,Set_Matrix){
	EXPECT_EQ(x.get(2,2),2);
	EXPECT_EQ(x.get(2,3),2);
	EXPECT_EQ(x.get(2,4),2);

	std::vector<std::vector< double > > data;

	std::vector< double > radek;
	for(int r=0;r<5;r++){
	radek.push_back(1);
	radek.push_back(6);
	radek.push_back(8);
	radek.push_back(10);
	radek.push_back(9);
	data.push_back(radek);
	};


	x.set(data);

	EXPECT_EQ(x.get(2,2),8);
	EXPECT_EQ(x.get(0,0),1);
	EXPECT_EQ(x.get(4,4),9);
}

TEST_F(Matrix_5x5,Set_Matrix_except){
	EXPECT_EQ(x.get(2,2),2);
	EXPECT_EQ(x.get(2,3),2);
	EXPECT_EQ(x.get(2,4),2);

	std::vector<std::vector< double > > data;

	std::vector< double > radek;
	for(int r=0;r<5;r++){
	radek.push_back(1);
	radek.push_back(6);
	radek.push_back(8);
	radek.push_back(10);
	radek.push_back(9);
	radek.push_back(7);
	data.push_back(radek);
	};


	EXPECT_FALSE(x.set(data));
}


TEST_F(Matrix_5x5,Equal_same){
	Matrix y = Matrix(5,5);
	for(int r=0;r<5;r++){
			for(int c=0;c<5;c++){
				if(!y.set(r,c,r))
					break;
			}
	}

	EXPECT_TRUE(x.operator==(y));

	y.set(1,1,0);
	EXPECT_EQ(y.get(1,1),0);

	EXPECT_FALSE(x.operator==(y));
}

TEST_F(Matrix_5x5,Equal_except){
	Matrix y = Matrix(5,6);
	for(int r=0;r<5;r++){
			for(int c=0;c<6;c++){
				if(!y.set(r,c,r))
					break;
			}
	}

	EXPECT_ANY_THROW(x.operator==(y));
}

TEST_F(Matrix_5x5,Plus_same){
	Matrix y = Matrix(5,5);
	for(int r=0;r<5;r++){
			for(int c=0;c<5;c++){
				if(!y.set(r,c,r))
					break;
			}
	}


	x=x.operator+(y);
	/*printf("\n");
	for(int r=0;r<5;r++){
		for(int c=0;c<5;c++){
			printf("%f ",x.get(r,c));
		}
		printf("\n");
	}*/

	EXPECT_EQ(x.get(1,1),2);
	EXPECT_EQ(x.get(3,1),6);

}

TEST_F(Matrix_5x5,Plus_except){
	Matrix y = Matrix(5,6);
	for(int r=0;r<5;r++){
			for(int c=0;c<6;c++){
				if(!y.set(r,c,r))
					break;
			}
	}


	EXPECT_ANY_THROW(x.operator+(y));
}

TEST_F(Matrix_5x5,Mul_same){
	Matrix y = Matrix(5,5);
	for(int r=0;r<5;r++){
			for(int c=0;c<5;c++){
				if(!y.set(r,c,r))
					break;
			}
	}


	x=x.operator*(y);
	printf("\n");
	/*for(int r=0;r<5;r++){
		for(int c=0;c<5;c++){
			printf("%f ",x.get(r,c));
		}
		printf("\n");
	}*/

	EXPECT_EQ(x.get(0,1),0);
	EXPECT_EQ(x.get(1,1),10);
	EXPECT_EQ(x.get(3,1),30);

}

TEST_F(Matrix_5x5,Mul_except){
	Matrix y = Matrix(7,6);
	for(int r=0;r<7;r++){
			for(int c=0;c<6;c++){
				if(!y.set(r,c,r))
					break;
			}
	}


	EXPECT_ANY_THROW(x.operator*(y));


}

TEST_F(Matrix_5x5,SolveEq_except){

	std::vector<double> b;
	b.push_back(10);
	b.push_back(20);
	b.push_back(30);
	b.push_back(40);
	b.push_back(50);

	EXPECT_ANY_THROW(x.solveEquation(b));
}


TEST_F(Matrix_3x2,Get_One){
	/*for(int r=0;r<5;r++){
		for(int c=0;c<5;c++){
			printf("[%d,%d]:%f\n",r,c,x.get(r,c));
		}
	}*/
	EXPECT_EQ(x.get(0,0),1);
	EXPECT_EQ(x.get(2,1),3);
	//EXPECT_ANY_THROW(x.get(4,15)); //ZJISTIT


}

TEST_F(Matrix_3x2,Set_One){
	EXPECT_EQ(x.get(0,0),1);
	EXPECT_EQ(x.get(2,1),3);

	x.set(2,1,10);
	EXPECT_EQ(x.get(0,0),1);
	EXPECT_EQ(x.get(2,1),10);

	EXPECT_FALSE(x.set(2,2,1));

}

TEST_F(Matrix_3x2,Set_Matrix){
	EXPECT_EQ(x.get(0,0),1);
	EXPECT_EQ(x.get(2,1),3);

	std::vector<std::vector< double > > data;

	std::vector< double > radek;
	for(int r=0;r<3;r++){
	radek.push_back(1);
	radek.push_back(6);
	data.push_back(radek);
	};




	x.set(data);

	EXPECT_EQ(x.get(0,0),1);
	EXPECT_EQ(x.get(2,1),6);
}

TEST_F(Matrix_3x2,Set_Matrix_except){
	EXPECT_EQ(x.get(0,0),1);
	EXPECT_EQ(x.get(2,1),3);

	std::vector<std::vector< double > > data;

	std::vector< double > radek;
	for(int r=0;r<3;r++){
	radek.push_back(1);
	radek.push_back(6);
	radek.push_back(7);
	data.push_back(radek);
	};


	EXPECT_FALSE(x.set(data));
}


TEST_F(Matrix_3x2,Equal_same){
	Matrix y = Matrix(3,2);
	for(int r=0;r<3;r++){
			for(int c=0;c<2;c++){
				if(!y.set(r,c,r+1))
					break;
			}
	}

	EXPECT_TRUE(x.operator==(y));

	y.set(1,1,0);
	EXPECT_EQ(y.get(1,1),0);

	EXPECT_FALSE(x.operator==(y));
}

TEST_F(Matrix_3x2,Equal_except){
	Matrix y = Matrix(5,6);
	for(int r=0;r<5;r++){
			for(int c=0;c<6;c++){
				if(!y.set(r,c,r))
					break;
			}
	}

	EXPECT_ANY_THROW(x.operator==(y));
}

TEST_F(Matrix_3x2,Plus_same){
	Matrix y = Matrix(3,2);
	for(int r=0;r<3;r++){
			for(int c=0;c<2;c++){
				if(!y.set(r,c,r+1))
					break;
			}
	}


	x=x.operator+(y);
	/*printf("\n");
	for(int r=0;r<5;r++){
		for(int c=0;c<5;c++){
			printf("%f ",x.get(r,c));
		}
		printf("\n");
	}*/

	EXPECT_EQ(x.get(1,1),4);
	EXPECT_EQ(x.get(2,1),6);

}

TEST_F(Matrix_3x2,Plus_except){
	Matrix y = Matrix(5,6);
	for(int r=0;r<5;r++){
			for(int c=0;c<6;c++){
				if(!y.set(r,c,r))
					break;
			}
	}


	EXPECT_ANY_THROW(x.operator+(y));
}

TEST_F(Matrix_3x2,Mul_same){
	Matrix y = Matrix(3,2);
	for(int r=0;r<3;r++){
			for(int c=0;c<2;c++){
				if(!y.set(r,c,r+1))
					break;
			}
	}

	EXPECT_ANY_THROW(x.operator*(y));

}

TEST_F(Matrix_3x2,Mul_except){
	Matrix y = Matrix(7,6);
	for(int r=0;r<7;r++){
			for(int c=0;c<6;c++){
				if(!y.set(r,c,r))
					break;
			}
	}


	EXPECT_ANY_THROW(x.operator*(y));


}

TEST_F(Matrix_3x2,SolveEq_except){

	std::vector<double> b;
	b.push_back(10);
	b.push_back(20);
	b.push_back(30);

	EXPECT_ANY_THROW(x.solveEquation(b));
}






/*** Konec souboru white_box_tests.cpp ***/
