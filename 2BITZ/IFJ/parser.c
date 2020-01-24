/* Project: Compiler IFJ18
 * Authors: Petr Křehlík(xkrehl04), Mikeš Tibor(xmikes12), Martin Klobušický(xklobu03), Pavel Fučík(xfucik08)
 * Date: 20.11.2018
 */
#include "parser.h"

// Global definitions of symtables and dynamic strings for output code
symtable table_global;
symtable table_local;

dstring program;
dstring if_while_dstring;

// Varible to store current token
token token_tmp;

// Counter to generate always unique names for IF/WHILE generation
unsigned int counter=0;


// Load next token to token_tmp
int get_next_token()
{
	token_tmp=get_token();
	debugn("PARSER GET_TOKEN:");
	debugnN(token_tmp.type);
	if(token_tmp.type==TOKEN_KEYWORD)
	{
		debugn(",KW_VAL:");
		debugnN(token_tmp.keyword_val);
	}

	debug("");
	
	return 0;
}


/* Universal functions to debug program if there is error...
 * Easy to use, just set DEBUG to FALSE or TRUE
*/

#define DEBUG FALSE
void debug(char* string)
{
	if(DEBUG){
		fprintf(stderr,"%s\n",string);
	}
}
void debugN(double number)
{
	if(DEBUG){
		fprintf(stderr,"%g\n",number);
	}
}
void debugnN(double number)
{
	if(DEBUG){
		fprintf(stderr,"%g",number);
	}
}
void debugn(char* string)
{
	if(DEBUG){
		fprintf(stderr,"%s",string);
	}
}

// Function to debug list, copied from IAL
void print_elements_of_list(List TL)	{
	List TempList=TL;	
	int CurrListLength = 0;
	printf("-----------------");
	while ((TempList.First!=NULL) && (CurrListLength<100))	{
		printf("\n\t %d",TempList.First->token_tmp.type);
		if ((TempList.First==TL.Active) && (TL.Active!=NULL))
			printf("\t <= toto je aktivní prvek ");
		TempList.First=TempList.First->ptr;	
		CurrListLength++;
	}
	printf("\n-----------------\n");
}


// Main parser function called from main
int parser()
{

	InitList(&tokensList);

	dstring_init(&program);
	dstring_init(&if_while_dstring);	

	gencontrol_redirect_output(&program);

	gen_program_header();
	

	// Fill up global table with functions definitions
	int return_code_1=first_run(); 

	// Syntax and semantic analysis by LL
	int return_code_2=second_run(); 

	
	
	gen_program_exit(0);

	debugn("Return code 1 run:");debugN(return_code_1);
	debugn("Return code 2 run:");debugN(return_code_2);

	// If all return codes are zero, then generate code
	if(return_code_1==0 && return_code_2==0)
	{
		printf("%s", program.data);
		
		
	}
	dstring_free(&if_while_dstring);
	dstring_free(&program);
	DisposeList(&tokensList);
	
	//GFreeAll();
	if(return_code_2>0)
	{
		return return_code_2;
	}
	else
	{
		return return_code_1;
	}
}

// Help function to add one entry into symtable
void add_one_internal_function(symtable *table, char* name, List *params)
{
	symbol *tmp;
	tmp=GMalloc(sizeof(symbol));
	tmp->id=GMalloc(sizeof(char)*strlen(name));
	strcpy(tmp->id,name);
	tmp->type=SYM_FUNC;
	tmp->params=params;
	symtable_add(table,name,tmp);
}

// Function that add all internal function and their parameters to global symtable
void add_internal_functions(symtable *table)
{
	char *functions[8]={"inputs","inputi","inputf","print","length","ord","substr","chr"};
	int i=0;
	List *params;
	params=GMalloc(sizeof(List));
	InitList(params);
	add_one_internal_function(table,functions[i++],params);//inputs
	add_one_internal_function(table,functions[i++],params);//inputi
	add_one_internal_function(table,functions[i++],params);//inputf
	add_one_internal_function(table,functions[i++],params);//print, standalone rule
	params=GMalloc(sizeof(List));
	InitList(params);
	ListInsert(params,"s");
	add_one_internal_function(table,functions[i++],params);//length
	params=GMalloc(sizeof(List));
	InitList(params);
	ListInsert(params,"s");
	ListInsert(params,"i");
	add_one_internal_function(table,functions[i++],params);//ord
	params=GMalloc(sizeof(List));
	InitList(params);
	ListInsert(params,"s");
	ListInsert(params,"i");
	ListInsert(params,"n");
	add_one_internal_function(table,functions[i++],params);//substr
	params=GMalloc(sizeof(List));
	InitList(params);
	ListInsert(params,"i");
	add_one_internal_function(table,functions[i++],params);//chr

	
}



//First run of parser only fill up GlobatTable with definitions of functions
//WARNING! First run only search for functions definitions in main body, it doesnt use LL
int first_run()
{
	debug("First run...Fill up table_global");
	
	get_next_token();
	if(token_tmp.type==TOKEN_EOF){
		return 0;
	}

	symtable_init(&table_global);
	add_internal_functions(&table_global);
	gen_builtin_functions();
	

	int code=prog_first(0);

	run=1;
	return code;


}

