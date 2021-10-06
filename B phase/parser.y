%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "SymbolTable.h"

	int yyerror(char *yaccProvideMessage);
	int yylex(void);
	extern int yylineno;
	extern char* yytext;
	extern char* yyval;
	extern FILE* yyin;
	extern FILE * yyout;

	ST_node *HashTable[hashTSize];//dilosi to hashtable
	ST_node *ScopeLists[ScopeNO];//dilosi toy pinaka poy krata tis scopelists

	int scope=0,FunDeclared=0;
	bool ArrayDeclaration=false;

	char FunctionName[]="function",tmp[100];//2 pinakes poy voithane stin ektiposi enos dummy onomatos gia tis function
	
	void LvalueAction(char *name,int line,char *error)
	{//ta actions poy simbainoyne otan sinantisoyme ena lvalue
		if(isLibFunc(name)) printf("Error:(line %d)Can not %s %s[Library Function]\n",line,error,name);
		else if(ReturnNode(HashTable,name,scope)!=NULL){
			if(ReturnNode(HashTable,name,scope)->type==USERFUNC){
			printf("Error:(line %d)Can not %s %s[User Function]\n",line,error,name);
			}
		}
	}
%}
/*Afetiria Analisis*/
%start program

/*Termatika symbola*/
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL

%token ASSIGN PLUS MINUS MULTI DIV MOD EQUAL NOTEQUAL INCR DECR LESS GREAT GREATEQUAL LESSEQUAL

%token LPAR RPAR LBRACKET RBRACKET LANGLE RANGLE SEMICOLON COMMA COLON DCOLON DOT DDOT

%token ID INT REAL YYSTRING

/*Proteraiotita kai prosetairistikotita*/
%right ASSIGN
%left OR
%left AND
%nonassoc EQUAL NOTEQUAL
%nonassoc GREAT GREATEQUAL LESS LESSEQUAL
%left PLUS MINUS
%left MULTI DIV MOD
%right NOT INCR DECR UMINUS
%left DOT DDOT
%left LBRACKET RBRACKET
%left LPAR RPAR

%nonassoc IF
%nonassoc ELSE

%union{
	char* strValue;
	int intValue;
	double realValue;
}

%%
program: 	statements/* stmt* none one or more */	{fprintf(yyout,"#Line %d: program->statements(Successful match)\n",yylineno);}
		;
statements:	statement statements/* one or more*/	{}
		|/*empty e kanonas*/	{fprintf(yyout,"#Line %d: statements->empty rule\n",yylineno);}		
		;
statement: 	expr SEMICOLON	{fprintf(yyout,"#Line %d: statement->expr;\n",yylineno);}
		|ifstmt		{fprintf(yyout,"#Line %d: statement->ifstatement\n",yylineno);}
		|whilestmt	{fprintf(yyout,"#Line %d: statement->whilestatement\n",yylineno);}
		|forstmt	{fprintf(yyout,"#Line %d: statement->forstatement\n",yylineno);}
		|returnstmt	{fprintf(yyout,"#Line %d: statement->returnstatement\n",yylineno);}
		|BREAK SEMICOLON	{fprintf(yyout,"#Line %d: statement->break;\n",yylineno);}
		|CONTINUE SEMICOLON	{fprintf(yyout,"#Line %d: statement->continue;\n",yylineno);}	
		|block		{fprintf(yyout,"#Line %d: statement->block\n",yylineno);}
		|funcdef	{fprintf(yyout,"#Line %d: statement->funcdef\n",yylineno);}
		|SEMICOLON	{fprintf(yyout,"#Line %d: statement->;\n",yylineno);}
		;
