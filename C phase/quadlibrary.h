#include "SymbolTable.h"
#define EXPAND_SIZE 1024
#define CURR_SIZE (total*sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE*sizeof(quad)+CURR_SIZE)

int ErrorCounter=0;
unsigned int total=0;
unsigned int currQuad=0;
int tmpVarCounter=-1;
char tmpVar[1000]={""};
char tmpName[]="@tmp";
char result[]={""};
char FunctionName[]="_function";//pinakes poy voithane stin ektiposi enos dummy onomatos gia tis function
char tmpC[1000]={""};
char Fresult[]={""};
int dummycounter=-1;

unsigned int ProgramVarOffset=0;
unsigned int FunctionLocalOffset=0;
unsigned int FormalArgOffset=0;
unsigned int scopeSpaceCounter=1;
ST_node *HashTable[hashTSize];//dilosi to hashtable 
ScopeLists *HeadNTail[2];//dilosi toy pinaka poy krata tis scopelists
//scopespace functions here
char *newtmpfuncname(){
	dummycounter++;
	sprintf(tmpC, "%d", dummycounter);
	strcpy(Fresult,FunctionName);
	strcat(Fresult, tmpC);
	return Fresult;
}

scopespace_t currscopespace(void){
	if(scopeSpaceCounter==1) return programvar;
	else{
		if(scopeSpaceCounter%2==0) return formalarg;
		else return fuctionlocal;	
	}
}

int currscopeoffset(){
	switch(currscopespace()){
		case programvar: return ProgramVarOffset;
		case fuctionlocal: return FunctionLocalOffset;
		case formalarg: return FormalArgOffset;
		default: assert(0);
	}
}

void inccurrscopeoffset(){
	switch(currscopespace()){
		case programvar:  ProgramVarOffset++; break;
		case fuctionlocal:  FunctionLocalOffset++; break;
		case formalarg:  FormalArgOffset++; break;
		default: assert(0);
	}
}

void enterscopespace(void){
	++scopeSpaceCounter;
}

void exitscopespace(void){
	assert(scopeSpaceCounter>1);
	--scopeSpaceCounter;
}

void resetformalargsoffset(void){
	FormalArgOffset=0;
}

void resetfunctionlocaloffset(){
	FunctionLocalOffset=0;
}

void restorecurrscopeoffset(int numb){
	switch(currscopespace()){
		case programvar: ProgramVarOffset=numb; break;
		case fuctionlocal: FunctionLocalOffset=numb; break;
		case formalarg: FormalArgOffset=numb; break;
		default: assert(0);
	}
}

typedef enum expr_t{
	var_e,tableitem_e,programfunc_e,libraryfunc_e,arithmexpr_e,boolexpr_e,
	assignexpr_e,newtable_e,constint_e,constdbl_e,constbool_e,conststr_e,nil_e
}expr_t;

typedef  enum iopcode{	
	assign,add,sub,mul,divv,mod,uminus,and,or,not,if_eq,if_noteq,if_lesseq,
	if_greatereq,if_less,if_greater,jump,call,param,retur,getretval,funcstart,
	funcend,tablecreate,tablegetelem,tablesetelem
}iopcode;

typedef struct expr{
	expr_t type;
	ST_node * sym;
	bool method;
	union{
		int intValue;
		double realValue;
		char* strValue;
		bool boolValue;
	}value;
	struct expr* index;
	struct expr* next;
}expr;

struct M{
	int enter;
	int test;
};

typedef struct quad{
	iopcode op;
	expr* result;
	expr* arg1;
	expr* arg2;
	unsigned int label;
	unsigned int line;
}quad;

typedef struct indexedexprs{

	expr* key;
	expr* value;
	struct indexedexprs* next;
}indexedexprs;

quad* quadArraylist=NULL;

void resetTmp(){
	tmpVarCounter=-1;
}

