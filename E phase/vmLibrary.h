#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "Tcode.h"

#define AVM_TABLE_HASHSIZE 211
#define AVM_WIPEOUT(m) memset(&(m),0,sizeof(m))
#define AVM_ENDING_PC codeSize
#define AVM_STACKSIZE 4096
#define AVM_STACKENV_SIZE 4
#define AVM_MAX_INSTRUCTIONS 25
#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_div execute_arithmetic
#define execute_mod execute_arithmetic
#define AVM_SAVEDTOPSP_OFFSET 1
#define AVM_SAVEDTOP_OFFSET 2
#define AVM_SAVEDPC_OFFSET 3
#define AVM_NUMACTUALS_OFFSET 4

unsigned int flag_enter=0,library_flag=0;
unsigned int executionFinished=0;
unsigned int pc=0;
unsigned int topsp,top;
unsigned int currLine=0;
unsigned int codeSize=0;
unsigned int totalActuals=0; 
instruction* code=NULL;

typedef enum avm_memcell_t{
	int_m=0,
	string_m=1,
	bool_m=2,
	table_m=3,
	userfunc_m=4,
	libfunc_m=5,
	nil_m=6,
	undef_m=7,
	dbl_m=8
}avm_memcell_t;

typedef struct avm_memcell{
	avm_memcell_t type;
	union{
		double dblVal;
		int intVal;
		char *strVal;
		unsigned int boolVal;
		struct avm_table *tableVal;
		unsigned int funcVal;
		char *libfuncVal;
	}data;
}avm_memcell;

avm_memcell stack[AVM_STACKSIZE];
avm_memcell retval,ax,bx;

typedef struct avm_table_bucket{
	avm_memcell key;
	avm_memcell value;
	struct avm_table_bucket *next;
}avm_table_bucket;

typedef struct avm_table{
	unsigned int refCounter;
	struct	avm_table_bucket* strIndexed[AVM_TABLE_HASHSIZE];
	struct avm_table_bucket* numIndexed[AVM_TABLE_HASHSIZE];
	unsigned int total; 
}avm_table;

void initialiseArrays(){	
	totalStrConsts=0;
	totalIntConsts=0;
	totalDblConsts=0;
	totalNameLibfuncs=0;
	totalUserFuncs=0;
	intConsts=NULL;
	dblConsts=NULL;
	strConsts=NULL;
	namedLibfuncs=NULL;
	userFuncs=NULL;
}

char *typeStrings[]={
	"int",
	"string",
	"bool",
	"table",
	"userfunc",
	"libfunc",
	"nil",
	"undef",
	"double"
};

void avm_memcellclear(avm_memcell *m);
void avm_callsaveenvironment();
void avm_calllibfunc(char* id);
int avm_get_envalue(int numb);
avm_table* avm_tablenew();
char * avm_tostring(avm_memcell *s);
avm_memcell *avm_tablegetelem(avm_table* table,avm_memcell *i);
void avm_tablesetelem(avm_table* table,avm_memcell *i,avm_memcell *c);

void avm_tablebucketsdestroy(avm_table_bucket **p){
	unsigned int i;
	avm_table_bucket *b;
	for( i=0;i<AVM_TABLE_HASHSIZE;++i,++p){
		for(b=*p;b;)
			{
				avm_table_bucket *del=b;
				b=b->next;
				avm_memcellclear(&del->key);
				avm_memcellclear(&del->value);
				free(del);
			}
			p[i]=(avm_table_bucket*)0;
	}
}

void avm_tabledestroy(avm_table* t){
	avm_tablebucketsdestroy(t->strIndexed);
	avm_tablebucketsdestroy(t->numIndexed);
	free(t);
}


void avm_tableincrrefcounter(avm_table* t){
	++t->refCounter;
}

void avm_tabledecrrefcounter(avm_table* t){
	assert(t->refCounter>0);
	if(!--t->refCounter) avm_tabledestroy(t);
}