expr:		assignexpr	{fprintf(yyout,"#Line %d: expr->assignexpr\n",yylineno);}
		|expr PLUS expr	{fprintf(yyout,"#Line %d: expr->expr+expr\n",yylineno);}
		|expr MINUS expr{fprintf(yyout,"#Line %d: expr->expr-expr\n",yylineno);}
		|expr MULTI expr{fprintf(yyout,"#Line %d: expr->expr*expr\n",yylineno);}
		|expr DIV expr	{fprintf(yyout,"#Line %d: expr->expr/expr\n",yylineno);}
		|expr MOD expr	{fprintf(yyout,"#Line %d: expr->exprMODexpr\n",yylineno);}
		|expr GREAT expr{fprintf(yyout,"#Line %d: expr->expr>expr\n",yylineno);}
		|expr GREATEQUAL expr	{fprintf(yyout,"#Line %d: expr->expr>=expr\n",yylineno);}
		|expr LESS expr	{fprintf(yyout,"#Line %d: expr->expr<expr\n",yylineno);}
		|expr LESSEQUAL expr	{fprintf(yyout,"#Line %d: expr->expr<=expr\n",yylineno);}
		|expr EQUAL expr{fprintf(yyout,"#Line %d: xpr->expr==expr\n",yylineno);}
		|expr NOTEQUAL expr	{fprintf(yyout,"#Line %d: expr->expr!=expr\n",yylineno);}
		|expr AND expr	{fprintf(yyout,"#Line %d: expr->exprANDexpr\n",yylineno);}
		|expr OR expr	{fprintf(yyout,"#Line %d: expr->exprORexpr\n",yylineno);}
		|term		{fprintf(yyout,"#Line %d: expr->term\n",yylineno);}
		;
term:		LPAR expr RPAR	{fprintf(yyout,"#Line %d: term->(expr)\n",yylineno);}
		|MINUS expr %prec UMINUS{fprintf(yyout,"#Line %d: term->-expr\n",yylineno);}
		|NOT expr	{fprintf(yyout,"#Line %d: term->!expr\n",yylineno);}
		|INCR lvalue	{
				LvalueAction(yylval.strValue,yylineno,"increment");
				 fprintf(yyout,"#Line %d: term->++lvalue\n",yylineno);
				 }
		|lvalue{LvalueAction(yylval.strValue,yylineno,"increment");}INCR	{fprintf(yyout,"#Line %d: term->lvalue++\n",yylineno);}
		|DECR lvalue	{LvalueAction(yylval.strValue,yylineno,"decrement");
				 fprintf(yyout,"#Line %d: term->--lvalue\n",yylineno);}
		|lvalue{LvalueAction(yylval.strValue,yylineno,"decrement");} DECR	{fprintf(yyout,"#Line %d: term->lvalue--\n",yylineno);}
		|primary	{fprintf(yyout,"#Line %d: term->primary\n",yylineno);}
		;
assignexpr: 	lvalue{if(!ArrayDeclaration){
			LvalueAction(yylval.strValue,yylineno,"assign any value to");
		}
		ArrayDeclaration=false;
		} ASSIGN expr	{fprintf(yyout,"#Line %d: assignexpr->lvalue=expr\n",yylineno);}
		;
primary:	lvalue		{fprintf(yyout,"#Line %d: primary->lvalue\n",yylineno);}
		|call		{fprintf(yyout,"#Line %d: primary->call\n",yylineno);}
		|objectdef	{fprintf(yyout,"#Line %d: primary->objectdef\n",yylineno);}
		|LPAR funcdef RPAR	{fprintf(yyout,"#Line %d: primary->(funcdef)\n",yylineno);}
		|const		{fprintf(yyout,"#Line %d: primary->const\n",yylineno);}
		;
