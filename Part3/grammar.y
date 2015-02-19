/* Changes:  */

/* 1. Character constants removed */
/* 2. Changed INTCONSTANT to INT_CONSTANT */
/* 3. Changed the production for constant_expression to include FLOAT_CONSTANT */
/* 4. Added examples of FLOAT_CONSTANTS */
/* 5. Added the description of STRING_LITERAL */
/* 6. Changed primary_expression and FOR */
/* 7. The grammar permits a empty statement. This should be  */
/*    explicitly represented in the AST. */
/* 8. To avoid local decl inside blocks, a rule for statement  */
/*    has been changed. */

/* ----------------------------------------------------------------------- */

/* start symbol is translation_unit */

/* ---------------------------------------------------- */
%scanner Scanner.h
%scanner-token-function d_scanner.lex()


%token VOID INT FLOAT FLOAT_CONSTANT INT_CONSTANT AND_OP OR_OP EQ_OP NE_OP LE_OP GE_OP STRING_LITERAL IF ELSE WHILE FOR RETURN IDENTIFIER INC_OP

%polymorphic ast:abstract_astnode*; stringTag:std::string; listTag:std::list<abstract_astnode*>

%type <ast> expression l_expression primary_expression postfix_expression unary_expression multiplicative_expression additive_expression 
iteration_statement selection_statement statement function_definition translation_unit logical_and_expression
equality_expression relational_expression assignment_statement compound_statement constant_expression

%type <listTag> expression_list statement_list

%type <stringTag> unary_operator IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL INC_OP


%%

translation_unit
	: function_definition 
	{
		//($1)->abstract_astnode::print();
	}
	| translation_unit function_definition 
        ;

function_definition
	: type_specifier fun_declarator compound_statement 
	{
		($3)->print();
	}
	;

type_specifier
	: VOID 	
        | INT   
	| FLOAT 
        ;

fun_declarator
	: IDENTIFIER '(' parameter_list ')' 
        | IDENTIFIER '(' ')' 
	;

parameter_list
	: parameter_declaration 
	| parameter_list ',' parameter_declaration 
	;

parameter_declaration
	: type_specifier declarator 
        ;

declarator
	: IDENTIFIER 
	| declarator '[' constant_expression ']' 
        ;

constant_expression 
        : INT_CONSTANT 
        {
        	$$ =  new IntConstAst($1);
        }
        | FLOAT_CONSTANT 
        {
        	$$ = new FloatConstAst($1);
        }
        ;

compound_statement
	: '{' '}' 
	{
		$$ = new BlockAst(std::list<abstract_astnode*>());
	}
	| '{' statement_list '}' 
	{
		$$ = new BlockAst($2);
	}
    | '{' declaration_list statement_list '}' 
    {
    	$$ = new BlockAst($3);// doubt
    }
	;

statement_list
	: statement
	{
		$$ = std::list<abstract_astnode*>(1,$1);
	}
        | statement_list statement	
        {
        	$$ = $1;
        	($$).push_back($2);
        }
	;

statement
        : '{' statement_list '}'  //a solution to the local decl problem
        {
        	$$ = new BlockAst($2);
        }
        | selection_statement 	
        {
        	$$ = $1;
        }
        | iteration_statement
        {
        	$$ = $1;
        } 	
		| assignment_statement	
		{
			$$ = $1;
		}
        | RETURN expression ';'
        {
        	$$ = new ReturnAst($2);
        }	
        ;

assignment_statement
	: ';'
	{
		$$ = new EmptyAst();
	} 								
	|  l_expression '=' expression ';'	
	{
		$$ = new OpAst(std::string("ASSIGNMENT"), $1, $3);
	}
	;

expression
        : logical_and_expression 
        {
        	$$ = $1;
        }
        | expression OR_OP logical_and_expression
        {
        	$$ = new OpAst("OR",$1,$3);
        }
	;

logical_and_expression
        : equality_expression
        {
        	$$ = $1;
        }
        | logical_and_expression AND_OP equality_expression 
        {
        	$$ = new OpAst(std::string("AND"),$1,$3);
        }
	;

equality_expression
	: relational_expression
	{
		$$ =$1;
	} 
    | equality_expression EQ_OP relational_expression
    {
    	$$ = new OpAst(std::string("EQ_OP"),$1,$3);
    } 	
	| equality_expression NE_OP relational_expression
    {
    	$$ = new OpAst(std::string("NE_OP"),$1,$3);
    } 	
	;