int LoadInstructions(){
	int i=0;
	FILE *f=fopen("BinaryCode.lol","rb");
	int MagicNumber;
	fread(&MagicNumber,sizeof(int),1,f);
	if(MagicNumber!=10){
		printf("Wrong type of file cant execute\n");
		exit(0);
	}
	fread(&totalIntConsts,sizeof(int),1,f);
	intConsts=(int*)malloc(totalIntConsts*sizeof(int));
	while(i<totalIntConsts){
		fread(&intConsts[i++],sizeof(int),1,f);
	}
	i=0;
	
	fread(&totalDblConsts,sizeof(int),1,f);
	dblConsts=(double*)malloc(totalDblConsts*sizeof(double));
	while(i<totalDblConsts){
		fread(&dblConsts[i],sizeof(double),1,f);
		i++;
	}

	i=0;
	fread(&totalStrConsts,sizeof(int),1,f);
	strConsts=(char**)malloc(totalStrConsts*sizeof(char*));
	while(i<totalStrConsts){
		fread(&strConsts[i],sizeof(char*),1,f);
		i++;
	}
	
	i=0;
	fread(&totalNameLibfuncs,sizeof(int),1,f);
	namedLibfuncs=(char**)malloc(totalNameLibfuncs*sizeof(char*));
	while(i<totalNameLibfuncs){
		fread(&namedLibfuncs[i],sizeof(char*),1,f);
		i++;
	}
	i=0;
	fread(&totalUserFuncs,sizeof(int),1,f);
	userFuncs=(userfunc*)malloc(totalUserFuncs*sizeof(userfunc));
	while(i<totalUserFuncs){
		fread(&userFuncs[i],sizeof(userfunc),1,f);
		fread(&userFuncs[i].id,sizeof(char*),1,f);
		fread(&userFuncs[i].address,sizeof(int),1,f);
		fread(&userFuncs[i].localSize,sizeof(int),1,f);
		i++;
	}
	fread(&codeSize,sizeof(int),1,f);
	code=(instruction*)malloc(codeSize*sizeof(instruction));
	i=0;
	instruction *tmp;
	while(i<codeSize){
		tmp=code+i;
		fread(&tmp->opcode,sizeof(int),1,f);
		switch(tmp->opcode){
			case assign_v:{
				tmp->result=(vmarg*)malloc(sizeof(vmarg));
				fread(&tmp->result->type,sizeof(int),1,f);
				fread(&tmp->result->val,sizeof(int),1,f);

				tmp->arg1=(vmarg*)malloc(sizeof(vmarg));
				fread(&tmp->arg1->type,sizeof(int),1,f);
				fread(&tmp->arg1->val,sizeof(int),1,f);
				break;
			}
			case add_v:
			case sub_v:
			case mul_v:
			case div_v:
			case mod_v:
			case jeq_v:
			case jne_v:
			case jgt_v:
			case jge_v:
			case jlt_v:
			case jle_v:
			case tablegetelem_v:
			case tablesetelem_v:{
				tmp->result=(vmarg*)malloc(sizeof(vmarg));
				fread(&tmp->result->type,sizeof(int),1,f);
				fread(&tmp->result->val,sizeof(int),1,f);
				
				tmp->arg1=(vmarg*)malloc(sizeof(vmarg));
				fread(&tmp->arg1->type,sizeof(int),1,f);
				fread(&tmp->arg1->val,sizeof(int),1,f);
		

				tmp->arg2=(vmarg*)malloc(sizeof(vmarg));
				fread(&tmp->arg2->type,sizeof(int),1,f);
				fread(&tmp->arg2->val,sizeof(int),1,f);
				break;
			}
			case funcenter_v:
			case funcexit_v:
			case jump_v:
			case newtable_v:{
				tmp->result=(vmarg*)malloc(sizeof(vmarg));
				fread(&tmp->result->type,sizeof(int),1,f);
				fread(&tmp->result->val,sizeof(int),1,f);
				break;
			}
			case call_v:
			case pusharg_v:{
				tmp->arg1=(vmarg*)malloc(sizeof(vmarg));
				fread(&tmp->arg1->type,sizeof(int),1,f);
				fread(&tmp->arg1->val,sizeof(int),1,f);
				break;			
			}		
		}
		i++;
	}
}

void avm_memcellclear(avm_memcell *m){
	if(m->type==undef_m) return;
 	else if(m->type==string_m){
		free(m->data.strVal);
	}
	else if(m->type==table_m){
		avm_tabledecrrefcounter(m->data.tableVal);
	}
	m->type=undef_m;
}

void avm_tablebucketsinit(avm_table_bucket **p){
	unsigned int i;
	for( i=0;i<AVM_TABLE_HASHSIZE;++i)
	{
		p[i]=(avm_table_bucket*)0;
	}
}

void avm_error(char *error_message){
	printf("Error: %s\n",error_message);
	executionFinished=1;
}

void avm_warning(char *message){
	printf("Warning: %s\n",message);
}

int consts_getint(int index){
	return intConsts[index];
}

double consts_getdbl(int index){
	return dblConsts[index];
}

char *consts_getstr(int index){
	return  strConsts[index];
}

char *libfuncs_getused(int index){
	return namedLibfuncs[index];
}
typedef unsigned char (*tobool_func_t)(avm_memcell*);