lvalue:		ID		{fprintf(yyout,"#Line %d: lvalue->id\n",yylineno);
				ST_node *tmp=ReturnNode(HashTable,yylval.strValue,scope);
				if(tmp!=NULL&&tmp->type!=USERFUNC&&!isLibFunc(yytext)){			
					if(tmp->scope!=0)//ean den briskomaste se global scope
					{	
						if(PendingFunction(ScopeLists,tmp->scope,scope)) printf("Error:(line %d) No access in %s\n",yylineno,yytext);//ean parembalete sinartisi anamesa
					}
					
				}else if(tmp!=NULL&&tmp->type==USERFUNC){}//ean einai userfunction simainei oti anaferetai s aytin
				else if(isLibFunc(yytext)) {}	
				else
				{
					if(scope==0) InsertSymbol(HashTable,ScopeLists,yytext,scope,yylineno,GLOBAL);
					else InsertSymbol(HashTable,ScopeLists,yytext,scope,yylineno,LOCCAL);
				}
				}
		|LOCAL ID	{fprintf(yyout,"#Line %d: lvalue->local id\n",yylineno);
				if(isLibFunc(yytext)&&scope!=0) printf("Error:(line %d)Cant have the same name with Library Function\n",yylineno);
				else if(SearchNameInScope(HashTable,yytext,scope)){//den to eisagei oyte petaei error gt simainei oti to xei brei kai anaferetai s ayto to simbolo
				}else{
					if(scope==0) InsertSymbol(HashTable,ScopeLists,yytext,scope,yylineno,GLOBAL);
					else InsertSymbol(HashTable,ScopeLists,yytext,scope,yylineno,LOCCAL);
					
				}
				}
		|DCOLON ID	{fprintf(yyout,"#Line %d: lvalue->::id\n",yylineno);
				
				if(!SearchNameInScope(HashTable,yylval.strValue,0)){//den iparxei global metavliti me ayto to onoma
					printf("Error:(line %d)No Global ID named %s\n",yylineno,yytext);
				}
				}
		|member		{fprintf(yyout,"#Line %d: lvalue->member\n",yylineno);}
		;
member:		lvalue DOT ID	{fprintf(yyout,"#Line %d: member->lvalue.id\n",yylineno);}
		|lvalue LBRACKET expr RBRACKET	{ArrayDeclaration=true;fprintf(yyout,"#Line %d: member->lvalue[expr]\n",yylineno);}
		|call DOT ID	{fprintf(yyout,"#Line %d: member->call.id\n",yylineno);}
		|call LBRACKET expr RBRACKET	{ArrayDeclaration=true;fprintf(yyout,"#Line %d: member->call[expr]\n",yylineno);}
		;
call:		call LPAR elist RPAR	{fprintf(yyout,"#Line %d: call->call(elist)\n",yylineno);}
		|lvalue callsuffix	{fprintf(yyout,"#Line %d: call->lvalue callsuffix\n",yylineno);}
		|LPAR funcdef RPAR LPAR elist RPAR{fprintf(yyout,"#Line %d: call->(funcdef)(elist)\n",yylineno);}
		;
callsuffix:	normcall	{fprintf(yyout,"#Line %d: callsuffix->normcall\n",yylineno);}
		|methodcall	{fprintf(yyout,"#Line %d: callsuffix->methodcall\n",yylineno);}
		;
normcall:	LPAR elist RPAR	{fprintf(yyout,"#Line %d: normcall->(elist)\n",yylineno);}
		;
methodcall:	DDOT ID LPAR elist RPAR	{fprintf(yyout,"#Line %d: methodcall->..id(elist)\n",yylineno);}
		;
elist:		expr listexpr/*[expr[,expr]*]*/	{fprintf(yyout,"#Line %d: elist->expr listexpr\n",yylineno);}
		|		
		;
listexpr:	COMMA expr listexpr/*[expr[,expr listexpr]] ara apo miden 1 i kai parapano list expr*/	{fprintf(yyout,"#Line %d: listexpr->, expr listexpr\n",yylineno);}
		|/*empty e kanonas[expr+none]*/{fprintf(yyout,"#Line %d: listexpr->empty rule\n",yylineno);}
		;