// Second run
int second_run()
{
	debug("second run...Analysis");


	symtable_init(&table_local);

	int code=prog_second();

	return code;
}



// Searching for function definitons in main body, saves in global symtable
int prog_first(int depth){
	if(token_tmp.type == TOKEN_KEYWORD && token_tmp.keyword_val == KW_DEF)
	{
		return prog_def();
	}
	else if(token_tmp.type == TOKEN_KEYWORD && (token_tmp.keyword_val == KW_IF || token_tmp.keyword_val == KW_WHILE))
	{
		get_next_token();
		int code;
		while(token_tmp.type!=TOKEN_KEYWORD || token_tmp.keyword_val!=KW_END)
		{
			if(token_tmp.type==TOKEN_EOF){
				RETURN_ERROR(SYNTAX_ERR);
			}
			if(token_tmp.type == TOKEN_KEYWORD && token_tmp.keyword_val == KW_IF)
			{
				code =  prog_first(1);
			}
			if(token_tmp.type == TOKEN_KEYWORD && token_tmp.keyword_val == KW_WHILE)
			{
				code =  prog_first(1);
			}
			get_next_token();
		}

		if(code!=NO_ERR)
		{
			return code;
		}
		
		get_next_token();
		if(token_tmp.type!=TOKEN_EOL)
		{
			RETURN_ERROR(SYNTAX_ERR);
		}
		if(depth==0)
		{
			return prog_first(0);
		}
		else
		{
			RETURN_ERROR(NO_ERR);
		}
		
	}
	else if(token_tmp.type==TOKEN_EOF)
	{
		return 0;
	}
	else if(token_tmp.type==TOKEN_EOL)
	{
		get_next_token();
		return prog_first(depth);
	}
	else
	{	
		get_next_token();
		return prog_first(depth);
	}

	return 0;
}

// Main prog LL rule for second run
int prog_second(){
	debugn("<prog> -> ");
	get_next_token();

	if(token_tmp.type==TOKEN_EOF)
	{
		// <prog> -> eof
		debug("epsilon");
		RETURN_ERROR(NO_ERR);
	}
	else if(token_tmp.type == TOKEN_KEYWORD && token_tmp.keyword_val == KW_DEF)
	{
		// <prog> -> def <func-id> (<param-list-first>) eol <statement-list> end eol <prog>
		debug("prog -> KW");
		int code = prog_def_2();
		if(code!=NO_ERR)
		{
			RETURN_ERROR(code);
		}
		else
		{
			return prog_second();
		}
	}
	else if(token_tmp.type==TOKEN_EOL)
	{
		// <prog> -> eol <prog>
		debug("<prog> -> epsilon (EOL)");
		return prog_second();
	}
	else
	{	
		// <prog> -> <statement> <prog>
		int code = statement(&table_global);
		if(code!=NO_ERR)
		{
			RETURN_ERROR(code);
		}
		else
		{
			return prog_second();
		}
	}

	return 0;

}
// Found function definition in first run, save data to symtable and jump to end
int prog_def(){
	get_next_token();
	if(token_tmp.type==TOKEN_IDENTIFIER){
		if(symtable_find(&table_global,token_tmp.string_val)==NULL)
		{
			symbol *s1;
			s1=GMalloc(sizeof(symbol));
			s1->id=GMalloc(sizeof(char)*strlen(token_tmp.string_val));
			s1->type=SYM_FUNC;
			s1->immediate=false;
			strcpy(s1->id,token_tmp.string_val);
			debug(token_tmp.string_val);
			symtable_add(&table_global,token_tmp.string_val,s1);

			get_next_token();
			if(token_tmp.type==TOKEN_LB){
				
				List *tmp;
				tmp=GMalloc(sizeof(List));
				InitList(tmp);
				int code=param_list_first(tmp,&table_global);
				if(code!=0)
				{
					return code;
				}
				else
				{
					s1->params=tmp;
				}
			}
			else
			{
				RETURN_ERROR(SYNTAX_ERR);
			}

			get_next_token();
			while(token_tmp.type!=TOKEN_KEYWORD || token_tmp.keyword_val!=KW_END){
				get_next_token();
				if(token_tmp.type==TOKEN_EOF){
					RETURN_ERROR(SYNTAX_ERR);
				}
			}
			get_next_token();
			return prog_first(0);
			
		}else
		{
			fprintf(stderr,"Symbol %s is already defined!\n",token_tmp.string_val);
			RETURN_ERROR(SEM_DEF_ERR); //Symbol error, variable is already defined
		}
	}
	


	return 0;


}