unsigned char int_tobool(avm_memcell *m) { return m->data.intVal!=0;}
unsigned char dbl_tobool(avm_memcell *m) { return m->data.dblVal!=0;}
unsigned char string_tobool(avm_memcell *m) { return m->data.strVal[0]!=0;} 
unsigned char bool_tobool(avm_memcell *m) { return m->data.boolVal;}
unsigned char table_tobool(avm_memcell *m) { return 1;}
unsigned char userfunc_tobool(avm_memcell *m) { return 1;}
unsigned char libfunc_tobool(avm_memcell *m) { return 1;}
unsigned char nil_tobool(avm_memcell *m) { return 0;}
unsigned char undef_tobool(avm_memcell *m) { assert(0); return 0;}

tobool_func_t toboolFuncs[]={
	int_tobool,
	string_tobool,
	bool_tobool,
	table_tobool,
	userfunc_tobool,
	libfunc_tobool,
	nil_tobool,
	undef_tobool,
	dbl_tobool
};

unsigned char avm_tobool(avm_memcell *m){
	assert(m->type>=0&&m->type<undef_m);
	return (*toboolFuncs[m->type])(m);
}

avm_memcell* avm_translate_operand(vmarg* arg,avm_memcell* req){
	switch(arg->type){
		//variables
		
		case global_a: 	{
				return &stack[AVM_STACKSIZE-1-arg->val];
		}
		case local_a: 	return &stack[topsp-arg->val];
		case formal_a:	return &stack[topsp+AVM_STACKENV_SIZE+1+arg->val];
		case retval_a: 	return &retval;
		case constint_a:{
			req->type=int_m;
			req->data.intVal=consts_getint(arg->val);
			return req;
		}
		case constdbl_a:{
			req->type=dbl_m;
			req->data.dblVal=consts_getdbl(arg->val);
			return req;
		}
		case string_a:{
			req->type=string_m;
			req->data.strVal=strdup(consts_getstr(arg->val));
			return req;
		}
		case bool_a:{
			req->type=bool_m;
			req->data.boolVal=arg->val;
			return req;
		}
		case nil_a:{
			req->type=nil_m; 
			return req;
		}
		case userfunc_a:{
			req->type=userfunc_m;
			req->data.funcVal=arg->val;//index
			return req;
		}
		case libfunc_a:{
			req->type=libfunc_m;
			req->data.libfuncVal=strdup(libfuncs_getused(arg->val));
			return req;
		}
	}
}

void assign_type(avm_memcell *lv,avm_memcell *rv){
	if(lv==rv) return;

	if(rv->type==undef_m) avm_warning("the Rvalue is undefined\n");
	memcpy(lv,rv,sizeof(avm_memcell));
	
	lv->type=rv->type;
	switch(lv->type){
		case int_m:{
			lv->data.intVal=rv->data.intVal;
			break;
		}
		case string_m:{
			lv->data.strVal=strdup(rv->data.strVal);
			break;
		}
		case bool_m:{
			lv->data.boolVal=rv->data.boolVal;
			break;
		}
		case table_m:{
			lv->data.tableVal=rv->data.tableVal;
			avm_tableincrrefcounter(lv->data.tableVal);
			break;
		}
		case userfunc_m:{
			lv->data.funcVal=rv->data.funcVal;
			break;
		}
		case libfunc_m:{
			lv->data.libfuncVal=rv->data.libfuncVal;
			break;
		}
		case dbl_m:{
			lv->data.dblVal=rv->data.dblVal;
			break;
		}
	}
}

void execute_assign (instruction *t){
	totalActuals=0;
	assert(t->result);
	assert(t->arg1);
	avm_memcell *lv=avm_translate_operand(t->result,(avm_memcell*) 0);
	avm_memcell *rv=avm_translate_operand(t->arg1,&ax);
	assign_type(lv,rv);
}

void StackCounter(){
	if(!top) avm_error("Kati ekanes lathos (Stack Overflow)");
	--top;
}

void avm_tableincrefcounter(avm_table *table){
	table->refCounter++;
}

