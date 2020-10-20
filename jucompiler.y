%{

    /*
        Luís Miguel Cerqueira Freitas 2017264549
        João Monte Monteiro 2016244006
    */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "y.tab.h"
#include "ast.h"

extern int linha, coluna, yyleng, prevline, prevcol,seof,eofl,eofc;
extern char* yytext;
bool erro = false;
int yylex(void);
void yyerror(char *s);
char guardador[80];
int lt,ct;
struct n* tree;

struct type_struct{
	int line,col;
	const char*type;
}
;

%}
%union {
	struct n* no;
	struct type_struct* t;	
	struct _args* args;
};


%token<args> AND
%token<args> ASSIGN
%token<args> STAR
%token<args> COMMA
%token<args> DIV
%token<args> EQ
%token<args> GE
%token<args> GT
%token<args> LBRACE
%token<args> LE
%token<args> LPAR
%token<args> LSQ
%token<args> LT
%token<args> MINUS
%token<args> MOD
%token<args> NE
%token<args> NOT
%token<args> OR
%token<args> PLUS
%token<args> RBRACE
%token<args> RPAR
%token<args> RSQ
%token<args> SEMICOLON
%token<args> ARROW
%token<args> LSHIFT
%token<args> RSHIFT
%token<args> XOR
%token<args> DOT
%token<args> BOOL
%token<args> CLASS
%token<args> DOTLENGTH
%token<args> DOUBLE
%token<args> ELSE
%token<args> IF
%token<args> INT
%token<args> PRINT
%token<args> PARSEINT
%token<args> PUBLIC
%token<args> RETURN
%token<args> STATIC
%token<args> STRING
%token<args> VOID
%token<args> WHILE
%token<args> ASPAS
%token<args> ENTER
%token<args> TAB
%token<args> INVALID_ESCAPE
%token<args> SPACE
%token<args> EXPOENTE
%token<args> INTLIT
%token<args> ID
%token<args> REALLIT
%token<args> RESERVED
%token<args> STRLIT
%token<args> BOOLLIT

%left COMMA
%right ASSIGN
%left OR
%left AND
%left LT GT GE LE EQ NE
%left XOR LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right NOT
%nonassoc IFX
%nonassoc ELSE


%type <no> Program Declarations MethodDecl FieldDecl MethodHeader MethodParams VarsAndStatements MethodBody VarDecl StatementList Statement  MethodInvocationList MethodInvocation Assignment ParseArgs Expr Expr_assig
%type <t> Type

%%

Program: CLASS ID LBRACE Declarations RBRACE {


tree = create_node("Program", get_line($1),get_col($1)); 
add_son(tree, new_id($<args->val>2,get_line($2),get_col($2)));
add_son(tree, $4); 
$$ = tree;}
;          

Declarations: 
{$$ = tree;}
			| Declarations SEMICOLON{
$$=$1;
}

			| Declarations MethodDecl 							{if ($1 != NULL){$$ = $1; add_next($1, $2);} else {$$ = $2;}}
			| Declarations FieldDecl SEMICOLON 							{if ($1 != NULL){$$ = $1; add_next($1, $2);} else {$$ = $2;}}
			
			;

MethodDecl:PUBLIC STATIC MethodHeader MethodBody	
{
	$$ = create_node("MethodDecl",get_line($2),get_col($2));
	
	add_son($$, $3);

	add_son($$, $4); 
}
;




FieldDecl: PUBLIC STATIC Type ID {
	$$ = create_node("FieldDecl",get_line($4),get_col($4));
	add_son($$,create_node($<t->type>3,$<t->line>3,$<t->col>3));
	sprintf(guardador,"%s",$<t->type>3);
	lt=$<t->line>3;
	ct=$<t->col>3;
	add_son($$, new_id($<args->val>4,get_line($4),get_col($4)));}
|FieldDecl COMMA ID {$$ = $1;
	node*temp=create_node("FieldDecl",get_line($3),get_col($3));
	add_son(temp,create_node(guardador,lt,ct));
	add_son(temp, new_id($<args->val>3,get_line($3),get_col($3)));
	add_next($$,temp);
}
|error SEMICOLON{
$$=NULL;}

;



Type: 	BOOL 																		{$<t->type>$ = "Bool";$<t->line>$=get_line($1);$<t->col>$=get_col($1);}
	|INT 																		{$<t->type>$ = "Int";$<t->line>$=get_line($1);$<t->col>$=get_col($1);}
	| DOUBLE 																	{$<t->type>$ ="Double";$<t->line>$=get_line($1);$<t->col>$=get_col($1);}
	;