// LL Rule
// <prog> -> def <func-id> (<param-list-first>) eol <statement-list> end eol
int prog_def_2(){
	debug("def <func-id> (<arg-list>) eol <statement-list> end eol");
	get_next_token();

	
	symtable_free(&table_local);
	symtable_init(&table_local);
	if(token_tmp.type==TOKEN_IDENTIFIER){
		symbol *tmp = symtable_find(&table_global,token_tmp.string_val);
		if(tmp==NULL)
		{
			debug("Fatal error in func definitions!");
			exit(99);
		}
		else
		{
			

			token fce = token_tmp;
			debug("gen_function_start");
			gen_function_start(fce.string_val);
			get_next_token();
			if(token_tmp.type==TOKEN_LB)
			{
				
				List *tmp;
				tmp=GMalloc(sizeof(List));
				InitList(tmp);
				debug("<param-list>");
				int code=param_list_first(tmp,&table_local);
				if(code!=0)
				{
					RETURN_ERROR( code);
				}
			}
			else
			{
				RETURN_ERROR(SYNTAX_ERR);
			}
			get_next_token();
			if(token_tmp.type!=TOKEN_EOL)
			{
				RETURN_ERROR(SYNTAX_ERR);
			}
			get_next_token();
			if(token_tmp.type == TOKEN_KEYWORD && token_tmp.keyword_val == KW_END)
			{
				debug("gen_function_end");
				gen_function_end(fce.string_val);
				RETURN_ERROR( NO_ERR);
			}

			debug("<statement-list>");
			int code=statement_list(&table_local);
			

			if(code!=0)
			{
				RETURN_ERROR( code);
			}

			get_next_token();
			if(token_tmp.type!=TOKEN_EOL)
			{
				RETURN_ERROR(SYNTAX_ERR);
			}

			debug("return def");
			/*get_next_token();
			if(token_tmp.type!=TOKEN_EOL){
				revert_token();
			}*/
			gen_function_end(fce.string_val);
			//symtable_print(&table_local);
			RETURN_ERROR( NO_ERR);
		}
	}
	else
	{
		RETURN_ERROR(SYNTAX_ERR);
	}


	


}


// LL Rule
// <param-list-first>
 int param_list_first(List* list, symtable *table){
 	debug("param list first");
	get_next_token();

	
	if(token_tmp.type==TOKEN_RB)
	{
		// <param-list-first> -> epsilon
		RETURN_ERROR(NO_ERR);
		
	}
	else if(token_tmp.type==TOKEN_IDENTIFIER || token_tmp.type==TOKEN_INT || token_tmp.type==TOKEN_FLOAT || token_tmp.type==TOKEN_STRING)
	{
		// <param-list-first> -> <id> <param-list>
		if(!ListSearch(list,token_tmp.string_val))
		{
				ListInsert(list,token_tmp.string_val);
				
				symbol *s1;
				s1=GMalloc(sizeof(symbol));
				
				s1->id=GMalloc(sizeof(char)*strlen(token_tmp.string_val));
				
				strcpy(s1->id,token_tmp.string_val);
				
				s1->type=SYM_NONE;
				s1->frame=SYM_LF;
				s1->immediate=false;
				debug(s1->id);
				symtable_add(table,token_tmp.string_val,s1);
				debug(list->First->data);
				//gen_definition(s1);
		}
		else
		{

			RETURN_ERROR(SEM_DEF_ERR);
		}
		get_next_token();
		return param_list(list,table);
	}
	else
	{
		RETURN_ERROR(SYNTAX_ERR);
	}
	
}
//LL Rule
//<param-list>
int param_list(List* list, symtable *table){
	debug("second param_list");
	
	if(token_tmp.type==TOKEN_RB)
	{
		//<param-list> -> epsilon
		RETURN_ERROR(NO_ERR);
		
	}
	if(token_tmp.type == TOKEN_COMMA){
		//<param-list> -> , <id> <param-list>
		get_next_token();
		if(token_tmp.type==TOKEN_IDENTIFIER || token_tmp.type==TOKEN_INT || token_tmp.type==TOKEN_FLOAT || token_tmp.type==TOKEN_STRING)
		{
			debug(token_tmp.string_val);
			if(!ListSearch(list,token_tmp.string_val))
			{
				
				ListInsert(list,token_tmp.string_val);
				symbol *s1;
				s1=GMalloc(sizeof(symbol));
				s1->id=GMalloc(sizeof(char)*strlen(token_tmp.string_val));
				strcpy(s1->id,token_tmp.string_val);
				s1->type=SYM_NONE;
				s1->frame=SYM_LF;
				s1->immediate=false;
				debug(s1->id);
				symtable_add(table,token_tmp.string_val,s1);
				
				
			}
			else
			{
				RETURN_ERROR(SEM_ELSE_ERR);
			}
			get_next_token();
			return param_list(list,table);
		}
		else
		{
			RETURN_ERROR(SYNTAX_ERR);
		}
	}
	else
	{
		RETURN_ERROR(SYNTAX_ERR);
	}
}

// Variable to determine if we are in IF/WHILE, needed because of the problem with variable redefinitions in code generation
int if_while_active=0;