void execute_uminus (instruction *t){return;}//make a funct that returns 0
void execute_and (instruction *t){return;}//make a funct that returns 0
void execute_or (instruction *t){return;}//make a funct that returns 0
void execute_not (instruction *t){return;}//make a funct that returns 0
void execute_jeq(instruction *t){
	assert(t->result->type==label_a);
	
	avm_memcell *rv1=avm_translate_operand(t->arg1,&ax);
	avm_memcell *rv2=avm_translate_operand(t->arg2,&bx);
	unsigned char result=0;
	if(rv1->type==undef_m||rv2->type==undef_m) {}
	else if(rv1->type==nil_m||rv2->type==nil_m) result=rv1->type==nil_m&&rv2->type==nil_m;
	else if(rv1->type==bool_m||rv2->type==bool_m) {result=(avm_tobool(rv1)==avm_tobool(rv2));}
	else if(rv1->type!=rv2->type){
		avm_error(" illegal assignment");
	}else{
		if(rv1->type==int_m){
			result=rv1->data.intVal==rv2->data.intVal;
		}
		else if(rv1->type==dbl_m){
			result=rv1->data.dblVal==rv2->data.dblVal;
		}
		else if(rv1->type==string_m){
			result=!strcmp(rv1->data.strVal,rv2->data.strVal);
		}
		else if(rv1->type==table_m){
			result=rv1->data.tableVal==rv2->data.tableVal;
		}
		else if(rv1->type==userfunc_m){
			result=rv1->data.funcVal==rv2->data.funcVal;						
		}
		else if(rv1->type==libfunc_m){
			result=!strcmp(rv1->data.libfuncVal,rv2->data.libfuncVal);
		}
	}

	if(!executionFinished&&result) pc=t->result->val;
}
void execute_jne (instruction *t){
	assert(t->result->type==label_a);
	
	avm_memcell *rv1=avm_translate_operand(t->arg1,&ax);
	avm_memcell *rv2=avm_translate_operand(t->arg2,&bx);
	unsigned char result=0;
	if(rv1->type==undef_m||rv2->type==undef_m) {}
	else if(rv1->type==nil_m||rv2->type==nil_m) result=rv1->type!=nil_m&&rv2->type!=nil_m;
	else if(rv1->type==bool_m||rv2->type==bool_m) {result=(avm_tobool(rv1)!=avm_tobool(rv2));}
	else if(rv1->type!=rv2->type){
		avm_error(" illegal assignment");
	}else{
		if(rv1->type==int_m){
			result=rv1->data.intVal!=rv2->data.intVal;
		}
		else if(rv1->type==dbl_m){
			result=rv1->data.dblVal!=rv2->data.dblVal;
		}
		else if(rv1->type==string_m){
			result=strcmp(rv1->data.strVal,rv2->data.strVal);
		}
		else if(rv1->type==table_m){
			result=rv1->data.tableVal!=rv2->data.tableVal;
		}
		else if(rv1->type==userfunc_m){
			result=rv1->data.funcVal!=rv2->data.funcVal;						
		}
		else if(rv1->type==libfunc_m){
			result=strcmp(rv1->data.libfuncVal,rv2->data.libfuncVal);
		}
	}
	if(!executionFinished&&result) pc=t->result->val;
}
void execute_jle (instruction *t){
	assert(t->result->type==label_a);
	
	avm_memcell *rv1=avm_translate_operand(t->arg1,&ax);
	avm_memcell *rv2=avm_translate_operand(t->arg2,&bx);
	unsigned char result=0;
	if(rv1->type==undef_m||rv2->type==undef_m) {}
	else if(rv1->type==nil_m||rv2->type==nil_m) result=rv1->type<=nil_m&&rv2->type<=nil_m;
	else if(rv1->type==bool_m||rv2->type==bool_m) {result=(avm_tobool(rv1)<=avm_tobool(rv2));}
	else if(rv1->type!=rv2->type){
		avm_error(" illegal assignment");
	}else{
		if(rv1->type==int_m){
			result=rv1->data.intVal<=rv2->data.intVal;
		}
		else if(rv1->type==dbl_m){
			result=rv1->data.dblVal<=rv2->data.dblVal;
		}
		else if(rv1->type==string_m){
			result=0<=strcmp(rv1->data.strVal,rv2->data.strVal);
		}
		else if(rv1->type==table_m){
			result=rv1->data.tableVal<=rv2->data.tableVal;
		}
		else if(rv1->type==userfunc_m){
			result=rv1->data.funcVal<=rv2->data.funcVal;						
		}
		else if(rv1->type==libfunc_m){
			result=0<=strcmp(rv1->data.libfuncVal,rv2->data.libfuncVal);
		}
	}

	if(!executionFinished&&result) pc=t->result->val;
}
void execute_jge (instruction *t){
	assert(t->result->type==label_a);
	
	avm_memcell *rv1=avm_translate_operand(t->arg1,&ax);
	avm_memcell *rv2=avm_translate_operand(t->arg2,&bx);
	unsigned char result=0;
	if(rv1->type==undef_m||rv2->type==undef_m) {}
	else if(rv1->type==nil_m||rv2->type==nil_m) result=rv1->type>=nil_m&&rv2->type>=nil_m;
	else if(rv1->type==bool_m||rv2->type==bool_m) {result=(avm_tobool(rv1)>=avm_tobool(rv2));}
	else if(rv1->type!=rv2->type){
		avm_error(" illegal assignment");
	}else{
		if(rv1->type==int_m){
			result=rv1->data.intVal>=rv2->data.intVal;
		}
		else if(rv1->type==dbl_m){
			result=rv1->data.dblVal>=rv2->data.dblVal;
		}
		else if(rv1->type==string_m){
			result=0>=strcmp(rv1->data.strVal,rv2->data.strVal);
		}
		else if(rv1->type==table_m){
			result=rv1->data.tableVal>=rv2->data.tableVal;
		}
		else if(rv1->type==userfunc_m){
			result=rv1->data.funcVal>=rv2->data.funcVal;						
		}
		else if(rv1->type==libfunc_m){
			result=0>=strcmp(rv1->data.libfuncVal,rv2->data.libfuncVal);
		}
	}

	if(!executionFinished&&result) pc=t->result->val;
}
void execute_jlt (instruction *t){
	assert(t->result->type==label_a);
	
	avm_memcell *rv1=avm_translate_operand(t->arg1,&ax);
	avm_memcell *rv2=avm_translate_operand(t->arg2,&bx);
	unsigned char result=0;
	if(rv1->type==undef_m||rv2->type==undef_m) {}
	else if(rv1->type==nil_m||rv2->type==nil_m) result=rv1->type<nil_m&&rv2->type<nil_m;
	else if(rv1->type==bool_m||rv2->type==bool_m) {result=(avm_tobool(rv1)<avm_tobool(rv2));}
	else if(rv1->type!=rv2->type){
		avm_error(" illegal assignment");
	}else{
		if(rv1->type==int_m){
			result=rv1->data.intVal<rv2->data.intVal;
		}
		else if(rv1->type==dbl_m){
			result=rv1->data.dblVal<rv2->data.dblVal;
		}
		else if(rv1->type==string_m){
			result=0<strcmp(rv1->data.strVal,rv2->data.strVal);
		}
		else if(rv1->type==table_m){
			result=rv1->data.tableVal<rv2->data.tableVal;
		}
		else if(rv1->type==userfunc_m){
			result=rv1->data.funcVal<rv2->data.funcVal;						
		}
		else if(rv1->type==libfunc_m){
			result=0<strcmp(rv1->data.libfuncVal,rv2->data.libfuncVal);
		}
	}

	if(!executionFinished&&result) pc=t->result->val;
}
void execute_jgt (instruction *t){
	assert(t->result->type==label_a);
	
	avm_memcell *rv1=avm_translate_operand(t->arg1,&ax);
	avm_memcell *rv2=avm_translate_operand(t->arg2,&bx);
	unsigned char result=0;
	if(rv1->type==undef_m||rv2->type==undef_m) {}
	else if(rv1->type==nil_m||rv2->type==nil_m) result=rv1->type>nil_m&&rv2->type>nil_m;
	else if(rv1->type==bool_m||rv2->type==bool_m) {result=(avm_tobool(rv1)>avm_tobool(rv2));}
	else if(rv1->type!=rv2->type){
		avm_error(" illegal assignment");
	}else{
		if(rv1->type==int_m){
			result=rv1->data.intVal>rv2->data.intVal;
		}
		else if(rv1->type==dbl_m){
			result=rv1->data.dblVal>rv2->data.dblVal;
		}
		else if(rv1->type==string_m){
			result=0>strcmp(rv1->data.strVal,rv2->data.strVal);
		}
		else if(rv1->type==table_m){
			result=rv1->data.tableVal>rv2->data.tableVal;
		}
		else if(rv1->type==userfunc_m){
			result=rv1->data.funcVal>rv2->data.funcVal;						
		}
		else if(rv1->type==libfunc_m){
			result=0>strcmp(rv1->data.libfuncVal,rv2->data.libfuncVal);
		}
	}

	if(!executionFinished&&result) pc=t->result->val;
}
void execute_jump (instruction *t){
	assert(t->result);
	if(!executionFinished) pc=t->result->val;
}
void execute_call (instruction *t){
	avm_memcell *callArg=avm_translate_operand(t->arg1,&ax);
	avm_callsaveenvironment();
	if(callArg->type==libfunc_m) avm_calllibfunc(callArg->data.libfuncVal);
	else if(callArg->type==userfunc_m){
		pc=userFuncs[callArg->data.funcVal].address;
		if(!code[pc].opcode==funcenter_v) avm_error("something went wrong with instructions");
	}
}
void execute_pusharg (instruction *t){
	avm_memcell *pushedArg=avm_translate_operand(t->arg1,&ax);
	
	assign_type(&stack[top],pushedArg);
	++totalActuals;
	StackCounter();	
}
void execute_funcenter (instruction *t){
	flag_enter=1;
	avm_memcell *funcenter=avm_translate_operand(t->result,&ax);
	totalActuals=0;
	topsp=top;
	top=top-userFuncs[funcenter->data.funcVal].localSize;
}
void execute_funcexit(instruction* unused){
	if(library_flag==0) flag_enter=0;
	library_flag=0;
	unsigned int oldTop=top;
	top=avm_get_envalue(topsp+AVM_SAVEDTOP_OFFSET);
	pc=avm_get_envalue(topsp+AVM_SAVEDPC_OFFSET);
	topsp=avm_get_envalue(topsp+AVM_SAVEDTOPSP_OFFSET);
	while(oldTop++<=top) avm_memcellclear(&stack[oldTop]);
}
void execute_newtable(instruction *instr){}
void execute_tablegetelem(instruction *instr){}
void execute_tablesetelem(instruction *instr){}
void execute_nop (instruction *t){}