char *newTmpName(){
	tmpVarCounter++;
	sprintf(tmpVar, "%d", tmpVarCounter);
	strcpy(result,tmpName);
	strcat(result, tmpVar);
	return result;
}
//insert tmp name to symboltable
ST_node *InsertTmpName(int scope)//HEeeeeeeeeeeere
{
	char* new_name=newTmpName();
	ST_node *tmp=InsertSymbol(HashTable,HeadNTail,new_name,scope,-1,LOCCAL,currscopeoffset(),-1,-1,currscopespace());		
	inccurrscopeoffset();
	return tmp;
	
}

void expand(){
	quad *newArray=(quad*)malloc(NEW_SIZE);
	if(quadArraylist){
		memcpy(newArray,quadArraylist,CURR_SIZE);
		free(quadArraylist);
	}
	quadArraylist=newArray;
	total+=EXPAND_SIZE;
}

void emit(iopcode op,expr* result,expr* arg1,expr* arg2,unsigned int label,unsigned int line)
{
	if(currQuad==total) expand();
	quad *New_node=quadArraylist+currQuad++;
	New_node->op=op;
	New_node->result=result;
	New_node->arg1=arg1;
	New_node->arg2=arg2;
	New_node->label=label;
	New_node->line=line;
}

int nextquadlabel(void){
	return currQuad;
}

void patchlabel(int quadNO,int label){
	assert(quadNO<currQuad);
	quadArraylist[quadNO].label=label;
}

expr *elist_add_front(expr* head,expr* Addnode)
{
	expr *tmp=head;
	if(tmp){
		tmp=Addnode;
	}else{
		tmp=Addnode;
		tmp->next=NULL;
	}
	Addnode->next=head;
	return tmp;
}

expr *newexpr(expr_t t){
	expr* new_expr=(expr*)malloc(sizeof(expr));
	new_expr->type=t;
	new_expr->sym=NULL;
	new_expr->index=NULL;
	new_expr->next=NULL;
	new_expr->method=false;
	return new_expr;
}

expr *lvalue_expr(ST_node* sym){
	assert(sym);
	expr *new_expr=(expr*)malloc(sizeof(expr));
	if(sym->type==LIBFUNC) new_expr->type=libraryfunc_e;
	else if(sym->type==USERFUNC) new_expr->type=programfunc_e;
	else new_expr->type=var_e;
	new_expr->sym=sym;
	new_expr->value.strValue=strdup(sym->name);
	new_expr->index=NULL;
	new_expr->next=NULL;
	new_expr->method=false;
	return new_expr;
}

expr *newexpr_conststring(char *s){
	expr *new_expr=newexpr(conststr_e);
	new_expr->value.strValue=strdup(s);
	return new_expr;
}

expr *newexpr_constint(int n){
	expr *new_expr=newexpr(constint_e);
	new_expr->value.intValue=n;
	return new_expr;
}

expr *newexpr_constreal(double n){
	expr *new_expr=newexpr(constdbl_e);
	new_expr->value.realValue=n;
	return new_expr;
}

expr *newexpr_constbool(bool n){
	expr *new_expr=newexpr(constbool_e);
	new_expr->value.boolValue=n;
	return new_expr;
}

expr *newexpr_constnil(){
	expr *new_expr=newexpr(nil_e);
	return new_expr;
}

expr* emit_iftableitem(expr* e,int scope){
	if(e->type!=tableitem_e) return e;
	else{
		expr *result=newexpr(var_e);
		result->sym=InsertTmpName(scope);
		emit(tablegetelem,result,e,e->index,-1,-1);
		return result;
	}
}

expr* member_item(expr *lvalue,char *name,int scope){
	lvalue=emit_iftableitem(lvalue,scope);
	expr *new_expr=newexpr(tableitem_e);
	new_expr->sym=lvalue->sym;
	new_expr->index=newexpr_conststring(name);
	new_expr->value.strValue=lvalue->sym->name;
	return new_expr;
}

void reverseElist(expr** head_ref)
{
    expr* prev   = NULL;
    expr* current = *head_ref;
    expr* next;
    while (current != NULL)
    {
        next  = current->next;  
        current->next = prev;   
        prev = current;
        current = next;
    }
    *head_ref = prev;
}