// LL Rule
// <statement-list>
int statement_list(symtable *table){
	debug("statement_list");
	if(token_tmp.type == TOKEN_KEYWORD && token_tmp.keyword_val == KW_END)
	{
		// <statement-list> -> epsilon
		RETURN_ERROR(NO_ERR);
		
	}
	else if(token_tmp.type==TOKEN_EOF)
	{
		RETURN_ERROR(SYNTAX_ERR);

	}else if(token_tmp.type == TOKEN_KEYWORD && token_tmp.keyword_val == KW_ELSE)
	{
		// <statement-list> -> epsilon
		RETURN_ERROR(NO_ERR);
	}
	else if(token_tmp.type==TOKEN_EOL)
	{
		// <statement-list> -> eol <statement-list>
		get_next_token();
		return statement_list(table);
	}
	else
	{
		// <statement-list> -> <statement> <statement-list>
		int code = statement(table);
		if(code!=NO_ERR)
		{
			RETURN_ERROR( code);
		}
		get_next_token();
		return statement_list(table);
	}
	
}
// LL Rule
// <statement>
int statement(symtable *table)
{
	
	if(token_tmp.type==TOKEN_KEYWORD && token_tmp.keyword_val==KW_DEF)
	{
		RETURN_ERROR(SYNTAX_ERR);
	};

	// <statement>
	debug("<statement> ->");
	switch(token_tmp.type)
	{
		case TOKEN_IDENTIFIER:return statement_id(table);break;
		case TOKEN_KEYWORD:return statement_kw(table);break;
		case TOKEN_INT:
		case TOKEN_FLOAT:
		case TOKEN_STRING:
		case TOKEN_LB:
		case TOKEN_RB:
		case TOKEN_EQ:
		case TOKEN_NE:
		case TOKEN_LT:
		case TOKEN_GT:
		case TOKEN_LE:
		case TOKEN_GE:
		case TOKEN_PLUS:
		case TOKEN_MINUS:
		case TOKEN_MUL: 
		case TOKEN_DIV: 
		case TOKEN_EXPO:
		debug("exp call");
		int code = expression_analysis(table);

		debugN(code);
		if(code!=NO_ERR)
		{
			RETURN_ERROR(code);
		}
		// Call <prog> or <statement-list>
		RETURN_ERROR( NO_ERR);
		break;

		default: RETURN_ERROR(SYNTAX_ERR);
	} 
	

}


/**
 * @brief      Expression analysis, use precedent syntactic analysis
 *
 * @return     Return code, if ok 0, else other codes
 */
int expression(){
	
	get_next_token();
	while(token_tmp.type!=TOKEN_EOL)
	{
		
		if(token_tmp.type==TOKEN_KEYWORD && token_tmp.keyword_val==KW_THEN)
		{
			break;
		}
		if(token_tmp.type==TOKEN_KEYWORD && token_tmp.keyword_val==KW_DO)
		{
			break;
		}
		if(token_tmp.type==TOKEN_KEYWORD && token_tmp.keyword_val==KW_ELSE)
		{
			
			break;
		}
		if(token_tmp.type==TOKEN_KEYWORD && token_tmp.keyword_val==KW_END)
		{
			
			break;
		}
		if(token_tmp.type==TOKEN_EOF)
		{
			
			break;
		}
		get_next_token();
	}
	debug("end exp");
	RETURN_ERROR(NO_ERR);
}

// Check if current token can by start of expression
int check_exp_start()
{
	switch(token_tmp.type)
	{
		case TOKEN_KEYWORD:
			if (token_tmp.keyword_val == KW_NIL)
			{
				RETURN_ERROR(NO_ERR);
				break;
			}
			else
			{
				RETURN_ERROR(SYNTAX_ERR);
			}

		case TOKEN_IDENTIFIER:
		case TOKEN_INT:
		case TOKEN_FLOAT:
		case TOKEN_STRING:
		case TOKEN_LB:
		case TOKEN_RB:
		case TOKEN_EQ:
		case TOKEN_NE:
		case TOKEN_LT:
		case TOKEN_GT:
		case TOKEN_LE:
		case TOKEN_GE:
		case TOKEN_PLUS:
		case TOKEN_MINUS:
		case TOKEN_MUL: 
		case TOKEN_DIV: 
		case TOKEN_EXPO:RETURN_ERROR(NO_ERR);break;
		default: RETURN_ERROR(SYNTAX_ERR);
	} 
}