typedef double(*arithmetic_func_t)(double x,double y);

double add_impl(double x,double y){return x+y;}
double sub_impl(double x,double y){return x-y;}
double mul_impl(double x,double y){return x*y;}
double div_impl(double x,double y){
	if(y==0) avm_error("U are trying to divide with zero\n");
	return x/y;
}
double mod_impl(double x,double y){
	if(y==0) avm_error("U are trying to divide with zero\n");
	return ((unsigned) x)%((unsigned)y);
}

arithmetic_func_t arithmeticFuncs[]={
	add_impl,
	sub_impl,
	mul_impl,
	div_impl,
	mod_impl
};

void execute_arithmetic (instruction *instr){
	avm_memcell *lv=avm_translate_operand(instr->result,(avm_memcell*) 0);
	avm_memcell *rv1=avm_translate_operand(instr->arg1,&ax);
	avm_memcell *rv2=avm_translate_operand(instr->arg2,&bx);
	assert(rv1&&rv2);
	
	if((rv1->type!=int_m||rv2->type!=int_m)&&(rv1->type!=dbl_m||rv2->type!=int_m)
&&(rv1->type!=int_m||rv2->type!=dbl_m)&&(rv1->type!=dbl_m||rv2->type!=dbl_m))	avm_error("not a number in arithmetic!");

	else{
		arithmetic_func_t op=arithmeticFuncs[instr->opcode-add_v];
		if(rv1->type==int_m&&rv2->type==int_m){
			lv->type=int_m;
			lv->data.intVal=(*op)(rv1->data.intVal,rv2->data.intVal);
		}else if(rv1->type==int_m&&rv2->type==dbl_m){
			lv->type=dbl_m;
			lv->data.dblVal=(*op)(rv1->data.intVal,rv2->data.dblVal);
		}else if(rv1->type==dbl_m&&rv2->type==int_m){
			lv->type=dbl_m;
			lv->data.dblVal=(*op)(rv1->data.dblVal,rv2->data.intVal);
		}else if(rv1->type==dbl_m&&rv2->type==dbl_m){
			lv->type=dbl_m;
			lv->data.dblVal=(*op)(rv1->data.dblVal,rv2->data.dblVal);
		}else if(rv1->type==int_m&&rv2->type==undef_m){			
			lv->type=int_m;
			lv->data.intVal=(*op)(rv1->data.intVal,rv2->data.intVal);
		}else if(rv1->type==undef_m&&rv2->type==int_m){
			lv->type=int_m;
			lv->data.intVal=(*op)(rv1->data.intVal,rv2->data.intVal);
		}else if(rv1->type==dbl_m&&rv2->type==undef_m){
			lv->type=dbl_m;
			lv->data.dblVal=(*op)(rv1->data.dblVal,rv2->data.dblVal);
		}else if(rv1->type==undef_m&&rv2->type==dbl_m){
			lv->type=dbl_m;
			lv->data.dblVal=(*op)(rv1->data.dblVal,rv2->data.dblVal);
		}else printf("heeeere what\n");
	}
}