objectdef:	LBRACKET elist RBRACKET		{fprintf(yyout,"#Line %d: objectdef->[elist]\n",yylineno);}
		|LBRACKET indexed RBRACKET	{fprintf(yyout,"#Line %d: objectdef->[indexed]\n",yylineno);}			
		;
indexed:	indexedelem indexedelemlist	{fprintf(yyout,"#Line %d: indexed->indexedelem indexedelemlist\n",yylineno);}
		;
indexedelemlist:COMMA indexedelem indexedelemlist{fprintf(yyout,"#Line %d: indexedelemlist->,indexedelem indexedelemlist\n",yylineno);}
		|/*empty e kanonas*/		{fprintf(yyout,"#Line %d: indexedelemlist->empty rule\n",yylineno);}
		;
indexedelem:	LANGLE expr COLON expr RANGLE	{fprintf(yyout,"#Line %d: indexedelem->{expr:expr}\n",yylineno);}
		;
block:		LANGLE{scope++;} RANGLE	{fprintf(yyout,"#Line %d: block->{}\n",yylineno);DisableSymbols(ScopeLists,scope);scope--;}//kane deactivate ta symbols poy einai s ayto to scope
		|LANGLE{scope++;} statement statements RANGLE{DisableSymbols(ScopeLists,scope);scope--;
			fprintf(yyout,"#Line %d: block->{statement statements}\n",yylineno);FunDeclared--;}
		;

funcdef:	FUNCTION{FunDeclared++;
			 sprintf(tmp, "%d", yylineno);//give a dummy name to function
			 strcat(FunctionName, tmp);//function +line of declaration
			 InsertSymbol(HashTable,ScopeLists,FunctionName,scope,yylineno,USERFUNC);
			 strcpy(FunctionName,"function");//arxikopoiisi pali
				} LPAR{scope++;} idlist RPAR block{scope++;}	{fprintf(yyout,"#Line %d: funcdef->function(idlist) block\n",yylineno);
		}
		|FUNCTION ID{	FunDeclared++;
				if(isLibFunc(yytext)){
				printf("Error:(line %d)Can not use same name with Library Functions\n",yylineno);
		}else if(SearchNameInScope(HashTable,yytext,scope))
		{
			printf("Error:(line %d) Redeclaration of symbol %s\n",yylineno,yytext);
		}else InsertSymbol(HashTable,ScopeLists,yytext,scope,yylineno,USERFUNC);
		} LPAR{scope++;} idlist RPAR{scope--;} block	{fprintf(yyout,"#Line %d: funcdef->function id (idlist) block\n",yylineno);}
		;
const:		INT	{fprintf(yyout,"#Line %d: const->INT\n",yylineno);}
		|REAL	{fprintf(yyout,"#Line %d: const->REAL\n",yylineno);}
		|YYSTRING	{fprintf(yyout,"#Line %d: const->STRING\n",yylineno);}
		|NIL	{fprintf(yyout,"#Line %d: const->NIL\n",yylineno);}
		|TRUE	{fprintf(yyout,"#Line %d: const->TRUE\n",yylineno);}
		|FALSE	{fprintf(yyout,"#Line %d: const->FALSE\n",yylineno);}
		;
idlist:		ID {	if(FunDeclared) AddArgs(ScopeLists,yytext,scope);
			if(isLibFunc(yytext)){
				printf("Error:(line %d)Formal can not use same name with Library Functions\n",yylineno);
			}else if(SearchNameInScope(HashTable,yytext,scope)){
				printf("Error:(line %d)Formal Redeclaration(ID %s)\n",yylineno,yytext);			
			}else InsertSymbol(HashTable,ScopeLists,yytext,scope,yylineno,FORMAL);
		}idlistlist	{fprintf(yyout,"#Line %d: idlist->id idlist\n",yylineno);}
		|/*empty e kanonas*/	{fprintf(yyout,"#Line %d: idlist->empty rule\n",yylineno);}
		;