// <statement> -> and identificator
int statement_id(symtable *table){
	debug("<statement> -> <id> or <func-id>");

	if (strcmp(token_tmp.string_val, "print") == 0)
	{
		//<statement> -> print(<print-arg-list-first>) eol
		int print_ret = statement_print(table);
		if (print_ret == NO_ERR)
		{
			// Load EOL/EOF
			if (token_tmp.type != TOKEN_EOL && token_tmp.type != TOKEN_EOF)
				get_next_token();

			if (token_tmp.type != TOKEN_EOL && token_tmp.type != TOKEN_EOF)
				RETURN_ERROR(SYNTAX_ERR);

			RETURN_ERROR( NO_ERR);
		}
		else
			RETURN_ERROR( print_ret);
	}
	
	token token_id=token_tmp;

	//symtable_print(&table_global);
	symbol *global=symtable_find(&table_global,token_tmp.string_val);
	if(global!=NULL && global->type==SYM_FUNC)
	{
		debug("fce");
		get_next_token();
		ListPush(&tokensList,token_tmp);
		if(token_tmp.type==TOKEN_ASSIGN)
		{
			RETURN_ERROR(SEM_DEF_ERR);
		}
		//<statement> -> <func-id>(<arg-list-first>) eol
		return statement_func(table,global);
	}

	

	//symtable_print(table);
	symbol *tmp = symtable_find(table,token_id.string_val);
	bool generate=false;
	if(tmp==NULL)
	{
		get_next_token();
		ListPush(&tokensList,token_tmp);
		if(token_tmp.type==TOKEN_ASSIGN)
		{
			debug("generate");
			generate=true;
		}
		else
		{
			RETURN_ERROR(SEM_DEF_ERR);

		}
	}
	
	get_next_token();

	if(token_tmp.type==TOKEN_ASSIGN)
	{
		debug("<statement> -> <id> =");
		
		// <statement> -> <id> =
		get_next_token();

		if(token_tmp.type==TOKEN_IDENTIFIER)
		{
			symbol *fce=symtable_find(&table_global,token_tmp.string_val);
			if(fce!=NULL && fce->type==SYM_FUNC)
			{
				// <statement> -> <id> = <func-id>(<arg-list-first>) eol

				debugn("id=func:");
				debug(token_id.string_val);
				tmp=GMalloc(sizeof(symbol));
				tmp->id=GMalloc(sizeof(char)*strlen(token_id.string_val));
				strcpy(tmp->id,token_id.string_val);
				tmp->type=SYM_NONE;
				tmp->frame=SYM_LF;
				tmp->immediate=false;
				if(generate==true)
				{
					generate=false;
					
					symtable_add(table,token_id.string_val,tmp);
					debug("gen_definition");
					// If IF/WHILE is active, we must write definitons before
					if(if_while_active)
						gencontrol_redirect_output(&program);

					gen_definition(tmp);

					if(if_while_active)
						gencontrol_redirect_output(&if_while_dstring);
				}
				// <statement> -> <id> = <func-id>(<arg-list-first>) eol
				return statement_id_func(table,fce,tmp);
			}
		}
		
		int check = check_exp_start();
		if(check!=NO_ERR)
		{
			debug("check exp start ERR");
			return check;
		}
		debug("check exp start OK");

		// Special variant a=a
		if(token_tmp.type==TOKEN_IDENTIFIER && strcmp(token_tmp.string_val,token_id.string_val)==0)
		{
			debug("a=a");
			get_next_token();
			ListPush(&tokensList,token_tmp);
			int check = check_exp_start();
			if(check!=NO_ERR)
			{
				generate=false;
				tmp=GMalloc(sizeof(symbol));
				tmp->id=GMalloc(sizeof(char)*strlen(token_id.string_val));
				strcpy(tmp->id,token_id.string_val);
				tmp->frame=SYM_LF;
				tmp->immediate=false;
				tmp->type=SYM_NONE;
				symtable_add(table,token_id.string_val,tmp);
				debugn("gen_definition");
				if(if_while_active)
					gencontrol_redirect_output(&program);

				gen_definition(tmp);

				if(if_while_active)
					gencontrol_redirect_output(&if_while_dstring);
				//symbol expr_result_sym = {.id="$expr_result", .immediate=false, .frame=SYM_GF};
				//debug("gen_assignment");
				//gen_assignment(tmp,&expr_result_sym);
				RETURN_ERROR( NO_ERR);
				
			}
			else
			{
				token_tmp=token_id;
			}
		}
		// <statement> -> <id> = <expression> eol
		debug("expr analysis...");
		int code = expression_analysis(table);
		


		debug("end of expr");
		if(code==NO_ERR)
		{
			debug("expr OK");
			if(generate==true)
			{
				generate=false;
				tmp=GMalloc(sizeof(symbol));
				tmp->id=GMalloc(sizeof(char)*strlen(token_id.string_val));
				strcpy(tmp->id,token_id.string_val);
				tmp->frame=SYM_LF;
				
				
				tmp->immediate=false;
				tmp->type=SYM_NONE;
				symtable_add(table,token_id.string_val,tmp);
				debugn("gen_definition");
				if(if_while_active)
					gencontrol_redirect_output(&program);

				gen_definition(tmp);

				if(if_while_active)
					gencontrol_redirect_output(&if_while_dstring);
			}
			tmp->frame=SYM_LF;
			tmp->immediate=false;
			tmp->type=SYM_NONE;
			symbol expr_result_sym = {.id="$expr_result", .immediate=false, .frame=SYM_GF};
			debug("gen_assignment");
			gen_assignment(tmp,&expr_result_sym);
			
			RETURN_ERROR( NO_ERR);
			
		}
		else
		{
			debug("expr ERR");
			return code;
		}
	}
	else
	{
		// <statement> -> <expression> eol
		debug("<expression>");
		ListPush(&tokensList,token_tmp);
		token_tmp=token_id;
		int code = expression_analysis(table);
		if(code==NO_ERR)
		{
			
				if(table==&table_global)
				{
					RETURN_ERROR( NO_ERR);
					
				}
				else
				{
					RETURN_ERROR( NO_ERR);
				}
			
		}
		else
		{

			return code;
		}

	}
}