MethodHeader : Type ID LPAR RPAR{
	$$=create_node("MethodHeader",get_line($2),get_col($2));
	add_son($$, create_node($<t->type>1,$<t->line>1,$<t->col>1));
	add_son($$, new_id($<args->val>2,get_line($2),get_col($2)));
	node* formalparam = create_node("MethodParams",get_line($3),get_col($3)); 
	add_son($$, formalparam);
}


|Type ID LPAR MethodParams RPAR{
$$=create_node("MethodHeader",get_line($2),get_col($2));
	add_son($$, create_node($<t->type>1,$<t->line>1,$<t->col>1));
	add_son($$, new_id($<args->val>2,get_line($2),get_col($2)));
	node* formalparam = create_node("MethodParams",get_line($3),get_col($3)); 
	add_son($$, formalparam);
	add_son(formalparam, $4);
}


|VOID ID LPAR RPAR{
$$=create_node("MethodHeader",get_line($2),get_col($2));
	add_son($$, create_node("Void",get_line($1),get_col($1)));
	add_son($$, new_id($<args->val>2,get_line($2),get_col($2)));
	node* formalparam = create_node("MethodParams",get_line($3),get_col($3)); 
	add_son($$, formalparam); 
}


|VOID ID LPAR MethodParams RPAR{
$$=create_node("MethodHeader",get_line($2),get_col($2));
	add_son($$, create_node("Void",get_line($1),get_col($1)));
	add_son($$, new_id($<args->val>2,get_line($2),get_col($2)));
	node* formalparam = create_node("MethodParams",get_line($3),get_col($3)); 
	add_son($$, formalparam);
	add_son(formalparam, $4); 
}

;

MethodParams: Type ID 															{
$$ = create_node("ParamDecl",get_line($2),get_col($2)); 
add_son($$, create_node($<t->type>1,$<t->line>1,$<t->col>1)); 
add_son($$, new_id($<args->val>2,get_line($2),get_col($2)));
}
			| MethodParams COMMA Type ID {
$$ = $1; 
node* params = create_node("ParamDecl",get_line($4),get_col($4)); 
add_son(params, create_node($<t->type>3,$<t->line>3,$<t->col>3)); 
add_son(params, new_id($<args->val>4,get_line($4),get_col($4)));
add_next($$, params);
}
			|STRING LSQ RSQ ID{
$$ = create_node("ParamDecl",get_line($4),get_col($4)); 
node* str = create_node("StringArray",get_line($1),get_col($1));
add_son($$, str); 
add_son($$, new_id($<args->val>4,get_line($4),get_col($4)));
}
;


VarsAndStatements: {$$ = NULL;}
 | VarsAndStatements VarDecl SEMICOLON  				{
if ($1 != NULL) {
$$ = $1; 
add_next($1, $2);
} 
else {
$$ = $2;}
}
 | VarsAndStatements Statement 					{
if ($1 != NULL) 
{$$ = $1; 
add_next($1, $2);
} else {
$$ = $2;}
}
;

MethodBody: LBRACE VarsAndStatements RBRACE{
if($2!=NULL){
$$ = create_node("MethodBody",get_line($1),get_col($1));
add_son($$, $2);}
else{
$$ = create_node("MethodBody",get_line($1),get_col($1));
}
}
;



VarDecl: Type ID{
$$ = create_node("VarDecl",$<t->line>1,$<t->col>1);
add_son($$, create_node($<t->type>1,$<t->line>1,$<t->col>1));
sprintf(guardador,"%s",$<t->type>1);
lt=$<t->line>1;
ct=$<t->col>1;
add_son($$, new_id($<args->val>2,get_line($2),get_col($2)));
}

|VarDecl COMMA ID{
	$$=$1;
	node*temp=create_node("VarDecl",$<t->line>1,$<t->col>1);
	add_son(temp, create_node(guardador,lt,ct));
	add_son(temp, new_id($<args->val>3,get_line($3),get_col($3)));
	add_next($$,temp);
}

;

StatementList:{$$=NULL;}

		|StatementList Statement{
		if($1!=NULL){
			$$=$1;
			add_next($$,$2);
		}
		else{
			$$=$2;		
		}

};




