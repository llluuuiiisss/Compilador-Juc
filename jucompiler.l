%{
    /*
        Luís Miguel Cerqueira Freitas 2015227420
        João Monte Monteiro 2016244006
    */

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdarg.h>
    #include <stdbool.h>
    #include "y.tab.h"
    #include "semantics.h"


    int linha=1;
    int coluna=1;
    int auxC=1;
    int auxL=1;
    int flag=0;
    int flagReturn=0;
    char *letra;
    char *string;
    int prevcol=1;
    int prevline=1;
    int seof=0;
    int eofc;
    int eofl;
    int prevl;
    int prevc;
    int invalidScape=0;
    int invalid_s = 0;
    int erro_l = 0;
    int erro_c = 0;
    int erros_lex = 0;
    int semicol = 0;
    char strlit[4000];
    int flagt = 0;
    int comment_l = 0;
    int comment_c = 1;
    int erroS=0;
    int pos_inicial=0;
    extern node* tree;
    extern bool erro;
    extern table_t* table;
    void init_yylval(int line, int col, char* text);
%}


%X COMMENTP 
%X COMMENTL 
%X STRLIT2


BOOLLIT 	"true"|"false"
AND 		"&&"
ASSIGN 		"="
STAR 		"*"
COMMA 		","
DIV  		"/"
EQ  		"=="
GE  		">="
GT 		">"
LBRACE  	"{"
LE  		"<="
LPAR 		"("
LSQ 		"["
LT 		"<"
MINUS 		"-"
MOD 		"%"
NE 		"!="
NOT 		"!"
OR 		"||"
PLUS 		"+"
RBRACE 		"}"
RPAR 		")"
RSQ 		"]"
SEMICOLON 	";"
ARROW 		"->"
LSHIFT 		"<<"
RSHIFT 		">>"
XOR 		"^"
DOT  		"."
BOOL 		"boolean"
CLASS 		"class"
DOTLENGTH 	".length"
DOUBLE 		"double"
ELSE 		"else"
IF 		"if"
INT		"int"
PRINT 		"System.out.print"
PARSEINT 	"Integer.parseInt"
PUBLIC 		"public"
RETURN 		"return"
STATIC 		"static"
STRING 		"String"
VOID 		"void"
WHILE 		"while"
ASPAS           "\""
ENTER           \\n
TAB             \\t 
INTLIT          0|([1-9]+[0-9]*|([1-9]+(\_)*)+([0-9]*(\_)*)*[0-9]+)+
ID 		("$"|"_"|[a-zA-Z])+("$"|"_"|[a-zA-Z]|{INTLIT})*
SPACE           " "|"\t"|"\f"
EXPOENTE        (e|E)("+"|"-")?((0*)?(\_)*)*?{INTLIT}
REALLIT         ((((0*)?(\_)*)*{INTLIT}".")|(((0*)?(\_)*)*{INTLIT}"."((0*)?(\_)*)*{INTLIT})|((0*)?(\_)*)*{INTLIT}"."((0*)?(\_)*)*{INTLIT}{EXPOENTE}|((0*)?(\_)*)*{INTLIT}"."{EXPOENTE}|"."((0*)?(\_)*)*{INTLIT}{EXPOENTE}|((0*)?(\_)*)*{INTLIT}{EXPOENTE}|"."((0*)?(\_)*)*{INTLIT})
RESERVED        "++"|"--"|"abstract"|"continue"|"for"|"new"|"switch"|"assert"|"default"|"package"|"synchronized"|"do"|"goto"|"private"|"this"|"break"|"implements"|"protected"|"throw"|"byte"|"import"|"throws"|"case"|"enum"|"instanceof"|"transient"|"catch"|"extends"|"short"|"try"|"char"|"final"|"interface"|"finally"|"long"|"strictfp"|"volatile"|"const"|"float"|"native"|"super"|"System"|"Integer"|"null"

INVALID_ESCAPE    \\.|\\