//LL Rule
// <statement> -> <id> = <func-id>(<arg-list-first>) eol
int statement_id_func(symtable *table, symbol *fce, symbol* id)
{
	debug("<statement> -> <id> = <func-id>(<arg-list>) eol");

	get_next_token();

	bool brackets=false;
	if(token_tmp.type==TOKEN_LB)
	{
		brackets=true;
		get_next_token();
	}
	
	gen_call_prepare();
	debug(fce->id);
	//symtable_print(table);
	int code = arg_list_first(fce,table,brackets);
	
	if(code!=NO_ERR)
	{
		return code;
	}
	gen_call(fce->id);
	symbol expr_result_sym = {.id="$expr_result", .immediate=false, .frame=SYM_GF};
	
	debug(id->id);
	if(id!=NULL)
	{
		debug("gen_assignment");
		gen_assignment(id,&expr_result_sym);
	}
	
		RETURN_ERROR( NO_ERR);
}
// LL Rule
// <statement> -> <func-id>(<arg-list-first>) eol
int statement_func(symtable *table, symbol *fce)
{
	debug("<statement> -> <func-id>(<arg-list>) eol");
	get_next_token();
	bool brackets=false;
	if(token_tmp.type==TOKEN_LB)
	{
		brackets=true;
		get_next_token();
	}
	
	gen_call_prepare();
	int code = arg_list_first(fce,table,brackets);

	if(code!=NO_ERR)
	{
		return code;
	}

	gen_call(fce->id);

	
	RETURN_ERROR( NO_ERR);

}

// If token is keyword, this function determine next rule
int statement_kw(symtable *table){
	int code;
	switch(token_tmp.keyword_val)
	{
		case KW_IF:return statement_if(table);break;
		case KW_WHILE:return statement_while(table);break;
		case KW_NIL:
		code = expression_analysis(table);
		if(code != NO_ERR)
		{
			RETURN_ERROR(code);
		}
		RETURN_ERROR( NO_ERR);
		break;
		default:RETURN_ERROR(SYNTAX_ERR);
	}
}
// LL Rule
// <statement> -> if <expression> then eol <statement-list>
int statement_if(symtable *table){
	debug("if <expression> then EOL");
	//printf("token:%d\n",token_tmp.type);
	//printf("token:%d\n",token_tmp.keyword_val);
	
	char str[12];
	sprintf(str, "%d", counter++);
	debugn("counter:");
	debug(str);

	if_while_active++;

	gencontrol_redirect_output(&if_while_dstring);
	gen_if(str);
	get_next_token();

	int check = check_exp_start();
	if(check!=NO_ERR)
	{
		debug("check exp start ERR");
		return check;
	}

	int code = expression_analysis(table);

	if(code != NO_ERR)
	{
		return code;
	}


	if(token_tmp.type!=TOKEN_KEYWORD || token_tmp.keyword_val!=KW_THEN)
	{
		RETURN_ERROR(SYNTAX_ERR);
	};

	gen_then(str);

	get_next_token();
	if(token_tmp.type!=TOKEN_EOL)
	{
		RETURN_ERROR(SYNTAX_ERR);
	};

	get_next_token();
	code = statement_list(table);
	
	if(code!=NO_ERR)
	{
		return code;
	};

	

	while(token_tmp.type==TOKEN_EOL){
		get_next_token();
	}



	if(token_tmp.type == TOKEN_KEYWORD && token_tmp.keyword_val == KW_END)
	{
		// <statement> -> if <expression> then eol <statement-list> end eol
		gen_else(str);
		gen_end_if(str);
		if_while_active--;
		if(if_while_active==0)
		{
			dstring_add(&program,if_while_dstring.data);
			dstring_free(&if_while_dstring);
			dstring_init(&if_while_dstring);
			gencontrol_redirect_output(&program);
		}	
		get_next_token();
		if(token_tmp.type!=TOKEN_EOL)
		{
			RETURN_ERROR(SYNTAX_ERR);
		}

		RETURN_ERROR( NO_ERR);
	}
	// <statement> -> if <expression> then eol <statement-list> else eol <statement-list> end eol
	if(token_tmp.type != TOKEN_KEYWORD || token_tmp.keyword_val != KW_ELSE)
	{
		RETURN_ERROR(SYNTAX_ERR);
	}
	gen_else(str);
	get_next_token();
	code = statement_list(table);

	if(code!=NO_ERR)
	{
		return code;
	}

	if(token_tmp.type == TOKEN_KEYWORD && token_tmp.keyword_val == KW_END)
	{
		gen_end_if(str);
		if_while_active--;
		if(if_while_active==0)
		{
			dstring_add(&program,if_while_dstring.data);
			dstring_free(&if_while_dstring);
			dstring_init(&if_while_dstring);
			gencontrol_redirect_output(&program);
		}	
		get_next_token();
		if(token_tmp.type!=TOKEN_EOL)
		{
			RETURN_ERROR(SYNTAX_ERR);
		}
		
		RETURN_ERROR( NO_ERR);


	}
	else
	{
		RETURN_ERROR(SYNTAX_ERR);
	}
	

}
// LL Rule
// <statement> -> while <expression> do eol <statement-list> end eol
int statement_while(symtable *table){
	debug("while <expression> do");
	char str[12];
	sprintf(str, "%d", counter++);

	if_while_active++;

	gencontrol_redirect_output(&if_while_dstring);
	get_next_token();
	gen_while(str);

	int check = check_exp_start();
	if(check!=NO_ERR)
	{
		debug("check exp start ERR");
		return check;
	}

	int code = expression_analysis(table);

	if(code != NO_ERR)
	{
		return code;
	}
	


	if(token_tmp.type!=TOKEN_KEYWORD || token_tmp.keyword_val!=KW_DO)
	{
		RETURN_ERROR(SYNTAX_ERR);
	};

	get_next_token();
	if(token_tmp.type!=TOKEN_EOL)
	{
		RETURN_ERROR(SYNTAX_ERR);
	};

	
	
	get_next_token();
	gen_do(str);

	code = statement_list(table);
	
	if(code!=NO_ERR)
	{
		return code;
	};

	gen_end_while(str);
	if_while_active--;
	if(if_while_active==0)
	{
		dstring_add(&program,if_while_dstring.data);
		dstring_free(&if_while_dstring);
		dstring_init(&if_while_dstring);
		gencontrol_redirect_output(&program);
	}	

	if(token_tmp.type != TOKEN_KEYWORD || token_tmp.keyword_val != KW_END)
	{
		RETURN_ERROR(SYNTAX_ERR);
	}
	get_next_token();
	if(token_tmp.type!=TOKEN_EOL)
	{
		RETURN_ERROR(SYNTAX_ERR);
	}
	RETURN_ERROR( NO_ERR);
	
	
}
// LL Rule
// <statement> -> print(<print-arg-list-first>) eol