Statement:
LBRACE StatementList RBRACE 	{

if (conta_irmaos($2) <= 1) {
      $$ = $2;
    } else {
      $$ = create_node("Block",get_line($1),get_col($1));
      add_son($$, $2);
    }
  

}
			| IF LPAR Expr_assig RPAR Statement %prec IFX{
	
				$$ = create_node("If",get_line($1),get_col($1)); 
				if($5==NULL){
					node*temp=create_node("Block",get_line($4),get_col($4));
					add_next($3,temp);
				}
				else{
					add_next($3,$5);
				}
				node*temp2=create_node("Block",get_line($4),get_col($4));
				add_next($3,temp2);
				add_son($$,$3);

}

			| IF LPAR Expr_assig RPAR Statement ELSE Statement{


				$$ = create_node("If",get_line($1),get_col($1)); 
				add_son($$, $3);

    				if($5==NULL){
					node*temp=create_node("Block",get_line($4),get_col($4));
					add_next($3,temp);
				}else{
					add_next($3,$5);
				}

    			
    				if($7==NULL){
					node*temp=create_node("Block",get_line($6),get_col($6));
					add_son($$,temp);
				}
				else{
					add_son($$,$7);
				}




}

			| WHILE LPAR Expr_assig RPAR Statement {
$$ = create_node("While",get_line($1),get_col($1)); 
add_son($$, $3);
if($5 !=NULL){
add_son($$, $5);
}else{
add_son($$, create_node("Block",get_line($2),get_col($2)));
}
}

|RETURN SEMICOLON{
$$ = create_node("Return",get_line($2),get_col($2));
}
|RETURN Expr_assig SEMICOLON{

$$ = create_node("Return",get_line($1),get_col($1)); 
add_son($$, $2);
}

| MethodInvocation SEMICOLON{

{$$ = $1;}
}
| Assignment SEMICOLON{

{$$ = $1;}
}
| ParseArgs SEMICOLON{

{$$ = $1;}
}
|PRINT LPAR STRLIT RPAR SEMICOLON{
$$=create_node("Print",get_line($1),get_col($1)); 
char arr[1000]; 
sprintf(arr, "StrLit(%s)", $<args->val>3);
add_son($$,create_node(arr,get_line($3),get_col($3)));
 
}
|PRINT LPAR Expr_assig RPAR SEMICOLON{

$$ = create_node("Print",get_line($1),get_col($1)); add_son($$, $3);
}

|SEMICOLON{
$$ = NULL; 
}


|error SEMICOLON{

$$=NULL;};



MethodInvocationList: Expr_assig 														{$$ = $1;}
				| MethodInvocationList COMMA Expr_assig  								{$$ = $1; add_next($1, $3);}
				;
MethodInvocation: ID LPAR RPAR 													{
$$ = create_node("Call",get_line($1),get_col($1)); 
add_son($$, new_id($<args->val>1,get_line($1),get_col($1)));}
				|ID LPAR MethodInvocationList RPAR 								{$$ = create_node("Call",get_line($1),get_col($1)); add_son($$, new_id($<args->val>1,get_line($1),get_col($1))); add_son($$, $3);}
				| ID LPAR error RPAR 											{$$ = NULL;}
				;

 				

Assignment: ID ASSIGN Expr_assig{
	node* ass = create_node("Assign",get_line($2),get_col($2)); 
	add_son(ass, new_id($<args->val>1,get_line($1),get_col($1))); 
	add_son(ass, $3);
	$$=ass;
};



ParseArgs : PARSEINT LPAR ID LSQ Expr_assig RSQ RPAR{
	node* temp = create_node("ParseArgs",get_line($3),get_col($3)); 
	add_son(temp, new_id($<args->val>3,get_line($3),get_col($3))); 
	add_son(temp, $5); 
	$$ = temp;
}	|PARSEINT LPAR error RPAR	{
$$=NULL;};

Expr_assig: Assignment 		{$$ = $1;}
		|Expr	{$$ = $1;};