relational_expression
	: additive_expression
	{
		$$ = $1;
	}
    | relational_expression '<' additive_expression
    {
    	$$ = new OpAst(std::string("LT"),$1,$3);
    } 
	| relational_expression '>' additive_expression 
    {
    	$$ = new OpAst(std::string("GT"),$1,$3);
    } 
	| relational_expression LE_OP additive_expression 
    {
    	$$ = new OpAst(std::string("LE_OP"),$1,$3);
    } 
    | relational_expression GE_OP additive_expression 
    {
    	$$ = new OpAst(std::string("GE_OP"),$1,$3);
    } 
	;

additive_expression 
	: multiplicative_expression
	{
		$$ = $1;
	}
	| additive_expression '+' multiplicative_expression
	{
		$$ = new OpAst(std::string("PLUS"),$1,$3);
	} 
	| additive_expression '-' multiplicative_expression
	{
		$$ = new OpAst(std::string("MINUS"),$1,$3);
	} 
	;

multiplicative_expression
	: unary_expression
	{
		$$ = $1;
	}
	| multiplicative_expression '*' unary_expression 
	{
		$$ = new OpAst(std::string("MULT"),$1,$3);
	}
	| multiplicative_expression '/' unary_expression 
	{
		$$ = new OpAst(std::string("DIVIDE"),$1,$3);
	}
	;

unary_expression
	: postfix_expression
	{
		$$ = $1;
	} 	
	| unary_operator postfix_expression
	{
		$$ = new UnopAst($1,$2);
	} 
	;

postfix_expression
	: primary_expression
	{
		$$ = $1;
	}
    | IDENTIFIER '(' ')'
    {
    	$$ = new FuncCallAst($1,list<abstract_astnode*>());
    }
	| IDENTIFIER '(' expression_list ')' 
	{
		$$ = new FuncCallAst($1, $3);
	}
	| l_expression INC_OP
	{
		$$ = new UnopAst("PP",$1);
	}
	;

primary_expression
	: l_expression
	{
		$$ = $1;
	}
    | l_expression '=' expression // added this production
    {
    	$$ = new AssAst($1,$3);
    }
	| INT_CONSTANT
	{
		$$ = new IntConstAst($1);
	}
	| FLOAT_CONSTANT
	{
		$$ = new FloatConstAst($1);
	}
    | STRING_LITERAL
    {
		$$ = new StringConstAst($1);
    }
	| '(' expression ')' 	
	{
		$$ = $2;
	}
	;

l_expression
        : IDENTIFIER
        {
        	$$ = new IdentifierAst($1);
        }
        | l_expression '[' expression ']'
        {
            $$ = new IndexAst($1, $3);
        }
        ;

expression_list
        : expression
        {
        	$$ = std::list<abstract_astnode*>(1,$1);
        }
        | expression_list ',' expression
        {
        	$$ = $1;
        	($$).push_back($3);
        }
        ;

unary_operator
        : '-'
        {
        	$$ = "UMINUS";
        }	
		| '!'
		{
			$$ = "NOT";
		}
		;

selection_statement
        : IF '(' expression ')' statement ELSE statement 
        {
        	$$ = new IfAst($3,$5,$7);
        }
	;

iteration_statement
	: WHILE '(' expression ')' statement
	{
		$$ = new WhileAst($3,$5);
	} 	
        | FOR '(' expression ';' expression ';' expression ')' statement  //modified this production
        {
 			$$= new ForAst($3, $5, $7, $9);       	
        }
        ;

declaration_list
        : declaration  					
        | declaration_list declaration
	;

declaration
	: type_specifier declarator_list';'
	;

declarator_list
	: declarator
	| declarator_list ',' declarator 
	;


/* A description of integer and float constants. Not part of the grammar.   */

/* Numeric constants are defined as:  */

/* C-constant: */
/*   C-integer-constant */
/*   floating-point-constant */
 
/* C-integer-constant: */
/*   [1-9][0-9]* */
/*   0[bB][01]* */
/*   0[0-7]* */
/*   0[xX][0-9a-fA-F]* */
 
/* floating-point-constant: */
/*   integer-part.[fractional-part ][exponent-part ] */

/* integer-part: */
/*   [0-9]* */
 
/* fractional-part: */
/*   [0-9]* */
 
/* exponent-part: */
/*   [eE][+-][0-9]* */
/*   [eE][0-9]* */

/* The rule given above is not entirely accurate. Correct it on the basis of the following examples: */

/* 1. */
/* 23.1 */
/* 01.456 */
/* 12.e45 */
/* 12.45e12 */
/* 12.45e-12 */
/* 12.45e+12 */

/* The following are not examples of FLOAT_CONSTANTs: */

/* 234 */
/* . */

/* We have not yet defined STRING_LITERALs. For our purpose, these are */
/* sequence of characters enclosed within a pair of ". If the enclosed */
/* sequence contains \ and ", they must be preceded with a \. Apart from */
/* \and ", the only other character that can follow a \ within a string */
/* are t and n.  */