int statement_print(symtable *table){
	debug("<statement> -> print");
	//symtable_print(table);
	// Always returns nil@nil
	symbol expr_result_sym = {.id="$expr_result", .frame=SYM_GF, .immediate=false};
	symbol nil_sym = {.immediate=true, .type=SYM_NONE};
	gen_assignment(&expr_result_sym, &nil_sym);

	// Load next token after "print"
	get_next_token();

	// No parentheses used for function call (valid)
	if (token_tmp.type == TOKEN_IDENTIFIER || token_tmp.type == TOKEN_INT || token_tmp.type == TOKEN_FLOAT || token_tmp.type == TOKEN_STRING)
		return print_arg_list_first(table);

	// Parentheses used for function call (valid)
	if (token_tmp.type == TOKEN_LB)
	{
		get_next_token();
		return print_arg_list_first(table);
	}

	RETURN_ERROR(SYNTAX_ERR);
}



int arg_list_first(symbol *fce, symtable *table, bool brackets)
{
	//Seznam požadovaných agrumentů funkce uložená v params->first->data
	List *params=fce->params;
	int len= ListLen(params);
	ElementPtr tmp=params->First;
	debugN(len);

	if(len==0)
	{
		if(token_tmp.type==TOKEN_IDENTIFIER || token_tmp.type==TOKEN_FLOAT || token_tmp.type==TOKEN_INT || token_tmp.type==TOKEN_STRING)
		{
			RETURN_ERROR(SEM_PAR_ERR);
		}
		if(token_tmp.type != TOKEN_EOL && brackets==0)
		{
			RETURN_ERROR(SYNTAX_ERR);
		}
		if(token_tmp.type != TOKEN_KEYWORD && token_tmp.keyword_val==KW_END)
		{
			RETURN_ERROR(SYNTAX_ERR);
		}
		if(token_tmp.type != TOKEN_KEYWORD && token_tmp.keyword_val==KW_ELSE)
		{
			RETURN_ERROR(SYNTAX_ERR);
		}
		if(brackets)
		{
			if(token_tmp.type==TOKEN_RB)
			{
				RETURN_ERROR(NO_ERR);
			}
			else
			{
				RETURN_ERROR(SYNTAX_ERR);
			}
		}
		else
		{
			RETURN_ERROR(NO_ERR);
		}
		
	}
	
	if (len!=0 && (token_tmp.type==TOKEN_EOL ||  token_tmp.type==TOKEN_RB))
	{
		RETURN_ERROR(SEM_PAR_ERR);
	}
	
	
	if(token_tmp.type==TOKEN_IDENTIFIER)
	{
		symbol *s1=symtable_find(table,token_tmp.string_val);
		gen_call_arg(tmp->data, s1);
		if(s1==NULL)
		{
			RETURN_ERROR(SEM_DEF_ERR);
		}
		
		get_next_token();
		return arg_list(fce,table,len-1,brackets,tmp->ptr);
	}
	else if(token_tmp.type==TOKEN_INT || token_tmp.type==TOKEN_FLOAT || token_tmp.type==TOKEN_STRING)
	{
		// Generate code
		symbol imm_sym=symbol_from_token(&token_tmp);
		gen_call_arg(tmp->data, &imm_sym);

		get_next_token();
		return arg_list(fce,table,len-1,brackets,tmp->ptr);
	}
	else
	{
		RETURN_ERROR(SEM_PAR_ERR);
	}


}

