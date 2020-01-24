/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#include "scanner.h"

//default state
state_t state = STATE_BEGIN;


//debuging function
#define DEBUG FALSE
void debug2(char* string)
{
    if(DEBUG){
        fprintf(stderr,"%s\n",string);
    }
}

//Initialization of buffer and List for tokens
dstring buffer;
List tokensList;

//Initialization of intermediate variables
int c;
int test = 0; 
int x=0;
int y=0;
int run=0;
int size = 0;

//Add char "c" to dstring
void buffAdd(char c){

    dstring_add_char(&buffer,c);
}


//Function that returns next token
token get_token()
{
    //pop token from list after first run
    if (tokensList.First != NULL && run==1)
    {
        return ListPop(&tokensList);
    }

    debug2("zavolan get_token\n");

    //free and initialize the buffer
    dstring_free(&buffer);
    dstring_init(&buffer);
    token newToken;


    while(true)
    {
        //test if I red first char of file
        test++;

        debug2("zavolan while\n");
    	if (y == 1)
    	{
    		newToken.type = TOKEN_EOF;
            return newToken;
    	}

        //get next char from stdin
    	c = getchar();

        //return EOF tokken if char is EOF and add token to list
		if (c == EOF)
		{
			y++;
			newToken.type = TOKEN_EOF;
            ListInsertLast(&tokensList, newToken);
			return newToken;
		}
        
        //
        
        switch(state){
            case STATE_BEGIN:

                // Whitespace
                if(c == ' ' || c == '\r' || c == '\t'){
                    break;
                }
                //return EOL tokken if char is EOFL and add token to list
                if(c == '\n')
                {
                    state = STATE_EOL;
                	newToken.type = TOKEN_EOL;
                    ListInsertLast(&tokensList, newToken);
                    size = 0;
                    //buffer.data = NULL;
                    return newToken;
                }

                //return EOF tokken if char is EOF and add token to list
                if(c == EOF)
                { 
					newToken.type = TOKEN_EOF;
					ListInsertLast(&tokensList, newToken);
                    return newToken;
                }

                //Return token of char or go to next state:

                // Expresions
                else if(c == '(')
                { 
                	newToken.type = TOKEN_LB;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }

                else if (c == ',')
                {
                    newToken.type = TOKEN_COMMA;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }

                else if(c == ')')
                { 
                    newToken.type = TOKEN_RB;
                    ListInsertLast(&tokensList, newToken);
                    return newToken; 
                }

                else if(c == '<')
                { 
                    state = STATE_LT;
                    break;
                }

                else if(c == '>')
                { 
                    state = STATE_GT;
                    break;
                }

                // Arithmetic operators
                else if(c == '+')
                { 
                    newToken.type = TOKEN_PLUS;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }

                else if (c == '-')
                {
                    newToken.type = TOKEN_MINUS;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }

                else if (c == '*')
                {
                    newToken.type = TOKEN_MUL;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }

                else if (c == '/')
                {
                    newToken.type = TOKEN_DIV;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }

                else if (c == '#')
                {
                    state = STATE_NS;
                }

                else if (c == '!')
                {
                    state = STATE_NE;
                }

                else if (c == '=')
                {
                    state = STATE_EQ;
                }

                else if (c == '"')
                {
                    state = STATE_STRING;
                }

                //Number
                else if(isdigit(c))
                { 
                    debug2("digit\n");
                    buffAdd(c);      
                    state = STATE_NUMBER;
                }

                //ID
                else if( ((c >= 'a') && (c <= 'z')) || c == '_' )
                { 
                    buffAdd(c);
                    state = STATE_ID;
                }
                
                //Invalid char on stdin
                else
                {
                    debug2("Lexikální chyba!!");
                    EXIT_ERROR(LEX_ERR);
                }
            break;

            //state End of Line
            case STATE_EOL:
                if (c == '=')
                {
                    state = STATE_COMMENT_TEST;
                    break;
                }
                else
                {
                    ungetc(c,stdin);
                    state = STATE_BEGIN;
                }
            break;

            //state when I get Number on stdin
            case STATE_NUMBER:
                if(isdigit(c))
                {
                    buffAdd(c);
                    break;
                }
                else if(c == '.')
                {
                    state = STATE_NUMBER_POINT_1;
                    buffAdd(c);
                    break;
                }
                else if(c == 'e' || c == 'E')
                {
                    state = STATE_NUMBER_EXPONENT;
                    buffAdd(tolower(c));
                    break;
                }
                else
                {
                	newToken.type = TOKEN_INT;
                	newToken.int_val = atoi(buffer.data);
                    ungetc(c, stdin);
                    size = 0;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }

            break;

            // Floating point number
            case STATE_NUMBER_POINT_1:
                if(isdigit(c))
                {
                    state = STATE_NUMBER_POINT_2;
                    buffAdd(c);
                    break;
                }

                else
                {
                    exit(LEX_ERR);
                }
            break;

            //If I got a number after dot(floating point number 2)
            case STATE_NUMBER_POINT_2:
                if (isdigit(c))
                {
                    buffAdd(c);
                    break;
                }

                else if (c == 'e' || c == 'E')
                {
                    buffAdd(c);
                    state = STATE_NUMBER_EXPONENT;
                    break;
                }

                else
                {
                    newToken.type = TOKEN_FLOAT;
                    newToken.float_val = atof(buffer.data);
                    ungetc(c, stdin);
                    size = 0;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }

            //State when after some number I get eor E as exponent
            case STATE_NUMBER_EXPONENT:
                if(isdigit(c))
                {
                    state = STATE_NUMBER_EXPONENT_FINAL;
                    buffAdd(c);
                    break;
                }
                else if(c == '+' || c == '-')
                {
                    state = STATE_NUMBER_EXPONENT_SIGN;
                    buffAdd(c);
                    break;
                }
                else
                {
                    debug2("Lexikální chyba!!");
                    EXIT_ERROR(LEX_ERR);
                }
            break;

            //State when I got sign after exponent
            case STATE_NUMBER_EXPONENT_SIGN:
                if (isdigit(c))
                {
                    buffAdd(c);
                    state = STATE_NUMBER_EXPONENT_FINAL;
                    break;
                }

                else
                {
                    debug2("Lexikální chyba!!");
                    EXIT_ERROR(LEX_ERR);
                }
            break;

            //State when I got number after sign or exponent 
            case STATE_NUMBER_EXPONENT_FINAL:
                if (isdigit(c))
                {
                    buffAdd(c);
                    break;
                }

                else
                {
                    newToken.type = TOKEN_FLOAT;
                    newToken.float_val = strtod(buffer.data, NULL);
                    ungetc(c, stdin);
                    size = 0;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }
            break;

            //State when I got '=' 
            case STATE_EQ:
            	if (c == 'b' && test == 1)
            	{
            		buffAdd(c);
            		state = STATE_COMMENT_TEST;
            		break;
            	}
                if (c == '=')
                {
                    state = STATE_BEGIN;
                    newToken.type = TOKEN_EQ;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }
                else
                {
                    state = STATE_BEGIN;
                    ungetc(c,stdin);
                    newToken.type = TOKEN_ASSIGN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }
            break;

            //Start of multiline coments when I got 'b' after '='
            case STATE_COMMENT_TEST:
                debug2("SOM V COMENT TEST:\n");
                if(buffer.data==NULL)
                {
                    buffer.data=malloc(sizeof(char));
                }

                //printf("%s\n", buffer.data);
                if (strcmp(buffer.data, "begin") == 0)
                {
                    debug2("Našiel som BEGIN\n");
                    ungetc(c,stdin);
                    state = STATE_LINE_COMMENT;
                    size = 0;
                    dstring_free(&buffer);
                    dstring_init(&buffer);
                    break;
                }

                debug2("HLADAM znak\n");
                //printf("%c\n", c);
                if (c == 'b' || c == 'e' || c == 'g' || c == 'i' || c == 'n')
                {
                    debug2("Našiel som znak\n");
                    buffAdd(c);
                    break;
                }
                else
                {
                	exit(LEX_ERR);
                }
            break;

            //State when I find "=begin"
            case STATE_LINE_COMMENT:                
                if (c == '\n')
                {
                    debug2("jsem tu");
                    state = STATE_COMMENT_EOL;
                    break;
                }
                else if (c == EOF)
                {
                    debug2("Lexikální chyba!!");
                    EXIT_ERROR(LEX_ERR);  
                }
                else
                {
                    break;
                }
            break;

            //State when I find '\n' after =begin
            case STATE_COMMENT_EOL:
                if (c == '=')
                {
                    debug2("Našiel som = na novom riadku\n");
                    size = 0;
                    state = STATE_COMMENT_EQ;
                    break;
                }
                else
                {
                    
                    break;
                }
            break;

            //State when I find '=' on new line after I find "=begin"
            case STATE_COMMENT_EQ:
                if (buffer.data == NULL)
                {
                    buffer.data = malloc(sizeof(char));
                }

                if (strcmp(buffer.data, "end") == 0)
                {
                    ungetc(c,stdin);
                    debug2("Našiel som END\n");
                    state = STATE_COMMENT_FINAL;;
                    break;
                }

                if ((c == 'e' || c == 'n' || c == 'd'))
                {
                    debug2("Hladam END\n");
                    buffAdd(c);
                    break;
                }


                else
                {
                    dstring_free(&buffer);
                    dstring_init(&buffer);
                    state = STATE_LINE_COMMENT;
                    break;
                }
            break;

            //State when I find "=end" after "=begin"
            case STATE_COMMENT_FINAL:
                if (c == '\n')
                {
                    debug2("Našiel som KONIEC RIADKU\n");
                    size = 0;
                    dstring_free(&buffer);
                    dstring_init(&buffer);
                    state = STATE_BEGIN;
                    break;
                }
                else if (c == EOF)
                {
                    size = 0;
                    dstring_free(&buffer);
                    dstring_init(&buffer);
                    ungetc(c,stdin);
                    state = STATE_BEGIN;
                    break;
                }
                else
                {
                    break;
                }
            break;

            //State when I fint starting char of ID
            case STATE_ID:
				if (isalpha(c) || isdigit(c) || c == '_')  
				{
		    		buffAdd(c);
				}
				else if ((c == '?') || (c == '!'))
				{
                    buffAdd(c);
                    newToken.type = TOKEN_IDENTIFIER;
                    newToken.string_val = malloc(sizeof(char)*strlen(buffer.data));
					strcpy(newToken.string_val,buffer.data);
                    //buffer.data = NULL;
                    size = 0;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
				}

                //Controling if I found keyworld or ID
				else 
				{
                    ungetc(c, stdin);
                    if (strcmp(buffer.data, "def") == 0)
                    {
                        newToken.type = TOKEN_KEYWORD;
                        newToken.keyword_val = KW_DEF;
                    }

                    else if (strcmp(buffer.data, "do") == 0)
                    {
                        newToken.type = TOKEN_KEYWORD;
                        newToken.keyword_val = KW_DO;
                    }

                    else if (strcmp(buffer.data, "else") == 0)
                    {
                        newToken.type = TOKEN_KEYWORD;
                        newToken.keyword_val = KW_ELSE;
                    }

                    else if (strcmp(buffer.data, "end") == 0)
                    {
                        newToken.type = TOKEN_KEYWORD;
                        newToken.keyword_val = KW_END;
                    }

                    else if (strcmp(buffer.data, "if") == 0)
                    {
                        newToken.type = TOKEN_KEYWORD;
                        newToken.keyword_val = KW_IF;
                    }

                    else if (strcmp(buffer.data, "not") == 0)
                    {
                        newToken.type = TOKEN_KEYWORD;
                        newToken.keyword_val = KW_NOT;
                    }

                    else if (strcmp(buffer.data, "nil") == 0)
                    {
                        newToken.type = TOKEN_KEYWORD;
                        newToken.keyword_val = KW_NIL;
                    }

                    else if (strcmp(buffer.data, "then") == 0)
                    {
                        newToken.type = TOKEN_KEYWORD;
                        newToken.keyword_val = KW_THEN;
                    }

                    else if (strcmp(buffer.data, "while") == 0)
                    {
                        newToken.type = TOKEN_KEYWORD;
                        newToken.keyword_val = KW_WHILE;
                    }

                    else
                    {
                        newToken.type = TOKEN_IDENTIFIER;
                        newToken.string_val = malloc(sizeof(char)*strlen(buffer.data));
                        strcpy(newToken.string_val,buffer.data);
                    }

                    //buffer.data = NULL;
                    size = 0;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
				}
				break;

            //State when I found '<'
            case STATE_LT:
                if (c == '=')
                {
                    newToken.type = TOKEN_LE;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }
                else
                {
                    ungetc(c, stdin);
                    newToken.type = TOKEN_LT;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }
            break;

            //State when I found '>'
            case STATE_GT:
                if (c == '=')
                {
                    newToken.type = TOKEN_GE;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }
                else
                {
                    ungetc(c, stdin);
                    newToken.type = TOKEN_GT;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }
            break;

            //State when I found '!'
            case STATE_NE:
                if (c == '=')
                {
                    newToken.type = TOKEN_NE;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }
                else
                {
                    
                    debug2("Lexikální chyba!!");
                    EXIT_ERROR(LEX_ERR);
                }

            break;  

            //One line coments
            case STATE_NS:
                if (c == '\n')
                {
                    ungetc(c, stdin);
                    state = STATE_BEGIN;
                }
            break;
            
            //String - when I found '"'
            case STATE_STRING:
                if(c == '"')
                {
                    newToken.type = TOKEN_STRING;
                    if(buffer.data!=NULL)
                    {
                        newToken.string_val = malloc(sizeof(char)*strlen(buffer.data));
                        strcpy(newToken.string_val,buffer.data);
                    }
                    else
                    {
                        newToken.string_val = malloc(sizeof(char));
                        newToken.string_val[0]='\0';
                    }
                    
                    //buffer.data = NULL;
                    size = 0;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }
                else if (c != '\\' && c > 31)
                {
                    buffAdd(c);
                    break;
                }
                else if (c == '\\')
                {
                    buffAdd(c);
                    state = STATE_SLASH;
                    break;
                }
                else
                {
                    debug2("Lexikální chyba!!");
                    EXIT_ERROR(LEX_ERR);
                }
            break;

            //State when I found '\' after '"'
            case STATE_SLASH:
                if (c == '"' || c == 'n' || c == 's' || c == 't' || c == '\\')
                {
                    buffAdd(c);
                    state = STATE_OS;
                    break;
                }
                else if (c == 'x')
                {
                    buffAdd(c);
                    state = STATE_HEX;
                    break;
                }
                else
                {
                    debug2("Lexikální chyba!!");
                    EXIT_ERROR(LEX_ERR);
                }

            break;

            //State when I found some special char after '\'
            case STATE_OS:
                if (c == '"')
                {
                    newToken.type = TOKEN_STRING;
                    newToken.string_val = malloc(sizeof(char)*strlen(buffer.data));
                    strcpy(newToken.string_val,buffer.data);
                    //buffer.data = NULL;
                    size = 0;
                    state = STATE_BEGIN;
                    ListInsertLast(&tokensList, newToken);
                    return newToken;
                }
                else if (c == '\\')
                {
                    buffAdd(c);
                    state = STATE_SLASH;
                    break;
                }
                else if (c > 31)
                {
                    buffAdd(c);
                    state = STATE_STRING;
                    break;
                }
                else 
                {
                    debug2("Lexikální chyba!!");
                    EXIT_ERROR(LEX_ERR);
                }
            break;

            //State when I found "\x" start of hexadecimal number
            case STATE_HEX:
                if (isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F'|| c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f')
                {
                    buffAdd(c);
                    state = STATE_HEX2;
                    break;
                }
                else
                {
                    debug2("Lexikální chyba!!");
                    EXIT_ERROR(LEX_ERR);
                }
            break;

            //State when I found one hexadecimal char
            case STATE_HEX2:
                if (isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F'|| c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f')
                {
                    buffAdd(c);
                    state = STATE_STRING;
                    break;
                }
                else
                {
                    ungetc(c, stdin);
                    state = STATE_STRING;
                    break;
                }
            break;

            //Default
            default: 
                newToken.type = TOKEN_EOF;
                return newToken;
                break;                       
        }
    }


	

}