expr* make_call(expr* lvalue,expr* elist,int scope){
	expr *func=emit_iftableitem(lvalue,scope);
	expr* tmpr=elist;

	reverseElist(&tmpr);
	while(tmpr){
	 	if(tmpr->type!=nil_e) emit(param,tmpr,NULL,NULL,-1,-1);
		tmpr=tmpr->next;
	}
	reverseElist(&tmpr);
	emit(call,func,NULL,NULL,-1,-1);
	expr *tmp=newexpr(var_e);
	tmp->sym=InsertTmpName(scope);
	emit(getretval,tmp,NULL,NULL,-1,-1);
	return tmp;
}
typedef struct myStack{
	int myNumb;
	struct myStack *next;
}myStack;

void check_arithmetic(expr* e,int line)
{
	if(e->type==constbool_e||
		e->type==conststr_e||
		e->type==nil_e||
		e->type==newtable_e||
		e->type==programfunc_e||
		e->type==libraryfunc_e||
		e->type==boolexpr_e){
	printf("Error:(line %d) Illegal(No arithmetic expression)\n",line);
	ErrorCounter++;
	}
}


indexedexprs* insertIndexValues(expr *key,expr *value)
{
	indexedexprs* new_index=(indexedexprs*)malloc(sizeof(indexedexprs));
	new_index->key=key;
	new_index->value=value;
	return new_index;
}

char *op2String(iopcode op)
{
	if(op==assign) return "Assign";
	else if(op==add) return "Add";
	else if(op==sub) return "Sub";
	else if(op==mul) return "Mul";
	else if(op==divv) return "Div";
	else if(op==mod) return "Mod";
	else if(op==uminus) return "Uminus";
	else if(op==and) return "And";
	else if(op==or) return "Or";
	else if(op==not) return "Not";
	else if(op==if_eq) return "If_eq";
	else if(op==if_noteq) return "If_noteq";
	else if(op==if_lesseq) return "If_lesseq";
	else if(op==if_greatereq) return "If_greatereq";
	else if(op==if_less) return "If_less";
	else if(op==if_greater) return "If_greater";
	else if(op==jump) return "Jump";
	else if(op==call) return "Call";
	else if(op==param) return "Param";
	else if(op==retur) return "Return";
	else if(op==getretval) return "Getretval";
	else if(op==funcstart) return "FunctionStart";
	else if(op==funcend) return "FunctionEnd";
	else if(op==tablecreate) return "TableCreate";
	else if(op==tablegetelem) return "TableGetElem";
	else if(op==tablesetelem) return "TableSetElem";
}

void QuadArqPrint(expr *expression){
	
	if(expression->type==constint_e){
		printf("%d \t",expression->value.intValue);
	}else if(expression->type==constdbl_e){
		printf("%f \t",expression->value.realValue);
	}else if(expression->type==conststr_e){
		printf("%s \t",expression->value.strValue);
	}else if(expression->type==constbool_e){
		if(expression->value.boolValue) printf("true \t");
		else printf("false \t");
	}else{
	
		if(!expression->sym) printf("%d \t",expression->value.intValue);
		else	printf("%s \t",expression->sym->name);
	}
}

void printQuads(){
	int i=0;
	printf("	######QUADS###### \n");
	quad *tmp=quadArraylist;
	printf("Command Format|QuadNO: OPCODE ARG1 ARG2 RESULT\n");
	while(i<currQuad){
		printf("%d: ",i);

		tmp=quadArraylist+i++;
		printf("%s \t",op2String(tmp->op));
		if(tmp->arg1){
			QuadArqPrint(tmp->arg1);
		}
		if(tmp->arg2){
			QuadArqPrint(tmp->arg2);
		}
		if(tmp->result){
			QuadArqPrint(tmp->result);
		}
		if(tmp->label!=-1) printf("%d \t",tmp->label);
		printf("\n");
	}
}