int arg_list(symbol *fce, symtable *table, int count, bool brackets, ElementPtr tmp)
{
	

	if(token_tmp.type==TOKEN_COMMA)
	{
		get_next_token();
		ListPush(&tokensList,token_tmp);
		if(token_tmp.type!=TOKEN_IDENTIFIER && token_tmp.type!=TOKEN_STRING && token_tmp.type!=TOKEN_INT && token_tmp.type!=TOKEN_FLOAT)
		{
			if(count==0)
			{
				RETURN_ERROR(SYNTAX_ERR);
			}
			else
			{
				RETURN_ERROR(SEM_PAR_ERR);
			}
			
		}
	}
	else
	{
		if(brackets)
		{
			if(count==0)
			{
				if(token_tmp.type==TOKEN_RB)
				{
					RETURN_ERROR(NO_ERR);
				}
				else
				{
					RETURN_ERROR(SYNTAX_ERR);
				}
			}
			else
			{
				RETURN_ERROR(SEM_PAR_ERR);
			}
		}
		else
		{
			if(count==0)
			{
				if(token_tmp.type==TOKEN_EOL)
				{
					RETURN_ERROR(NO_ERR);
				}
				else
				{
					RETURN_ERROR(SYNTAX_ERR);
				}
			}
			else
			{
				RETURN_ERROR(SEM_PAR_ERR);
			}
		}
		
	}

	get_next_token();
	if(token_tmp.type==TOKEN_IDENTIFIER)
	{
		if(count==0)
		{
			RETURN_ERROR(SEM_PAR_ERR);
		}
		symbol *s1=symtable_find(table,token_tmp.string_val);
		if(s1==NULL)
		{
			RETURN_ERROR(SEM_DEF_ERR);
		}
		gen_call_arg(tmp->data, s1);
		
		get_next_token();
		return arg_list(fce,table,count-1,brackets,tmp->ptr);
	}
	else if(token_tmp.type==TOKEN_INT || token_tmp.type==TOKEN_FLOAT || token_tmp.type==TOKEN_STRING)
	{

		if(count==0)
		{
			RETURN_ERROR(SEM_PAR_ERR);
		}

		// Generate code
		symbol imm_sym=symbol_from_token(&token_tmp);
		gen_call_arg(tmp->data, &imm_sym);

		get_next_token();
		return arg_list(fce,table,count-1,brackets,tmp->ptr);
	}
	else
	{
		RETURN_ERROR(SYNTAX_ERR);
	}



}

int print_arg_list_first(symtable *table)
{
	debug("print arg list first");
	// Right bracket (or eol) would mean zero arguments (semantically wrong)
	if (token_tmp.type == TOKEN_RB || token_tmp.type == TOKEN_EOL || token_tmp.type == TOKEN_EOF)
		RETURN_ERROR(SEM_PAR_ERR);

	// Argument must be ID/literal
	if (token_tmp.type != TOKEN_IDENTIFIER && token_tmp.type != TOKEN_INT && token_tmp.type != TOKEN_FLOAT && token_tmp.type != TOKEN_STRING)
		RETURN_ERROR(SYNTAX_ERR);
	
	// ID must be defined
	if (token_tmp.type == TOKEN_IDENTIFIER)
	{
		debug(token_tmp.string_val);
		symbol *arg = symtable_find(table, token_tmp.string_val);
		if (!arg)
			RETURN_ERROR(SEM_DEF_ERR);
		// Generate code
		gen_write(arg);
	}
	else
	{
		// Generate code

		symbol imm_sym = symbol_from_token(&token_tmp);
		gen_write(&imm_sym);
	}

	// Continue to the next argument
	return print_arg_list(table);
}

int print_arg_list(symtable *table)
{
	// This should be a comma (,), right bracket, eol or eof
	get_next_token();

	// End of function call (valid)
	if (token_tmp.type == TOKEN_RB || token_tmp.type == TOKEN_EOL || token_tmp.type == TOKEN_EOF)
	{
		RETURN_ERROR(NO_ERR);
	}

	// Comma means another argument
	if (token_tmp.type != TOKEN_COMMA)
		RETURN_ERROR(SYNTAX_ERR);

	// Load argument
	get_next_token();

	// Argument must be ID/literal
	if (token_tmp.type != TOKEN_IDENTIFIER && token_tmp.type != TOKEN_INT && token_tmp.type != TOKEN_FLOAT && token_tmp.type != TOKEN_STRING)
		RETURN_ERROR(SYNTAX_ERR);

	// ID must be defined
	debug("print next argument");
	if (token_tmp.type == TOKEN_IDENTIFIER)
	{
		symbol *id = symtable_find(table, token_tmp.string_val);
		if (!id)
			RETURN_ERROR(SEM_DEF_ERR);

		// Generate code
		gen_write(id);
	}
	else
	{
		// Generate code
		symbol imm_sym = symbol_from_token(&token_tmp);
		gen_write(&imm_sym);
	}

	// Continue to the next argument
	return print_arg_list(table);
}


