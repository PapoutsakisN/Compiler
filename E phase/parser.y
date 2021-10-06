%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "vmLibrary.h"

	int yyerror(char *yaccProvideMessage);
	int yylex(void);
	extern int yylineno;
	extern char* yytext;
	extern char* yyval;
	extern FILE* yyin;
	extern FILE * yyout;
	
	int loopCounter=0;
	int FunDeclared=0;
	bool ArrayDeclaration=false;
	int scope=0;
	
	PatchList *ContinueList=NULL;
	PatchList *BreakList=NULL;
	NestLoops *LoopListStack=NULL;

	NestLoops * pushNestedLoop(){
		NestLoops *tmp=(NestLoops*)malloc(sizeof(NestLoops));
		tmp->ContinueList=ContinueList;
		tmp->BreakList=BreakList;
		if(LoopListStack==NULL){
			tmp->next=NULL;
		}else{
			tmp->next=LoopListStack;
		}
		return tmp;
	}
	
	myStack *loopStack=NULL;

	myStack *pushloop()
	{
		myStack *tmp=(myStack*)malloc(sizeof(myStack));
		tmp->myNumb=loopCounter;
		if(loopStack==NULL){
			tmp->next=NULL;
		}else{
			tmp->next=loopStack;
		}
		return tmp;
	}

	int poploop()
	{	
		myStack *tmp=loopStack;
		int savedOffset;
		if(tmp==NULL) return 0;
		else{
			savedOffset=loopStack->myNumb;
			tmp=tmp->next;
			free(loopStack);
			loopStack=tmp;
			return savedOffset;
		}
	}
	myStack *OffsetStack=NULL;

	myStack *pushOffset(int n)
	{
		myStack *tmp=(myStack*)malloc(sizeof(myStack));
		tmp->myNumb=n;
		if(OffsetStack==NULL){
			tmp->next=NULL;
		}else{
			tmp->next=OffsetStack;
		}
		return tmp;
	}

	int popOffset()
	{	
		myStack *tmp=OffsetStack;
		int savedOffset;
		if(tmp==NULL) return 0;
		else{
			savedOffset=OffsetStack->myNumb;
			tmp=tmp->next;
			free(OffsetStack);
			OffsetStack=tmp;
			return savedOffset;
		}
	}

	void LvalueAction(char *name,int line,char *error)
	{//ta actions poy simbainoyne otan sinantisoyme ena lvalue
		if(isLibFunc(name)) {printf("Error:(line %d)Can not %s %s[Library Function]\n",line,error,name);
					ErrorCounter++;
		}
		else if(ReturnNode(HashTable,name,scope)!=NULL){
			if(ReturnNode(HashTable,name,scope)->type==USERFUNC){
				printf("Error:(line %d)Can not %s %s[User Function]\n",line,error,name);
				ErrorCounter++;
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

%token <strValue> ID 
%token <intValue> INT 
%token <realValue> REAL 
%token <strValue> YYSTRING

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
	struct expr* expression;
	struct ST_node* sym;
	struct indexedexprs *indexexpr; 
	struct M *Mquad;
}
 
%type <expression> lvalue member tableitem  assignexpr expr elist normcall callsuffix methodcall term statement statements ifstmt call primary objectdef block listexpr const returnstmt
%type <strValue> funcname
%type <intValue> funcbody ifprefix elseprefix whilestart whilecond N M 
%type <sym> funcprefix funcdef
%type <indexexpr> indexed indexedelemlist indexedelem
%type <Mquad> forprefix

%%
program: 	statements/* stmt* none one or more */	{fprintf(yyout,"#Line %d: program->statements(Successful match)\n",yylineno);}
		;
statements:	statement statements/* one or more*/	{}
		|/*empty e kanonas*/	{fprintf(yyout,"#Line %d: statements->empty rule\n",yylineno);}		
		;
statement: 	expr SEMICOLON	{fprintf(yyout,"#Line %d: statement->expr;\n",yylineno);
				$$=$1;
				resetTmp();			
				}
		|ifstmt		{fprintf(yyout,"#Line %d: statement->ifstatement\n",yylineno);
				$$=$1;				
				}
		|whilestmt	{fprintf(yyout,"#Line %d: statement->whilestatement\n",yylineno);
				$$=NULL;				
				}
		|forstmt	{fprintf(yyout,"#Line %d: statement->forstatement\n",yylineno);
				$$=NULL;				
				}
		|returnstmt	{fprintf(yyout,"#Line %d: statement->returnstatement\n",yylineno);
				$$=newexpr_constnil();			
				}
		|BREAK SEMICOLON	{fprintf(yyout,"#Line %d: statement->break;\n",yylineno);
							if(loopCounter==0){
								printf("Error:(line %d) break not inside loop %s\n",yylineno,yytext);
								ErrorCounter++;
							}else{
								BreakList=add_node(BreakList,nextquadlabel());
								emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
							}
							}
		|CONTINUE SEMICOLON	{fprintf(yyout,"#Line %d: statement->continue;\n",yylineno);
							if(loopCounter==0){
								printf("Error:(line %d) continue not inside loop %s\n",yylineno,yytext);
								ErrorCounter++;
							}else{
								ContinueList=add_node(ContinueList,nextquadlabel());
								emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
							}
							}
		|block		{fprintf(yyout,"#Line %d: statement->block\n",yylineno);
				$$=$1;
				}
		|funcdef	{fprintf(yyout,"#Line %d: statement->funcdef\n",yylineno);}
		|SEMICOLON	{fprintf(yyout,"#Line %d: statement->;\n",yylineno);
				$$=NULL;
				resetTmp();	
				}
		;
expr:		assignexpr	{fprintf(yyout,"#Line %d: expr->assignexpr\n",yylineno);
				$$=$1;
				$$->type=assignexpr_e;				
				}
		|expr PLUS expr	{fprintf(yyout,"#Line %d: expr->expr+expr\n",yylineno);
				$$=newexpr(arithmexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(add,$$,$1,$3,-1,nextquadlabel());
				}
		|expr MINUS expr{fprintf(yyout,"#Line %d: expr->expr-expr\n",yylineno);
				$$=newexpr(arithmexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(sub,$$,$1,$3,-1,nextquadlabel());	
				}
		|expr MULTI expr{fprintf(yyout,"#Line %d: expr->expr*expr\n",yylineno);
				$$=newexpr(arithmexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(mul,$$,$1,$3,-1,nextquadlabel());
				}
		|expr DIV expr	{fprintf(yyout,"#Line %d: expr->expr/expr\n",yylineno);
				$$=newexpr(arithmexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(divv,$$,$1,$3,-1,nextquadlabel());
				}
		|expr MOD expr	{fprintf(yyout,"#Line %d: expr->exprMODexpr\n",yylineno);
				$$=newexpr(arithmexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(mod,$$,$1,$3,-1,nextquadlabel());
				}
		|expr GREAT expr{fprintf(yyout,"#Line %d: expr->expr>expr\n",yylineno);
				$$=newexpr(boolexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(if_greater,NULL,$1,$3,nextquadlabel()+3,nextquadlabel());
				emit(assign,$$,newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,$$,newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
		|expr GREATEQUAL expr	{fprintf(yyout,"#Line %d: expr->expr>=expr\n",yylineno);
				$$=newexpr(boolexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(if_greatereq,NULL,$1,$3,nextquadlabel()+3,nextquadlabel());
				emit(assign,$$,newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,$$,newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
		|expr LESS expr	{fprintf(yyout,"#Line %d: expr->expr<expr\n",yylineno);
				$$=newexpr(boolexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(if_less,NULL,$1,$3,nextquadlabel()+3,nextquadlabel());
				emit(assign,$$,newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,$$,newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
		|expr LESSEQUAL expr	{fprintf(yyout,"#Line %d: expr->expr<=expr\n",yylineno);
				$$=newexpr(boolexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(if_lesseq,NULL,$1,$3,nextquadlabel()+3,nextquadlabel());
				emit(assign,$$,newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,$$,newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
		|expr EQUAL expr{fprintf(yyout,"#Line %d: xpr->expr==expr\n",yylineno);
				$$=newexpr(boolexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(if_eq,NULL,$1,$3,nextquadlabel()+3,nextquadlabel());
				emit(assign,$$,newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,$$,newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
		|expr NOTEQUAL expr	{fprintf(yyout,"#Line %d: expr->expr!=expr\n",yylineno);
				$$=newexpr(boolexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				emit(if_noteq,NULL,$1,$3,nextquadlabel()+3,nextquadlabel());
				emit(assign,$$,newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,$$,newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
		|expr AND expr	{fprintf(yyout,"#Line %d: expr->exprANDexpr\n",yylineno);
				$$=newexpr(boolexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
				//emit(and,$$,$1,$3,-1,nextquadlabel());//fixed  the and for the Tcode
				emit(if_eq,newexpr_constbool(false),$1,NULL,nextquadlabel()+4,nextquadlabel());
				emit(if_eq,newexpr_constbool(false),$3,NULL,nextquadlabel()+3,nextquadlabel());
				emit(assign,$$,newexpr_constbool(true),NULL,-1,-1);
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,$$,newexpr_constbool(false),NULL,-1,-1);
				}
		|expr OR expr	{fprintf(yyout,"#Line %d: expr->exprORexpr\n",yylineno);
				$$=newexpr(boolexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue=$$->sym->name;
			
				//emit(or,$$,$1,$3,-1,nextquadlabel());//fixed the or for the Tcode
				emit(if_eq,newexpr_constbool(true),$1,NULL,nextquadlabel()+4,nextquadlabel());
				emit(if_eq,newexpr_constbool(true),$3,NULL,nextquadlabel()+3,nextquadlabel());
				emit(assign,$$,newexpr_constbool(false),NULL,-1,-1);
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,$$,newexpr_constbool(true),NULL,-1,-1);
				}
		|term		{fprintf(yyout,"#Line %d: expr->term\n",yylineno);
				$$ = $1;
				}
		;
term:		LPAR expr RPAR	{fprintf(yyout,"#Line %d: term->(expr)\n",yylineno);
				$$=$2;			
				}
		|MINUS expr %prec UMINUS{fprintf(yyout,"#Line %d: term->-expr\n",yylineno);
					check_arithmetic($2,yylineno);
					$$=newexpr(arithmexpr_e);
					$$->sym=InsertTmpName(scope);
					$$->value.strValue = $$->sym->name;
					//emit(uminus,$$,$2,NULL,-1,nextquadlabel());//
					emit(mul,$$,$2,newexpr_constint(-1),-1,nextquadlabel());
					}
		|NOT expr	{fprintf(yyout,"#Line %d: term->!expr\n",yylineno);
				$$=newexpr(boolexpr_e);
				$$->sym=InsertTmpName(scope);
				$$->value.strValue = $$->sym->name;
			//	emit(not,$$,$2,NULL,-1,nextquadlabel());//fixed the Not for the Tcode
				emit(if_eq,NULL,$2,newexpr_constbool(false),nextquadlabel()+3,nextquadlabel());//here changing to arg2---------------------
				emit(assign,$$,newexpr_constbool(false),NULL,-1,-1);
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,$$,newexpr_constbool(true),NULL,-1,-1);			
				}
		|INCR lvalue	{
				//if(yyval.strValue) LvalueAction(yylval.strValue,yylineno,"increment"); dont uncomment its bad for ur health
				check_arithmetic($2,yylineno);
				if($2->type==tableitem_e){
					
					$$=emit_iftableitem($2,scope);
					emit(add,$$,$$,newexpr_constint(1),-1,nextquadlabel());
					emit(tablesetelem,$$,$2,$2->index,-1,nextquadlabel());	
				}else{
					emit(add,$2,$2,newexpr_constint(1),-1,nextquadlabel());
					$$=newexpr(arithmexpr_e);
					$$->sym=InsertTmpName(scope);
					emit(assign,$$,$2,NULL,-1,nextquadlabel());
				}
				 fprintf(yyout,"#Line %d: term->++lvalue\n",yylineno);
				 }
		|lvalue{
		//	if(yyval.strValue) LvalueAction(yylval.strValue,yylineno,"increment"); dont uncomment its bad for ur health
			check_arithmetic($1,yylineno);		
			}INCR	{fprintf(yyout,"#Line %d: term->lvalue++\n",yylineno);
				$$=newexpr(var_e);
				$$->sym=InsertTmpName(scope);				
				if($1->type==tableitem_e){
					expr* tmp=emit_iftableitem($1,scope);
					emit(assign,$$,tmp,NULL,-1,nextquadlabel());
					emit(add,tmp,tmp,newexpr_constint(1),-1,nextquadlabel());
					emit(tablesetelem,tmp,$1,$1->index,-1,nextquadlabel());
				}else{
					emit(assign,$$,$1,NULL,-1,nextquadlabel());
					emit(add,$1,$1,newexpr_constint(1),-1,nextquadlabel());
				}				
				}
		|DECR lvalue	{
				//if(yyval.strValue) LvalueAction(yylval.strValue,yylineno,"decrement"); dont uncomment its bad for ur health
				check_arithmetic($2,yylineno);
				fprintf(yyout,"#Line %d: term->--lvalue\n",yylineno);
				if($2->type==tableitem_e){
					$$=emit_iftableitem($2,scope);
					emit(sub,$$,$$,newexpr_constint(1),-1,nextquadlabel());
					emit(tablesetelem,$$,$2,$2->index,-1,nextquadlabel());	
				}else{
					emit(sub,$2,$2,newexpr_constint(1),-1,nextquadlabel());
					$$=newexpr(arithmexpr_e);
					$$->sym=InsertTmpName(scope);
					emit(assign,$$,$2,NULL,-1,nextquadlabel());
				}
				}
		|lvalue{
		//	if(yyval.strValue) LvalueAction(yylval.strValue,yylineno,"decrement"); dont uncomment its bad for ur health
			check_arithmetic($1,yylineno);} DECR	
			{fprintf(yyout,"#Line %d: term->lvalue--\n",yylineno);
			$$=newexpr(var_e);
			$$->sym=InsertTmpName(scope);				
			if($1->type==tableitem_e){
				expr* tmp=emit_iftableitem($1,scope);
				emit(assign,$$,tmp,NULL,-1,nextquadlabel());
				emit(sub,tmp,tmp,newexpr_constint(1),-1,nextquadlabel());
				emit(tablesetelem,tmp,$1,$1->index,-1,nextquadlabel());
			}else{
				emit(assign,$$,$1,NULL,-1,nextquadlabel());
				emit(sub,$1,$1,newexpr_constint(1),-1,nextquadlabel());
			}
			}
		|primary	{fprintf(yyout,"#Line %d: term->primary\n",yylineno);
				$$=$1;				
				}
		;
assignexpr: 	lvalue{if(!ArrayDeclaration){
		 LvalueAction(yylval.strValue,yylineno,"assign any value to");
		}
		ArrayDeclaration=false;
		} ASSIGN expr	{fprintf(yyout,"#Line %d: assignexpr->lvalue=expr\n",yylineno);
				if($1->type==tableitem_e){
					emit(tablesetelem,$4,$1,$1->index,-1,nextquadlabel());
					$$=emit_iftableitem($1,scope);
					$$->type=assignexpr_e;		
				}else{
					emit(assign,$1,$4,NULL,-1,nextquadlabel());
					$$=newexpr(assignexpr_e);
					$$->sym=InsertTmpName(scope);
					emit(assign,$$,$1,NULL,-1,nextquadlabel());
				}			
				}
		;
primary:	lvalue		{fprintf(yyout,"#Line %d: primary->lvalue\n",yylineno);
				$$=emit_iftableitem($1,scope);
				}
		|call		{fprintf(yyout,"#Line %d: primary->call\n",yylineno);}
		|objectdef	{fprintf(yyout,"#Line %d: primary->objectdef\n",yylineno);
				$$=$1;				
				}
		|LPAR funcdef RPAR	{fprintf(yyout,"#Line %d: primary->(funcdef)\n",yylineno);
					$$=newexpr(programfunc_e);
					$$->sym=$2;					
					}
		|const		{fprintf(yyout,"#Line %d: primary->const\n",yylineno);
				$$=$1;					
				}
		;
lvalue:		ID		{fprintf(yyout,"#Line %d: lvalue->id\n",yylineno);
				ST_node *tmp=ReturnNode(HashTable,yylval.strValue,scope);
				if(tmp!=NULL&&tmp->type!=USERFUNC&&!isLibFunc(yytext)){		
					if(tmp->scope!=0)//ean den briskomaste se global scope
					{	
						if(PendingFunction(HeadNTail,tmp->scope,scope)) {
							printf("Error:(line %d) No access in %s\n",yylineno,yytext);//ean parembalete sinartisi anamesa
							ErrorCounter++;
						}
					}
				}else if(tmp!=NULL&&tmp->type==USERFUNC){}//ean einai userfunction simainei oti anaferetai s aytin
				else if(isLibFunc(yytext)) {}	
				else
				{
					if(scope==0) tmp=InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,GLOBAL,currscopeoffset(),-1,-1,currscopespace());
					else tmp=InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,LOCCAL,currscopeoffset(),-1,-1,currscopespace());
					inccurrscopeoffset();
				}
				$$=lvalue_expr(tmp);
				}
		|LOCAL ID	{fprintf(yyout,"#Line %d: lvalue->local id\n",yylineno);
				ST_node *tmp=SearchNameInScope(HashTable,yytext,scope);
				if(isLibFunc(yytext)&&scope!=0) {printf("Error:(line %d)Cant have the same name with Library Function\n",yylineno);
								ErrorCounter++;			
				}
				else if(tmp){//den to eisagei oyte petaei error gt simainei oti to xei brei kai anaferetai s ayto to simbolo
				}else{
					if(scope==0) tmp=InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,GLOBAL,currscopeoffset(),-1,-1,currscopespace());
					else tmp=InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,LOCCAL,currscopeoffset(),-1,-1,currscopespace());
					inccurrscopeoffset();					
				}
				if(tmp) $$=lvalue_expr(tmp);
				else $$=newexpr_constnil();
				}
		|DCOLON ID	{fprintf(yyout,"#Line %d: lvalue->::id\n",yylineno);
				ST_node *tmp=SearchNameInScope(HashTable,yylval.strValue,0);
				if(tmp==NULL){//den iparxei global metavliti me ayto to onoma
					printf("Error:(line %d)No Global ID named %s\n",yylineno,yytext);
					ErrorCounter++;
					$$=newexpr_constnil();
				}else $$=lvalue_expr(tmp);
				}
		|member		{ $$=$1; fprintf(yyout,"#Line %d: lvalue->member\n",yylineno);}
		;
tableitem:	lvalue DOT ID	{fprintf(yyout,"#Line %d: member->lvalue.id\n",yylineno);
				$$=member_item($1,$3,scope);
		}
		|lvalue LBRACKET expr RBRACKET	{ArrayDeclaration=true;fprintf(yyout,"#Line %d: member->lvalue[expr]\n",yylineno);
						$1=emit_iftableitem($1,scope);
						$$=newexpr(tableitem_e);
						$$->sym=$1->sym;
						$$->index=$3;
		}
		;
member:		tableitem {fprintf(yyout,"#Line %d: member->tableitem\n",yylineno);}
		|call DOT ID	{fprintf(yyout,"#Line %d: member->call.id\n",yylineno);
				$$->value.strValue=$3;
				$1->type=tableitem_e;				
				}
		|call LBRACKET expr RBRACKET	{ArrayDeclaration=true;fprintf(yyout,"#Line %d: member->call[expr]\n",yylineno);}
		;
call:		call LPAR elist RPAR	{fprintf(yyout,"#Line %d: call->call(elist)\n",yylineno);
						$$=make_call($1,$3,scope);
					}
		|lvalue callsuffix	{fprintf(yyout,"#Line %d: call->lvalue callsuffix\n",yylineno);
					if($2->method){
						expr *self=$1;
						$1=emit_iftableitem(member_item(self,$2->value.strValue,scope),scope);
						$2=elist_add_front($2,self);
					}					
					$$=make_call($1,$2,scope);
					}
		|LPAR funcdef RPAR LPAR elist RPAR{	fprintf(yyout,"#Line %d: call->(funcdef)(elist)\n",yylineno);
							expr* func=newexpr(programfunc_e);
							func->sym=$2;
							$$=make_call(func,$5,scope);						
		}
		;
callsuffix:	normcall	{fprintf(yyout,"#Line %d: callsuffix->normcall\n",yylineno);
				$$=$1;				
				}
		|methodcall	{fprintf(yyout,"#Line %d: callsuffix->methodcall\n",yylineno);
				$$=$1;
				}
		;
normcall:	LPAR elist RPAR	{fprintf(yyout,"#Line %d: normcall->(elist)\n",yylineno);
				if($2)$$=$2;
				else $$->next=NULL;
				$$->method=false;				
				}
		;
methodcall:	DDOT ID LPAR elist RPAR	{fprintf(yyout,"#Line %d: methodcall->..id(elist)\n",yylineno);
					if($4) $$=$4;
					$$->method=true;
					$$->value.strValue=$2;
					}
		;
elist:		expr listexpr/*[expr[,expr]*]*/	{fprintf(yyout,"#Line %d: elist->expr listexpr\n",yylineno);
						$$=$1;
						$$->next=$2;				
						}
		|	{fprintf(yyout,"#Line %d: elist->empty rule\n",yylineno);
			$$=newexpr_constnil();		
			}
		;
listexpr:	COMMA expr listexpr/*[expr[,expr listexpr]] ara apo miden 1 i kai parapano list expr*/	{fprintf(yyout,"#Line %d: listexpr->, expr listexpr\n",yylineno);
													if($2){
														$$=$2;
														$$->next=$3;	
													}	}
		|/*empty e kanonas[expr+none]*/{fprintf(yyout,"#Line %d: listexpr->empty rule\n",yylineno);
						$$=newexpr_constnil();			
						}
		;
objectdef:	LBRACKET elist RBRACKET		{fprintf(yyout,"#Line %d: objectdef->[elist]\n",yylineno);
						expr *tmp,*tmp2;

						tmp2=$2;
						tmp=newexpr(newtable_e);
						
						tmp->sym=InsertTmpName(scope);
						emit(tablecreate,tmp,NULL,NULL,-1,nextquadlabel());
						int i=0;
						while(tmp2->next){
							emit(tablesetelem,tmp,tmp2,newexpr_constint(i++),-1,nextquadlabel());
							tmp2=tmp2->next;
						}
						$$=tmp;
						}
		|LBRACKET indexed RBRACKET	{fprintf(yyout,"#Line %d: objectdef->[indexed]\n",yylineno);
						expr *tmp;
	
						indexedexprs* tmp2=$2;
						tmp=newexpr(newtable_e);
						tmp->sym=InsertTmpName(scope);
						emit(tablecreate,tmp,NULL,NULL,-1,nextquadlabel());
						while(tmp2){
							emit(tablesetelem,tmp,tmp2->key,tmp2->value,-1,nextquadlabel());
							tmp2=tmp2->next;
						}
						$$=tmp;
						}			
		;
indexed:	indexedelem indexedelemlist	{fprintf(yyout,"#Line %d: indexed->indexedelem indexedelemlist\n",yylineno);
						$$=$1;
						$$->next=$2;
						}
		;
indexedelemlist:COMMA indexedelem indexedelemlist{fprintf(yyout,"#Line %d: indexedelemlist->,indexedelem indexedelemlist\n",yylineno);
						$$=$2;
						$$->next=$3;
						}
		|/*empty e kanonas*/		{fprintf(yyout,"#Line %d: indexedelemlist->empty rule\n",yylineno);
						$$=NULL;					
						}
		;
indexedelem:	LANGLE expr COLON expr RANGLE	{fprintf(yyout,"#Line %d: indexedelem->{expr:expr}\n",yylineno);
						$$=insertIndexValues($2,$4);
						}
		;
block:		LANGLE{scope++;} RANGLE	{fprintf(yyout,"#Line %d: block->{}\n",yylineno);DisableSymbols(HeadNTail,scope);scope--;}//kane deactivate ta symbols poy einai s ayto to scope
		|LANGLE{scope++;} statement statements RANGLE{DisableSymbols(HeadNTail,scope);scope--;
			fprintf(yyout,"#Line %d: block->{statement statements}\n",yylineno);
			$$=$3;
			}
		;
funcname: 	ID {$$=$1; fprintf(yyout,"#Line %d: funcname->id\n",yylineno);}
		|/*empty e kanonas*/{$$=newtmpfuncname(); fprintf(yyout,"#Line %d: funcname->empty rule\n",yylineno);}
		;
funcprefix:	FUNCTION funcname{FunDeclared++;
			ST_node *tmp=SearchNameInScope(HashTable,yytext,scope);
			if(isLibFunc(yytext)){
				printf("Error:(line %d)Can not use same name with Library Functions\n",yylineno);
				ErrorCounter++;
			}else if(tmp){
				printf("Error:(line %d) Redeclaration of symbol %s\n",yylineno,yytext);
				ErrorCounter++;
			}else{
				tmp=InsertSymbol(HashTable,HeadNTail,$2,scope,yylineno,USERFUNC,0,nextquadlabel(),0,currscopespace());
			}
			$$=tmp;
			emit(funcstart,lvalue_expr($$),NULL,NULL,-1,nextquadlabel());
			OffsetStack=pushOffset(currscopeoffset());//ok
			enterscopespace();
			resetformalargsoffset();
			fprintf(yyout,"#Line %d: funcprefix->function funcname\n",yylineno);
		}
		;
funcargs:	LPAR{scope++;} idlist RPAR{
					enterscopespace();
					resetfunctionlocaloffset();
					scope--;
					fprintf(yyout,"#Line %d: funcargs->(idlist)\n",yylineno);
		};
funcstart:	/*empty rule*/{loopStack=pushloop();loopCounter=0;};
funcend:	/*empty rule*/{loopCounter=poploop();};
funcbody:	funcstart block funcend{
			exitscopespace();
			
			FunDeclared--;
			fprintf(yyout,"#Line %d: funcbody->body\n",yylineno);		
		}
		;
funcdef:	funcprefix funcargs funcbody{
			fprintf(yyout,"#Line %d: funcdef->funcprefix funcargs funcbody\n",yylineno);
			exitscopespace();

			$1->VariablesNO=FunctionLocalOffset;
			restorecurrscopeoffset(popOffset());
			$$=$1;
			emit(funcend,lvalue_expr($1),NULL,NULL,-1,nextquadlabel());
		}
		;
const:		INT	{fprintf(yyout,"#Line %d: const->INT\n",yylineno);
			$$=newexpr_constint($1);
			}
		|REAL	{fprintf(yyout,"#Line %d: const->REAL\n",yylineno);
			$$=newexpr_constreal($1);
			}
		|YYSTRING	{fprintf(yyout,"#Line %d: const->STRING\n",yylineno);
				$$=newexpr_conststring($1);
				}
		|NIL	{fprintf(yyout,"#Line %d: const->NIL\n",yylineno);
			$$=newexpr_constnil();
			}
		|TRUE	{fprintf(yyout,"#Line %d: const->TRUE\n",yylineno);
			$$=newexpr_constbool(true);
			}
		|FALSE	{fprintf(yyout,"#Line %d: const->FALSE\n",yylineno);
			$$=newexpr_constbool(false);
			}
		;
idlist:		ID {	if(FunDeclared) AddArgs(HeadNTail,yytext,scope);
			if(isLibFunc(yytext)){
				printf("Error:(line %d)Formal can not use same name with Library Functions\n",yylineno);
				ErrorCounter++;
			}else if(SearchNameInScope(HashTable,yytext,scope)){
				printf("Error:(line %d)Formal Redeclaration(ID %s)\n",yylineno,yytext);		
				ErrorCounter++;	
			}else {
				InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,FORMAL,currscopeoffset(),-1,-1,currscopespace());
				inccurrscopeoffset();
			}
		}idlistlist	{fprintf(yyout,"#Line %d: idlist->id idlist\n",yylineno);}
		|/*empty e kanonas*/	{fprintf(yyout,"#Line %d: idlist->empty rule\n",yylineno);}
		;
idlistlist:	COMMA ID{
			if(FunDeclared) AddArgs(HeadNTail,yytext,scope);
			if(isLibFunc(yytext)){
				printf("Error:(line %d)Formal can not use same name with Library Functions\n",yylineno);
				ErrorCounter++;
			}else if(SearchNameInScope(HashTable,yytext,scope)){
				printf("Error:(line %d)Formal Redeclaration(ID %s)\n",yylineno,yytext);		
				ErrorCounter++;	
			}else {
				InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,FORMAL,currscopeoffset(),-1,-1,currscopespace());
				inccurrscopeoffset();
			}
		} idlistlist	{fprintf(yyout,"#Line %d: idlistlist->, id idlistlist\n",yylineno);}
		|/*empty e kanonas*/	{fprintf(yyout,"#Line %d: idlistlist->empty rule\n",yylineno);}
		;
ifprefix:	IF LPAR expr RPAR {	fprintf(yyout,"#Line %d: ifprefix->if(expr)\n",yylineno);
					emit(if_eq,$3,newexpr_constbool(true),NULL,nextquadlabel()+2,nextquadlabel());
					$$=nextquadlabel();
					emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
		}
		;
elseprefix:	ELSE{	fprintf(yyout,"#Line %d: elseprefix->else\n",yylineno);
			$$=nextquadlabel();
			emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
		}
		;
ifstmt:		ifprefix statement	{fprintf(yyout,"#Line %d: ifstmt->ifprefix statement\n",yylineno);
					patchlabel($1,nextquadlabel());					
					}
		|ifprefix statement elseprefix statement {	fprintf(yyout,"#Line %d: ifstmt->ifprefix statement elseprefix statement\n",yylineno);
								patchlabel($1,$3+1);	
								patchlabel($3,nextquadlabel());			
		}
		;
whilestart:	WHILE{	$$=nextquadlabel();
			++loopCounter;
			if(loopCounter>1||FunDeclared) {
				LoopListStack=pushNestedLoop();
				BreakList=NULL;
				ContinueList=NULL;
			}	
		}
		;
whilecond:	LPAR expr RPAR{ emit(if_eq,$2,newexpr_constbool(true),NULL,nextquadlabel()+2,nextquadlabel());
				$$=nextquadlabel();
				emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
		}
		;
whilestmt:	 whilestart whilecond statement	{fprintf(yyout,"#Line %d: whilestatement->while(expr)statement\n",yylineno);
						emit(jump,NULL,NULL,NULL,$1,nextquadlabel());//check it here
						patchlabel($2,nextquadlabel());	//ayto patsarei apo pano
						
							while(ContinueList!=NULL){
								patchlabel(ContinueList->quadNo,$1);
								ContinueList=ContinueList->next;
							}
						
							while(BreakList!=NULL){
								patchlabel(BreakList->quadNo,nextquadlabel());
								BreakList=BreakList->next;
							}
											
						if(LoopListStack){
							BreakList=LoopListStack->BreakList;
							ContinueList=LoopListStack->ContinueList;
							NestLoops *tmp=LoopListStack;
							tmp=LoopListStack->next;
							free(LoopListStack);
							LoopListStack=tmp;
						}
						--loopCounter;				
		}
		;
N:		{$$=nextquadlabel();
		emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
		}		
		;
M:		{$$=nextquadlabel();}
		;
forprefix:	FOR{	++loopCounter;
			if(loopCounter>1||FunDeclared){
				LoopListStack=pushNestedLoop();
				BreakList=NULL;
				ContinueList=NULL;
			}		
		} LPAR elist SEMICOLON M expr SEMICOLON{
							$$=(struct M*)malloc(sizeof(struct M));
							$$->test=$6;
							$$->enter=nextquadlabel();
							emit(if_eq,NULL,$7,newexpr_constbool(true),-1,nextquadlabel());
							}
		;
forstmt:	forprefix N elist RPAR N statement N	{fprintf(yyout,"#Line %d: forstatement->for(elist;expr;elist) statement\n",yylineno);
							patchlabel($1->enter,$5+1);
							patchlabel($2,nextquadlabel());
							patchlabel($5,$1->test);
							patchlabel($7,$2+1);
												
							while(ContinueList!=NULL){
								patchlabel(ContinueList->quadNo,$2+1);
								ContinueList=ContinueList->next;
							}
									
							while(BreakList!=NULL){
								patchlabel(BreakList->quadNo,nextquadlabel());
								BreakList=BreakList->next;
							}

							if(LoopListStack){
								BreakList=LoopListStack->BreakList;
								ContinueList=LoopListStack->ContinueList;
								NestLoops *tmp=LoopListStack;
								tmp=LoopListStack->next;
								free(LoopListStack);
								LoopListStack=tmp;
							}
							--loopCounter;
							}
		;
returnstmt:	RETURN SEMICOLON	{fprintf(yyout,"#Line %d: returnstament->return;\n",yylineno);
					if(FunDeclared){
						emit(retur,NULL,NULL,NULL,-1,nextquadlabel());
					}else{
						printf("Error:(line %d)Return used outside of a function\n",yylineno);
						ErrorCounter++;
					}				
					}
		|RETURN expr SEMICOLON	{fprintf(yyout,"#Line %d: returnstament->return expr;\n",yylineno);
					if(FunDeclared){
						emit(retur,NULL,NULL,$2,-1,nextquadlabel());
					}else{
						printf("Error:(line %d)Return used outside of a function\n",yylineno);
						ErrorCounter++;
					}					
					}
		;

%%

int yyerror(char *yaccProvideMessage)
{
	fprintf(stderr,"%s :at line %d \n",yaccProvideMessage,yylineno);
	ErrorCounter++;
}

void initializeStructs()
{//ginontai arxikopoiisi toy hashtale toy pinaka me ta scopelists
//kai ta library functions poy eisagontai ston pinaka
	int i;
	for(i=0;i<hashTSize;i++)
	{
		HashTable[i]=NULL;	
	}	
	for(i=0;i<2;i++)
	{
		HeadNTail[i]=NULL;
	}

	InsertSymbol(HashTable,HeadNTail,"print",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"input",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"objectmemberkeys",0,0,LIBFUNC,0,-1,0,programvar);
	InsertSymbol(HashTable,HeadNTail,"objecttotalmembers",0,0,LIBFUNC,0,-1,0,programvar);
	InsertSymbol(HashTable,HeadNTail,"objectcopy",0,0,LIBFUNC,0,-1,0,programvar);
	InsertSymbol(HashTable,HeadNTail,"totalarguments",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"argument",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"typeof",0,0,LIBFUNC,0,-1,0,programvar);
	InsertSymbol(HashTable,HeadNTail,"strtonum",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"sqrt",0,0,LIBFUNC,0,-1,0,programvar);
	InsertSymbol(HashTable,HeadNTail,"cos",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"sin",0,0,LIBFUNC,0,-1,0,programvar);//ok
	
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
	yyparse();//2 fash
	generateT();
	patch_incomplete_jumps();
	//Print_HashTable(HashTable);
	//ScopeNode(HeadNTail);
	
	if(ErrorCounter) printf("ERROR NO: %d.Correct them to produce quads!\n",ErrorCounter);
	else{
		//printQuads();//3h fash
		//printTcode();//4h fash
		//printTables();
		produceFile();
		run_virtual_machine();
	}
	return 0;
}
