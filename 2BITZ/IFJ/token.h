/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#ifndef TOKEN_H
#define TOKEN_H

typedef enum token_type_t {
	
	// Literals
	TOKEN_INT,//0
	TOKEN_FLOAT,//1
	TOKEN_STRING,//2

	// Identifier/keyword
	TOKEN_IDENTIFIER,//3
	TOKEN_KEYWORD,//4

	// Whitespace
	TOKEN_EOL,//5
	TOKEN_EOF,//6
	TOKEN_WHITESPACE, // TODO: možná není třeba 7

	// Expressions
	TOKEN_COMMA,// comma ',' 8
	TOKEN_LB,	// left bracket '(' 9
	TOKEN_RB,	// right bracket ')' 10

	// Comparison operators
	TOKEN_EQ, // equality			== 11
	TOKEN_NE, // not equal			!= 12
	TOKEN_LT, // less than			< 13
	TOKEN_GT, // greater than		> 14
	TOKEN_LE, // less or equal		<= 15
	TOKEN_GE, // greater or equal	>= 16 

	// Arithmetic operators
	TOKEN_PLUS, //17
	TOKEN_MINUS, //18
	TOKEN_MUL, //19
	TOKEN_DIV, //20
	TOKEN_ASSIGN, //21

	//Error
	TOKEN_ERROR, //22

	TOKEN_EXPO //Number in exponencial form 23
} token_type;

typedef enum keyword_type_t {
	KW_DEF,
	KW_DO,
	KW_ELSE,
	KW_END,
	KW_IF,
	KW_NOT,
	KW_NIL,
	KW_THEN,
	KW_WHILE,

	KW_PRINT,
	// TODO: později případně další vestavěné funkce
} keyword_type;

typedef enum state_t{
    STATE_BEGIN = 0,// 0 Start

    STATE_LT, // less than  <
    STATE_GT, // greater than >
    STATE_EQ, // equality =
    STATE_NE, // not equal !=
    STATE_NS,  // #

    STATE_MUL, // *
    STATE_MINUS, // -

    STATE_SLASH, // \...

    STATE_OS,    // \n...

    STATE_EOL,
    STATE_COMMENT_FINAL,
    STATE_COMMENT_EQ,
    STATE_COMMENT_EOL,
    STATE_LINE_COMMENT,
    STATE_COMMENT_TEST,
    STATE_ID,
    STATE_ID2,
	STATE_STRING,
	STATE_HEX,   // \x
	STATE_HEX2,   // \x
    STATE_NUMBER,
    STATE_NUMBER_POINT_1,
    STATE_NUMBER_POINT_2,
    STATE_NUMER_DECIMAL,
    STATE_NUMBER_EXPONENT,
    STATE_NUMBER_EXPONENT_FINAL,
    STATE_NUMBER_EXPONENT_SIGN

} state_t;

typedef struct token_t {
	token_type type;
	union {
		int int_val;
		double float_val;
		char *string_val;
		keyword_type keyword_val;
	};
} token;

#endif