%%
"/*"	        {BEGIN COMMENTP;comment_c = coluna;coluna += yyleng;comment_l = linha;}
<COMMENTP>(\n)|(\r)|(\r\n)    {linha++;coluna = 1;prevl=eofl;prevc=eofc;eofl=linha;eofc=coluna;}
<COMMENTP>"*/"	{BEGIN 0;coluna += yyleng;}
<COMMENTP>.	{coluna += yyleng;prevl=eofl;prevc=eofc;eofl=linha;eofc=coluna;}
<COMMENTP><<EOF>> {BEGIN 0;seof++;eofl=prevl;eofc=prevc;semicol = 0; coluna++;printf("Line %d, col %d: unterminated comment\n", comment_l, comment_c);}

"//"	{BEGIN COMMENTL;coluna += yyleng;}
<COMMENTL>(\n)|(\r)|(\r\n)	{BEGIN 0; if (semicol == 1){ printf("SEMICOLON\n");}coluna = 1;linha++;semicol = 0;}
<COMMENTL>.	{coluna += yyleng;}

{ASPAS}				{BEGIN STRLIT2;string=yytext;pos_inicial=coluna;coluna+=yyleng;erroS=1;invalidScape=0; }

<STRLIT2>"\""			{BEGIN(0); if((flag==1)&&(erroS==1))printf("STRLIT(%s)\n",string);coluna+=yyleng; if((flagReturn==1)&&(invalidScape==0)){yylval.args->val=strdup(string);return STRLIT;}}

<STRLIT2>(\n)|(\r)|(\r\n)	{BEGIN(0);printf("Line %d, col %d: unterminated string literal\n", linha, pos_inicial);linha++;coluna=1;}

<STRLIT2>\\f|\\n|\\r|\\t|\\\\|\\\"	{coluna+=yyleng;}

<STRLIT2>(\\.|\\)		{printf("Line %d, col %d: invalid escape sequence (%s)\n", linha, coluna,yytext);coluna += yyleng;erroS=0;invalidScape=1;} 

<STRLIT2><<EOF>>			{BEGIN(0);printf("Line %d, col %d: unterminated string literal\n", linha, pos_inicial);}

<STRLIT2>.			{coluna+=yyleng;}