typedef void(*execute_func_t)(instruction*);
execute_func_t executeFuncs[]={
/*assign_v,add_v,sub_v,mul_v,div_v,mod_v,uminus_v,and_v,or_v,not_v,jeq_v,jne_v,
jgt_v,jlt_v,jge_v,jle_v,jump_v,call_v,funcenter_v,funcexit_v,newtable_v,
tablegetelem_v,tablesetelem_v,pusharg_v,nop_v,*/
	execute_assign,
	execute_add,
	execute_sub,
	execute_mul,
	execute_div,
	execute_mod,
	execute_uminus,//not existent command here
	execute_and,//not existent command here
	execute_or,//not existent command here
	execute_not,//not existent command here
	execute_jeq,
	execute_jne,
	execute_jgt,	
	execute_jlt,
	execute_jge,
	execute_jle,
	execute_jump,
	execute_call,
	execute_funcenter,
	execute_funcexit,
	execute_newtable,
	execute_tablegetelem,
	execute_tablesetelem,
	execute_pusharg,
	execute_nop
};

void execute_cycle(){
	if(executionFinished) return;
	else{
		if(pc==AVM_ENDING_PC){
			executionFinished=1;
			return;		
		}
		else{
			assert(pc<AVM_ENDING_PC);
			instruction *instr=instructions+pc;
			assert(instr->opcode>=0&&instr->opcode<=AVM_MAX_INSTRUCTIONS);
			if(instr->srcLine) currLine=instr->srcLine;
			unsigned int oldPC=pc;
			(*executeFuncs[instr->opcode])(instr);
			if(pc==oldPC) ++pc;	
		}
	}
}