idlistlist:	COMMA ID{
			if(FunDeclared) AddArgs(ScopeLists,yytext,scope);
			if(isLibFunc(yytext)){
				printf("Error:(line %d)Formal can not use same name with Library Functions\n",yylineno);
			}else if(SearchNameInScope(HashTable,yytext,scope)){
				printf("Error:(line %d)Formal Redeclaration(ID %s)\n",yylineno,yytext);			
			}else InsertSymbol(HashTable,ScopeLists,yytext,scope,yylineno,FORMAL);
		} idlistlist	{fprintf(yyout,"#Line %d: idlistlist->, id idlistlist\n",yylineno);}
		|/*empty e kanonas*/	{fprintf(yyout,"#Line %d: idlistlist->empty rule\n",yylineno);}
		;
ifstmt:		IF LPAR expr RPAR statement	{fprintf(yyout,"#Line %d: ifstatement->if(expr) statement\n",yylineno);}
		|IF LPAR expr RPAR statement ELSE statement	{fprintf(yyout,"#Line %d: ifstatement->if(expr) statement else statement\n",yylineno);}
		;
whilestmt:	WHILE LPAR expr RPAR statement	{fprintf(yyout,"#Line %d: whilestatement->while(expr)statement\n",yylineno);}
		;
forstmt:	FOR LPAR elist SEMICOLON expr SEMICOLON elist RPAR statement	{fprintf(yyout,"#Line %d: forstatement->for(elist;expr;elist) statement\n",yylineno);}
		;
returnstmt:	RETURN SEMICOLON	{fprintf(yyout,"#Line %d: returnstament->return;\n",yylineno);}
		|RETURN expr SEMICOLON	{fprintf(yyout,"#Line %d: returnstament->return expr;\n",yylineno);}
		;

%%
int yyerror(char *yaccProvideMessage)
{
	fprintf(stderr,"%s :at line %d \n",yaccProvideMessage,yylineno);
}

void initializeStructs()
{//ginontai arxikopoiisi toy hashtale toy pinaka me ta scopelists
//kai ta library functions poy eisagontai ston pinaka
	int i;
	for(i=0;i<hashTSize;i++)
	{
		HashTable[i]=NULL;	
	}	
	for(i=0;i<20;i++)
	{
		ScopeLists[i]=NULL;
	}
	
	InsertSymbol(HashTable,ScopeLists,"print",0,0,LIBFUNC);//ok
	InsertSymbol(HashTable,ScopeLists,"input",0,0,LIBFUNC);//ok
	InsertSymbol(HashTable,ScopeLists,"objectmemberkeys",0,0,LIBFUNC);
	InsertSymbol(HashTable,ScopeLists,"objecttotalmembers",0,0,LIBFUNC);
	InsertSymbol(HashTable,ScopeLists,"objectcopy",0,0,LIBFUNC);
	InsertSymbol(HashTable,ScopeLists,"totalarguments",0,0,LIBFUNC);//ok
	InsertSymbol(HashTable,ScopeLists,"argument",0,0,LIBFUNC);//ok
	InsertSymbol(HashTable,ScopeLists,"typeof",0,0,LIBFUNC);
	InsertSymbol(HashTable,ScopeLists,"strtonum",0,0,LIBFUNC);//ok
	InsertSymbol(HashTable,ScopeLists,"sqrt",0,0,LIBFUNC);
	InsertSymbol(HashTable,ScopeLists,"cos",0,0,LIBFUNC);//ok
	InsertSymbol(HashTable,ScopeLists,"sin",0,0,LIBFUNC);//ok
}

int main(int argc,char **argv)
{
	if(argc>1){
		if(!(yyin=fopen(argv[1],"r"))){
			fprintf(stderr,"Cannot read file:%s\n",argv[1]);
			return 1;
		}
	}else yyin=stdin;
	
	initializeStructs();
	yyparse();

	Print_HashTable(HashTable);
	//PrintSymbolsV2(ScopeLists);
	return 0;
}