{BOOLLIT}         {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("BOOLLIT(%s)\n",yytext);else if(flagReturn==1){yylval.args->val=strdup(yytext);return BOOLLIT;}}
{AND}             {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("AND\n");else if(flagReturn==1)return AND;}
{ASSIGN}          {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("ASSIGN\n");else if(flagReturn==1)return ASSIGN;}
{STAR}            {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("STAR\n");else if(flagReturn==1)return STAR;}
{COMMA}           {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("COMMA\n");else if(flagReturn==1)return COMMA;}
{DIV}             {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("DIV\n");else if(flagReturn==1)return DIV;}
{EQ}              {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("EQ\n");else if(flagReturn==1)return EQ;}
{GE}              {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("GE\n");else if(flagReturn==1)return GE;}
{GT}              {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("GT\n");else if(flagReturn==1)return GT;}
{LBRACE}          {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("LBRACE\n");else if(flagReturn==1)return LBRACE;}
{LE}              {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("LE\n");else if(flagReturn==1)return LE;}
{LPAR}		  {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("LPAR\n");else if(flagReturn==1)return LPAR;}
{LSQ}             {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("LSQ\n");else if(flagReturn==1)return LSQ;}
{LT}              {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("LT\n");else if(flagReturn==1)return LT;}
{MINUS}           {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("MINUS\n");else if(flagReturn==1)return MINUS;}
{MOD}             {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("MOD\n");else if(flagReturn==1)return MOD;}
{NE}              {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("NE\n");else if(flagReturn==1)return NE;}
{NOT}             {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("NOT\n");else if(flagReturn==1)return NOT;}
{OR}              {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("OR\n");else if(flagReturn==1)return OR;}
{PLUS}            {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("PLUS\n");else if(flagReturn==1)return PLUS;}
{RBRACE}          {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("RBRACE\n");else if(flagReturn==1)return RBRACE;}
{RPAR}            {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("RPAR\n");else if(flagReturn==1)return RPAR;}
{RSQ}             {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("RSQ\n");else if(flagReturn==1)return RSQ;}
{SEMICOLON}       {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("SEMICOLON\n");else if(flagReturn==1)return SEMICOLON;}
{ARROW}           {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("ARROW\n");else if(flagReturn==1)return ARROW;}
{LSHIFT}          {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("LSHIFT\n");else if(flagReturn==1)return LSHIFT;}
{RSHIFT}          {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("RSHIFT\n");else if(flagReturn==1)return RSHIFT;}
{XOR}             {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("XOR\n");else if(flagReturn==1)return XOR;}
{BOOL}            {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("BOOL\n");else if(flagReturn==1)return BOOL;}
{CLASS}           {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("CLASS\n");else if(flagReturn==1)return CLASS;}
{DOTLENGTH}       {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("DOTLENGTH\n");else if(flagReturn==1)return DOTLENGTH;}
{DOUBLE}          {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("DOUBLE\n");else if(flagReturn==1)return DOUBLE;}
{ELSE}            {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("ELSE\n");else if(flagReturn==1)return ELSE;}
{IF}              {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("IF\n");else if(flagReturn==1)return IF;}
{INT}             {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("INT\n");else if(flagReturn==1)return INT;}
{PRINT}           {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("PRINT\n");else if(flagReturn==1)return PRINT;}
{PARSEINT}        {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("PARSEINT\n");else if(flagReturn==1)return PARSEINT;}
{PUBLIC}          {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("PUBLIC\n");else if(flagReturn==1)return PUBLIC;}
{RETURN}          {coluna+=yyleng;if(flag==1)printf("RETURN\n");else if(flagReturn==1)return RETURN;}
{STATIC}          {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("STATIC\n");else if(flagReturn==1)return STATIC;}
{STRING}          {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("STRING\n");else if(flagReturn==1)return STRING;}
{VOID}            {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("VOID\n");else if(flagReturn==1)return VOID;}
{WHILE}           {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("WHILE\n");else if(flagReturn==1)return WHILE;}
{RESERVED}        {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("RESERVED(%s)\n",yytext);else if(flagReturn==1){yylval.args->val=strdup(yytext);return RESERVED;}}
{ID}              {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("ID(%s)\n",yytext);else if(flagReturn==1){yylval.args->val=strdup(yytext);return ID;}}
{INTLIT}          {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("INTLIT(%s)\n",yytext);else if(flagReturn==1){yylval.args->val=strdup(yytext);return INTLIT;}}
{REALLIT}         {init_yylval(linha,coluna,NULL);coluna+=yyleng;if(flag==1)printf("REALLIT(%s)\n",yytext);else if(flagReturn==1){yylval.args->val=strdup(yytext);return REALLIT;}}
{SPACE}           {coluna+=yyleng;}
"\r\n"|"\r"|"\n"  {linha++;coluna=1;}

.		  {printf("Line %d, col %d: illegal character (%s)\n",linha,coluna,yytext);coluna++;}
<<EOF>>           {coluna+=yyleng;return 0;}


%%

int main(int argc, char **argv)
{
        if ( (argc>1) && (strcmp(argv[1],"-l")==0) ){
            	flag=1;
	    	yylex();
        }
	else if ((argc>1) &&(strcmp(argv[1],"-t")==0)){
            	flagReturn=1;
		yyparse();
		if(!erro){
			print_tree(0, tree,false);
		}
    		free_tree(tree);
        }
	else if ((argc>1) &&(strcmp(argv[1],"-e1")==0)){
            	flag=1;
	    	yylex();
        }	
	else if ((argc>1) &&(strcmp(argv[1],"-e2")==0)){
		flagReturn=1;
		yyparse();
	}	
	else if ((argc>1) &&(strcmp(argv[1],"-s")==0)){
		flagReturn = 1;
		yyparse();
		if (!erro){

			sem(tree);
			print_table(global);
			//imprimir  ast anotada
			print_tree(0, tree, true);
		}
		free_tree(tree);
	}else{
		flagReturn = 1;
		yyparse();
		if (!erro){
			sem(tree);
		}
		free_tree(tree);
	}
	
	

    return 0;
}

void init_yylval(int line, int col, char* text){
	yylval.args = (struct _args *) malloc(sizeof(struct _args));
	yylval.args->line = line;
	yylval.args->col = col;
	if (text != NULL){
		yylval.args->val = strdup(text);
	} else {
		yylval.args->val = NULL;
	}
}


int yywrap()
{
    return 1;
}