void avm_push_envalue(int numb){
	stack[top].type=int_m;
	stack[top].data.intVal= numb;
	StackCounter();
}

int avm_get_envalue(int numb){
	return stack[numb].data.intVal;
}

void avm_callsaveenvironment(){
	avm_push_envalue(totalActuals);
	avm_push_envalue(pc+1);
	avm_push_envalue(top+totalActuals+2);
	avm_push_envalue(topsp);
}

unsigned int avm_totalactuals(void){
	return avm_get_envalue(topsp+AVM_NUMACTUALS_OFFSET);
}

char * avm_tostring(avm_memcell *s){
	switch(s->type){
		
		case int_m:{
			char *intM=(char*)malloc(sizeof(char)*10);
			sprintf(intM,"%d",s->data.intVal);
			return intM;
		}
		case string_m:{
			return strdup(s->data.strVal);
		}
		case bool_m:{
			if(s->data.boolVal==1) return strdup("true");
			else return strdup("false");
		}
		case table_m:{
			avm_warning("Trying to pring a table not fixed\n");
			return strdup("nil");
		}
		case userfunc_m:{
			printf("User function :");
			char *intM=(char*)malloc(sizeof(char)*10);
			sprintf(intM,"%d",s->data.funcVal);
			return intM;
		}
		case libfunc_m:{
			printf("Library function :");
			return strdup(s->data.libfuncVal);
		}
		case nil_m:{
			return strdup("nil");
		}
		case undef_m:{
			char *intM=(char*)malloc(sizeof(char)*10);
			sprintf(intM,"%d",s->data.intVal);
			return intM;
		}
		case dbl_m:{
			char *dblM=(char*)malloc(sizeof(char)*20);
			sprintf(dblM,"%lf",s->data.dblVal);
			return dblM;
			break;
		}
		default:
			return strdup("nil");
	}
}

avm_memcell *avm_getactual(unsigned int i){
	assert(i<avm_totalactuals());
	return &stack[topsp+AVM_STACKENV_SIZE+1+i];
}
//LIBRARY FUNCTIONS DECLARATION
void libfunc_print(void){
	unsigned int i,n=avm_totalactuals();
	for(i=0;i<n;++i){
		char *s=avm_tostring(avm_getactual(i));
		if(!s) return ;
		fputs(strdup(s),stdout);
		free(s);
	}
}

void libfunc_totalarguments(void){
	unsigned int p_topsp=avm_get_envalue(topsp+AVM_SAVEDTOPSP_OFFSET);
	avm_memcellclear(&retval);
	if(!flag_enter){
		avm_warning("'totalarguments' called outside a function!");
		retval.type=nil_m;
	}else{
		retval.type=int_m;
		retval.data.intVal=avm_get_envalue(p_topsp+AVM_NUMACTUALS_OFFSET);
	}
}



void libfunc_typeof(void){
	unsigned int n=avm_totalactuals();
	if(n!=1) avm_error("one argument exprected in 'typeof'!\n");
	else{
		avm_memcellclear(&retval);
		retval.type=string_m;
		retval.data.strVal=strdup(typeStrings[avm_getactual(0)->type]);
	}
}

void libfunc_input(){
	unsigned int n=avm_totalactuals();
	if(n!=0){
		avm_error("no argument expected in 'input'!\n");
	}else{
		char *str;
		int tmp;
		size_t strlen=0;
		str=realloc(NULL,sizeof(char)*50);
		while(EOF!=(tmp=fgetc(stdin))&&tmp!='\n'){
			str[strlen++]=tmp;
			if(strlen==50) str=realloc(str,sizeof(char)*(66));
		}
		str[strlen++]='\0';
		if(!strcmp(str,"true")){
			retval.type=bool_m;
			retval.data.boolVal=1;
		}else if(!strcmp(str,"false")){
			retval.type=bool_m;
			retval.data.boolVal=0;
		}
		else if(!strcmp(str,"nil")){
			retval.type=nil_m;
		}else if(atof(str)!=0||!strcmp(str,"0")){
			retval.type=dbl_m;
			retval.data.dblVal=atof(str);
		}else{
			retval.type=string_m;
			retval.data.strVal=str;
		}
	} 
}