Expr:  Expr PLUS Expr 									{node* add = create_node("Add",get_line($2),get_col($2)); add_son(add, $1); add_son(add, $3); $$ = add;}
	| Expr MINUS Expr 								{node* sub   = create_node("Sub",get_line($2),get_col($2));    add_son(sub, $1); add_son(sub, $3); $$ = sub;}
	| Expr STAR Expr 								{node* mul   = create_node("Mul",get_line($2),get_col($2));    add_son(mul, $1); add_son(mul, $3); $$ = mul;}
	| Expr DIV Expr 								{node* div   = create_node("Div",get_line($2),get_col($2));    add_son(div, $1); add_son(div, $3); $$ = div;}
	| Expr MOD Expr									{node* mod   = create_node("Mod",get_line($2),get_col($2));    add_son(mod, $1); add_son(mod, $3); $$ = mod;}
	| Expr OR Expr 									{node* or    = create_node("Or",get_line($2),get_col($2));     add_son(or, $1);  add_son(or, $3);  $$ = or;}
	| Expr AND Expr 								{node* and   = create_node("And",get_line($2),get_col($2));    add_son(and, $1); add_son(and, $3); $$ = and;}
	| Expr XOR Expr 								{node* xor   = create_node("Xor",get_line($2),get_col($2));    add_son(xor, $1); add_son(xor, $3); $$ = xor;}
	| Expr LSHIFT Expr 								{node* lshift   = create_node("Lshift",get_line($2),get_col($2)); add_son(lshift, $1); add_son(lshift, $3); $$ = lshift;}
	| Expr RSHIFT Expr 								{node* rshift   = create_node("Rshift",get_line($2),get_col($2)); add_son(rshift, $1); add_son(rshift, $3); $$ = rshift;}
	| Expr EQ Expr 									{node* eq    = create_node("Eq",get_line($2),get_col($2));     add_son(eq, $1);  add_son(eq, $3);  $$ = eq;}
	| Expr GE Expr									{node* ge    = create_node("Ge",get_line($2),get_col($2));     add_son(ge, $1);  add_son(ge, $3);  $$ = ge;}
	| Expr GT Expr 									{node* gt    = create_node("Gt",get_line($2),get_col($2));     add_son(gt, $1);  add_son(gt, $3);  $$ = gt;}
	| Expr LE Expr 									{node* le    = create_node("Le",get_line($2),get_col($2));     add_son(le, $1);  add_son(le, $3);  $$ = le;}
	| Expr LT Expr 									{node* lt    = create_node("Lt",get_line($2),get_col($2));     add_son(lt, $1);  add_son(lt, $3);  $$ = lt;}
	| Expr NE Expr 									{node* ne    = create_node("Ne",get_line($2),get_col($2));     add_son(ne, $1);  add_son(ne, $3);  $$ = ne;}
	| MINUS Expr %prec NOT								{node* minus = create_node("Minus",get_line($1),get_col($1));  add_son(minus, $2);		  $$ = minus;}
	| NOT Expr %prec NOT								{node* not   = create_node("Not",get_line($1),get_col($1));    add_son(not, $2); 		  $$ = not;}
	| PLUS Expr %prec NOT								{node* plus  = create_node("Plus",get_line($1),get_col($1));   add_son(plus, $2); 		  $$ = plus;}
	| LPAR Expr RPAR 	{$$ = $2;}
	| LPAR error RPAR 	{$$ = NULL;}
	| MethodInvocation 	{$$ = $1;}
	| ParseArgs 		{$$ = $1;}
	| ID			{char arr[1000]; sprintf(arr, "Id(%s)", $<args->val>1); $$ = create_node(arr,get_line($1),get_col($1));}
	| ID DOTLENGTH		{char arr[1000]; sprintf(arr, "Id(%s)", $<args->val>1); $$ = create_node("Length",get_line($2),get_col($2));add_son($$,create_node(arr,get_line($1),get_col($1)));}
	| INTLIT 		{char arr[1000]; sprintf(arr, "DecLit(%s)", $<args->val>1); $$ = create_node(arr,get_line($1),get_col($1));}
	| REALLIT 		{char arr[1000]; sprintf(arr, "RealLit(%s)", $<args->val>1); $$ = create_node(arr,get_line($1),get_col($1));}
	| BOOLLIT		{char arr[1000]; sprintf(arr, "BoolLit(%s)", $<args->val>1); $$ = create_node(arr,get_line($1),get_col($1)); }
	;


%%


void yyerror (char *s){
	erro = true;
	if(seof>0){
		printf("Line %d, col %d: %s: %s\n", eofl, eofc, s, yytext);
	}
	else if (yychar == STRLIT){
		printf("Line %d, col %d: %s: %s\n", linha, (int) (coluna-strlen(yylval.args->val)), s, yylval.args->val);
	} else if ((yytext[strlen(yytext) - 1] == '\n' ) || (yytext[strlen(yytext) - 1] == '\r')){
		printf("Line %d, col %d: %s: %s\n", linha-1, prevcol, s, yytext);
	}
	else {
		printf("Line %d, col %d: %s: %s\n", linha, coluna-yyleng, s, yytext);
	}
}