void libfunc_strtonum(){
	unsigned int n=avm_totalactuals();
	if(n!=1) avm_error("one argument exprected in 'strtonum'!\n");
	else{
		if(avm_getactual(0)->type!=string_m) avm_error("argument must be string\n");
		else{
			int number=atof(avm_getactual(0)->data.strVal);
			if(number==0&&strcmp(avm_getactual(0)->data.strVal,"0")){
				retval.type=nil_m;
			}
			else{
				retval.type=dbl_m;
				retval.data.dblVal=number;
			}
		}
	}
}

void libfunc_sqrt(){//theloume periorismo iso i mmegalitero tou miden kai ena argumetn
	unsigned int n=avm_totalactuals();
	if(n!=1) avm_error("one argument exprected in 'sqrt'!\n");
	else{
		if(avm_getactual(0)->type==dbl_m) {
			retval.type=dbl_m;
			if(avm_getactual(0)->data.dblVal<0){
				retval.type=nil_m;
				avm_warning(" ego tha s matho mathimatika to argument prepei na einai thetiko");				
				return;
			}
			retval.data.dblVal=sqrt(avm_getactual(0)->data.dblVal);
		}
		else if(avm_getactual(0)->type==int_m) {
			retval.type=dbl_m;
			if(avm_getactual(0)->data.intVal<0) {
				retval.type=nil_m;
				avm_warning(" ego tha s matho mathimatika sqrt>=0");
				return;
			}
			retval.data.dblVal=sqrt(avm_getactual(0)->data.intVal);
		}
		else {
			avm_warning("unknown type inside 'cos' function");
			retval.type=nil_m;
		}

	}
}

void libfunc_cos(){
	unsigned int n=avm_totalactuals();
	if(n!=1) avm_error("one argument exprected in 'cos'!\n");
	else{
		if(avm_getactual(0)->type==dbl_m) {
			retval.type=dbl_m;
			retval.data.dblVal=cos(avm_getactual(0)->data.dblVal);
		}
		else if(avm_getactual(0)->type==int_m) {
			retval.type=dbl_m;
			retval.data.dblVal=cos(avm_getactual(0)->data.intVal);
		}
		else {
			avm_warning("unknown type inside 'cos' function");
			retval.type=nil_m;
		}
	}
}

void libfunc_sin(){
	unsigned int n=avm_totalactuals();
	if(n!=1) avm_error("one argument exprected in 'sin'!\n");
	else{
		if(avm_getactual(0)->type==dbl_m) {
			retval.type=dbl_m;
			retval.data.dblVal=sin(avm_getactual(0)->data.dblVal);
		}
		else if(avm_getactual(0)->type==int_m) {
			retval.type=dbl_m;
			retval.data.dblVal=sin(avm_getactual(0)->data.intVal);
		}
		else {
			avm_warning("unknown type inside 'sin' function");
			retval.type=nil_m;
		}
	}
}

void avm_calllibfunc(char* id){
	if(!isLibFunc(id)){
		avm_error("unsupported lib func called!\n");
	}else{
		topsp=top;
		totalActuals=0;
		if(!strcmp(id,"print")) libfunc_print();
		else if(!strcmp(id,"typeof")) libfunc_typeof();
		else if(!strcmp(id,"totalarguments")) libfunc_totalarguments();
		else if(!strcmp(id,"strtonum")) libfunc_strtonum();
		else if(!strcmp(id,"sqrt")) libfunc_sqrt();
		else if(!strcmp(id,"cos")) libfunc_cos();
		else if(!strcmp(id,"sin")) libfunc_sin();
		else if(!strcmp(id,"input")) libfunc_input();
		else printf("Not any other function fixed yet\n");
		if(!executionFinished) {library_flag=1;execute_funcexit((instruction*)0);}
	}
}

void SetStackUndef(){
	int i;
	for(i=0;i<AVM_STACKSIZE;i++) stack[i].type=undef_m;
}

void run_virtual_machine(){
	initialiseArrays();
	SetStackUndef();
	LoadInstructions();
	printf("--Virtual Machine--\n");
	int i=0;
	top=topsp=AVM_STACKSIZE-(totalIntConsts+totalDblConsts+totalStrConsts)-1;
	while(executionFinished==0){
		execute_cycle();
		i++;
	}
	return;
}

