#include "quadlibrary.h"

#define EXPAND_SIZEINST 1024
#define CURR_SIZEINST (totalINST*sizeof(instruction))
#define NEW_SIZEINST (EXPAND_SIZEINST*sizeof(instruction)+CURR_SIZEINST)

typedef enum vmarg_t{
	label_a=0,
	global_a=1,
	formal_a=2,
	local_a=3,
	constint_a=4,
	string_a=5,
	bool_a=6,
	nil_a=7,
	userfunc_a=8,
	libfunc_a=9,
	retval_a=10,
	constdbl_a=11
}vmarg_t;

typedef enum vmopcode{
assign_v,add_v,sub_v,mul_v,div_v,mod_v,uminus_v,and_v,or_v,not_v,jeq_v,jne_v,
jgt_v,jlt_v,jge_v,jle_v,jump_v,call_v,funcenter_v,funcexit_v,newtable_v,
tablegetelem_v,tablesetelem_v,pusharg_v,nop_v,
}vmopcode;

typedef struct vmarg{
	char *name;	
	vmarg_t type;
	unsigned int val;
}vmarg;

typedef struct instruction{
	vmopcode opcode;
	vmarg *result;
	vmarg *arg1;
	vmarg *arg2;
	unsigned int srcLine;
}instruction;

typedef struct userfunc{
	unsigned int address;
	unsigned int localSize;
	char *id;
}userfunc;

typedef struct retList{
	int instrNo;
	struct retList *next;
}retList;

typedef struct FuncStack{
	ST_node *sym;
	retList *retHead;
	int label_jump;
	struct FuncStack *next;
}FuncStack;

typedef struct incomplete_jump{
	unsigned int instrNo;
	unsigned int iaddress;
	struct incomplete_jump *next;
}incomplete_jump;

incomplete_jump *ij_head=NULL;
int ij_total=0;
int currproccessedquad=0;
int totalINST=0;
int currINST=0;
instruction *instructions=NULL;
FuncStack *FuncStackHead=NULL;

int MagicNumber=10;

unsigned int totalIntConsts;
unsigned int totalDblConsts;
unsigned int totalStrConsts;
unsigned int totalNameLibfuncs;
unsigned int totalUserFuncs;
int* intConsts;
double* dblConsts;
char** strConsts;
char** namedLibfuncs;
userfunc* userFuncs;

void reset_operand(vmarg **ptr){
	*ptr=(vmarg*)malloc(sizeof(vmarg));
}

unsigned int consts_newstring(char *s){
	if(totalStrConsts==0){
		strConsts=(char **)malloc(sizeof(char*));
		strConsts[0]=strdup(s);
		totalStrConsts++;
		return totalStrConsts-1;
	}else{
		strConsts=(char **)realloc(strConsts,(totalStrConsts+1)*sizeof(char*));
		strConsts[totalStrConsts]=strdup(s);
		totalStrConsts++;
		return totalStrConsts-1;
	}
}

unsigned int consts_newint(int n){
	if(totalIntConsts==0){
		intConsts=(int *)malloc(sizeof(int));
		intConsts[0]=n;
		totalIntConsts++;
		return totalIntConsts-1;
	}else{
		intConsts=(int *)realloc(intConsts,(totalIntConsts+1)*sizeof(int));
		intConsts[totalIntConsts]=n;
		totalIntConsts++;
		return totalIntConsts-1;
	}
}

unsigned int consts_newdbl(double n){
	if(totalDblConsts==0){
		dblConsts=(double *)malloc(sizeof(double));
		dblConsts[0]=n;
		totalDblConsts++;
		return totalDblConsts-1;
	}else{
		dblConsts=(double *)realloc(dblConsts,(totalDblConsts+1)*sizeof(double));
		dblConsts[totalDblConsts]=n;
		totalDblConsts++;
		return totalDblConsts-1;
	}
}

unsigned int locateLibfunc(char *s){
	int i=0;	
	while(i<totalNameLibfuncs){
		if(!strcmp(namedLibfuncs[i],s)) return i;
		i++;
	}
}

void InsertLibFuncs(){
	namedLibfuncs=(char**)malloc(12*sizeof(char*));
	namedLibfuncs[0]=strdup("print");
	namedLibfuncs[1]=strdup("input");
	namedLibfuncs[2]=strdup("objectmemberkeys");
	namedLibfuncs[3]=strdup("objecttotalmembers");
	namedLibfuncs[4]=strdup("objectcopy");
	namedLibfuncs[5]=strdup("totalarguments");
	namedLibfuncs[6]=strdup("argument");
	namedLibfuncs[7]=strdup("typeof");
	namedLibfuncs[8]=strdup("strtonum");
	namedLibfuncs[9]=strdup("sqrt");
	namedLibfuncs[10]=strdup("cos");
	namedLibfuncs[11]=strdup("sin");
	totalNameLibfuncs=12;
}

unsigned int userfuncs_newfunc(ST_node *sym){
	int i=0;//userFuncs
	while(i<totalUserFuncs){
		if(!strcmp(userFuncs[i].id,sym->name)) return i;
		i++;
	}
	if(totalUserFuncs==0){
		userFuncs=(userfunc*)malloc(sizeof(userfunc));
		userFuncs[0].address=sym->taddress;
		userFuncs[0].localSize=sym->VariablesNO;
		userFuncs[0].id=strdup(sym->name);
		totalUserFuncs++;
		return totalUserFuncs-1;
	}else{
		userFuncs=(userfunc*)realloc(userFuncs,(totalUserFuncs+1)*sizeof(userfunc));
		userFuncs[totalUserFuncs].address=sym->taddress;
		userFuncs[totalUserFuncs].localSize=sym->VariablesNO;
		userFuncs[totalUserFuncs].id=strdup(sym->name);
		totalUserFuncs++;
		return totalUserFuncs-1;
	}
}

vmarg * make_operand(expr *e){
	
	if(!e) return NULL;
	vmarg *arg=(vmarg *)malloc(sizeof(vmarg));	
	if(e->sym)arg->name=e->sym->name;

	switch(e->type){
		case var_e:
		case tableitem_e:
		case arithmexpr_e:
		case boolexpr_e:
		case newtable_e:
		case assignexpr_e:{
			assert(e->sym);
			arg->val=e->sym->offset;
			
			switch(e->sym->scopespace){
				case programvar: arg->type=global_a; break;
				case fuctionlocal: arg->type=local_a; break;
				case formalarg:	 arg->type=formal_a; break;
				default: assert(0);
			}
		break;
		}
		case constbool_e:{//Check it here
			if(e->value.boolValue) arg->val=1;
			else arg->val=0;
			arg->type=bool_a;
			break;
		}
		case conststr_e:{
			arg->val=consts_newstring(e->value.strValue);
			arg->type=string_a;
			break;
		}
		case constint_e:{
			arg->val=consts_newint(e->value.intValue);
			arg->type=constint_a;
			break;
		}
		case constdbl_e:{
			arg->val=consts_newdbl(e->value.realValue);
			arg->type=constdbl_a;
			break;
		}
		case nil_e:{
			arg->type=nil_a;
			break;
		}
		case programfunc_e:{
			arg->type=userfunc_a;
			arg->val=userfuncs_newfunc(e->sym);
			break;
		}
		case libraryfunc_e:{
			arg->type=libfunc_a;
			arg->val=locateLibfunc(e->sym->name);
			break;
		}
		default:assert(0);
	}
	return arg;
}

void make_intoperand(vmarg *arg,int val){
	arg->val=consts_newint(val);
	arg->type=constint_a;
}

void make_dbloperand(vmarg *arg,double val){
	arg->val=consts_newdbl(val);
	arg->type=constdbl_a;
}

vmarg * make_booloperand(vmarg *arg,int val){
	arg->val=val;
	arg->type=bool_a;
	return arg;
}

vmarg * make_retvaloperand(vmarg *arg){
	arg->type=retval_a;
	arg->val=0;//here is the val that is printed int (retval)x
	return arg;
}

void add_incomplete_jump(unsigned int instrNo,unsigned int iaddress){
	incomplete_jump *tmp=(incomplete_jump*)malloc(sizeof(incomplete_jump));
	tmp->instrNo=instrNo;
	tmp->iaddress=iaddress;
	tmp->next=ij_head;
	ij_head=tmp;	
	++ij_total;
}

void patch_incomplete_jumps(){
	incomplete_jump *tmp=ij_head;
	while(tmp){
		if(tmp->iaddress==currQuad){
			instructions[tmp->instrNo].result->val=currINST;
		}else{
			instructions[tmp->instrNo].result->val=quadArraylist[tmp->iaddress].taddress;
		}
		tmp=tmp->next;
	}
}

void expand_SizeINST(){
	instruction *newArray=(instruction*)malloc(NEW_SIZEINST);
	if(instructions){
		memcpy(newArray,instructions,CURR_SIZEINST);
		free(instructions);
	}
	instructions=newArray;
	totalINST+=EXPAND_SIZEINST;
}

void emitV2(instruction *newINST)
{
	if(currINST==totalINST) expand_SizeINST();
	instruction *New_node=instructions+currINST++;
	New_node->opcode=newINST->opcode;
	New_node->result=newINST->result;
	New_node->arg1=newINST->arg1;
	New_node->arg2=newINST->arg2;
	New_node->srcLine=newINST->srcLine;
}

void generate(vmopcode op,quad* Q){
	instruction *t=(instruction*)malloc(sizeof(instruction));
	t->opcode=op;
	t->arg1=make_operand(Q->arg1);
	t->arg2=make_operand(Q->arg2);
	t->result=make_operand(Q->result);
	Q->taddress=currINST;
	emitV2(t);
}

void generate_ADD(quad* Q) {generate(add_v,Q);}
void generate_SUB(quad* Q) {generate(sub_v,Q);}
void generate_MUL(quad* Q) {generate(mul_v,Q);}
void generate_DIV(quad* Q) {generate(div_v,Q);}
void generate_MOD(quad* Q) {generate(mod_v,Q);}
void generate_NEWTABLE(quad* Q) {generate(newtable_v,Q);}
void generate_TABLEGETELEM(quad* Q) {generate(tablegetelem_v,Q);}
void generate_TABLESETELEM(quad* Q) {generate(tablesetelem_v,Q);}
void generate_ASSIGN(quad* Q) {generate(assign_v,Q);}
void generate_NOP(quad* Q) {
	instruction *t=(instruction*)malloc(sizeof(instruction));
	t->opcode=nop_v;
	emitV2(t);
}

void generate_relational(vmopcode op,quad* Q){//ok except  currproccessedquad
	instruction *t=(instruction*)malloc(sizeof(instruction));
	t->opcode=op;
	t->arg1=make_operand(Q->arg1);
	
	if(Q->result) t->arg2=make_operand(Q->result);
	else t->arg2=make_operand(Q->arg2);
	reset_operand(&t->result);
	t->result->type=label_a;
	if(Q->label<currproccessedquad){
		t->result->val=quadArraylist[Q->label].taddress;
	}else{
		add_incomplete_jump(currINST,Q->label);
	}
	Q->taddress=currINST;
	emitV2(t);
}

void generate_JUMP(quad* Q){generate_relational(jump_v,Q);}
void generate_IF_EQ(quad* Q){generate_relational(jeq_v,Q);}
void generate_IF_NOTEQ(quad* Q){generate_relational(jne_v,Q);}
void generate_IF_GREATER(quad* Q){generate_relational(jgt_v,Q);}
void generate_IF_GREATEREQ(quad* Q){generate_relational(jge_v,Q);}
void generate_IF_LESS(quad* Q){generate_relational(jlt_v,Q);}
void generate_IF_LESSEQ(quad* Q){generate_relational(jle_v,Q);}

void generate_NOT(){}
void generate_OR(){}
void generate_UMINUS(){}
void generate_AND(){}

void generate_PARAM(quad *Q){
	instruction *t=(instruction*)malloc(sizeof(instruction));
	
	Q->taddress=currINST;
	t->opcode=pusharg_v;
	t->arg1=make_operand(Q->result);
	emitV2(t);
}

void generate_CALL(quad *Q){
	instruction *t=(instruction*)malloc(sizeof(instruction));
	Q->taddress=currINST;
	t->opcode=call_v;
	t->arg1=make_operand(Q->result);
	t->arg2=NULL;
	emitV2(t);
}

void generate_GETRETVAL(quad *Q){
	instruction *t=(instruction*)malloc(sizeof(instruction));
	Q->taddress=currINST;
	t->opcode=assign_v;
	reset_operand(&t->result);
	reset_operand(&t->arg1);
	t->result=make_operand(Q->result);
	t->arg1=make_retvaloperand(t->arg1);
	t->arg2=NULL;
	emitV2(t);
}

FuncStack* pushFunc(ST_node *fun){
	FuncStack *tmp=(FuncStack*)malloc(sizeof(FuncStack));
	tmp->sym=fun;
	tmp->retHead=NULL;
	tmp->label_jump=currINST-1;
	if(FuncStackHead==NULL) tmp->next=NULL;
	else tmp->next=FuncStackHead;
	return tmp;
}

typedef struct jumpStack{
	int number;
	struct jumpStack *next;
}jumpStack;

jumpStack *Jhead=NULL;

jumpStack* add_StartJump(int number){
		jumpStack *tmp=(jumpStack*)malloc(sizeof(jumpStack));
		tmp->number=number;
		if(Jhead==NULL){
			tmp->next=NULL;
		}else{
			tmp->next=Jhead;
		}
}

int popjump()
{	
	jumpStack *tmp=Jhead;
	int number;
	if(tmp==NULL) return 0;
	else{
		number=Jhead->number;
		tmp=tmp->next;
		free(Jhead);
		Jhead=tmp;
		return number;
	}
}


void generate_FUNCSTART(quad *Q){
	
	instruction *tmp=(instruction*)malloc(sizeof(instruction));
	tmp->opcode=jump_v;
	reset_operand(&tmp->result);
	tmp->result->type=label_a;
	tmp->result->val=-1;
	tmp->arg1=NULL;
	tmp->arg2=NULL;
	Jhead=add_StartJump(currINST);
	emitV2(tmp);
	
	ST_node *f=Q->result->sym;
	f->taddress=currINST;
	Q->taddress=currINST;
	Q->result->sym->taddress=currINST;
	FuncStackHead=pushFunc(f);
	

	instruction *t=(instruction*)malloc(sizeof(instruction));
	reset_operand(&t->result);

	t->opcode=funcenter_v;
	reset_operand(&t->result);
	t->result=make_operand(Q->result);
	t->result->val=userfuncs_newfunc(f);
		
	t->arg1=NULL;
	t->arg2=NULL;
	emitV2(t);
}

void append(int instrNO)
{
	retList *tmp=FuncStackHead->retHead;
	retList *new_node=(retList*)malloc(sizeof(retList));
	new_node->next=tmp;
	new_node->instrNo=instrNO;
	FuncStackHead->retHead=new_node;
}

void generate_RETURN(quad *Q){
	instruction *t=(instruction*)malloc(sizeof(instruction));
	Q->taddress=currINST;
	t->opcode=assign_v;
	reset_operand(&t->result);
	t->result=make_retvaloperand(t->result);
	if(Q->arg1) t->arg1=make_operand(Q->arg1);
	else if(Q->arg2) t->arg1=make_operand(Q->arg2);
	emitV2(t);
	
	append(currINST);
	
	t->opcode=jump_v;
	reset_operand(&t->result);
	t->result->type=label_a;
	t->arg1=NULL;
	t->arg2=NULL;
	emitV2(t);
}

FuncStack* popFunc(){
	FuncStack *tmp=FuncStackHead;
	FuncStack *tmp2;
	if(tmp==NULL) return NULL;
	else{
		tmp2=tmp=FuncStackHead;
		tmp=tmp->next;
		free(FuncStackHead);
		FuncStackHead=tmp;
		return tmp2;
	}
}

void backpatch(){
	
	FuncStack *f=popFunc();
	if(f==NULL) return;
	if(f->retHead==NULL) return;
	retList *tmp=f->retHead;
	while(tmp){
		instruction tmp2=instructions[tmp->instrNo];
		tmp2.result->val=currINST;
		tmp=tmp->next;
	}
}



void generate_FUNCEND(quad *Q){

	backpatch();
	Q->taddress=currINST;
	instruction *t=(instruction*)malloc(sizeof(instruction));
	t->opcode=funcexit_v;
	reset_operand(&t->result);
	t->result=make_operand(Q->result);

	t->arg1=NULL;
	t->arg2=NULL;
	emitV2(t);
	instructions[popjump()].result->val=currINST;
	
}

typedef void (*generator_func_t)(quad*);

generator_func_t generators[]={
	generate_ASSIGN,
	generate_ADD,
	generate_SUB,
	generate_MUL,
	generate_DIV,
	generate_MOD,
	generate_UMINUS,
	generate_AND,
	generate_OR,
	generate_NOT,
	generate_IF_EQ,
	generate_IF_NOTEQ,
	generate_IF_LESSEQ,
	generate_IF_GREATEREQ,
	generate_IF_LESS,
	generate_IF_GREATER,
	generate_JUMP,
	generate_CALL,
	generate_PARAM,
	generate_RETURN,
	generate_GETRETVAL,
	generate_FUNCSTART,
	generate_FUNCEND,
	generate_NEWTABLE,
	generate_TABLEGETELEM,
	generate_TABLESETELEM,
	generate_NOP
};

void generateT()
{
	InsertLibFuncs();
	for(currproccessedquad=0;currproccessedquad<currQuad;++currproccessedquad) {
		(*generators[quadArraylist[currproccessedquad].op])(quadArraylist+currproccessedquad);
	}
}

char *op2StringINST(vmopcode op)
{	
	if(op==assign_v) return "assign_v";
	else if(op==add_v) return "add_v";
	else if(op==sub_v) return "sub_v";
	else if(op==mul_v) return "mul_v";
	else if(op==div_v) return "div_v";
	else if(op==mod_v) return "mod_v";
	else if(op==uminus_v) return "uminus_v";
	else if(op==and_v) return "and_v";
	else if(op==or_v) return "or_v";
	else if(op==not_v) return "not_v";
	else if(op==jeq_v) return "jeq_v";
	else if(op==jne_v) return "jne_v";
	else if(op==jgt_v) return "jgt_v";
	else if(op==jlt_v) return "jlt_v";
	else if(op==jge_v) return "jge_v";
	else if(op==jle_v) return "jle_v";
	else if(op==jump_v) return "jump_v";
	else if(op==call_v) return "call_v";
	else if(op==pusharg_v) return "pusharg_v";
	else if(op==nop_v) return "nop_v";
	else if(op==funcenter_v) return "funcenter_v";
	else if(op==funcexit_v) return "funcexit_v";
	else if(op==tablegetelem_v) return "tablegetelem_v";
	else if(op==tablesetelem_v) return "tablesetelem_v";
	else if(op==newtable_v) return "newtable_v";
}

void InstructionArqPrint(vmarg *arg){
	
	vmarg_t tmp=arg->type;
	if(tmp==label_a) printf("0%d(label)%d",arg->type,arg->val);
	else if(tmp==global_a) printf("0%d(global)%d:%s",arg->type,arg->val,arg->name);
	else if(tmp==formal_a) printf("0%d(formal)%d:%s",arg->type,arg->val,arg->name);
	else if(tmp==local_a) printf("0%d(local)%d:%s",arg->type,arg->val,arg->name);
	else if(tmp==constint_a) printf("0%d(int)%d:%d",arg->type,arg->val,intConsts[arg->val]);
	else if(tmp==string_a) printf("0%d(string)%d:%s",arg->type,arg->val,strConsts[arg->val]);
	else if(tmp==bool_a) printf("0%d(bool)%d:",arg->type,arg->val);//check here if needs true false
	else if(tmp==nil_a) printf("0%d(nil)%d:",arg->type,arg->val);
	else if(tmp==userfunc_a) printf("0%d(userfunc)%d:%s",arg->type,arg->val,arg->name);
	else if(tmp==libfunc_a) printf("0%d(libfunc)%d:%s",arg->type,arg->val,arg->name);
	else if(tmp==retval_a) printf("0%d(retval)%d:",arg->type,arg->val);
	else if(tmp==constdbl_a) printf("0%d(double)%d:%f",arg->type,arg->val,dblConsts[arg->val]);
}

void printTcode(){
	int i=0;
	printf("	######TCODE###### \n");	
	instruction *tmp=instructions;
	while(i<currINST){
		printf("%d: ",i);
		tmp=instructions+i++;
		printf("%s \t",op2StringINST(tmp->opcode));
		if(tmp->result){
			InstructionArqPrint(tmp->result);
		}
		printf("\t");
		if(tmp->arg1){
			InstructionArqPrint(tmp->arg1);
		}
		printf("\t");
		if(tmp->arg2){
			InstructionArqPrint(tmp->arg2);
		}
		printf("\n");
	}
}

void printTables()
{
	int i=0;
	printf("||||INT TABLE||||\n");

	while(i<totalIntConsts){
		printf("%d: %d\n",i, intConsts[i]);
		i++;
	}

	printf("||||DOUBLE TABLE||||\n");

	i=0;
	while(i<totalDblConsts){
		printf("%d: %f\n",i, dblConsts[i]);
		i++;
	}

	printf("||||STRING TABLE||||\n");

	i=0;
	while(i<totalStrConsts){
		printf("%d: %s\n",i ,strConsts[i]);
		i++;
	}

	printf("||||LIBRARY FUNCTION TABLE||||\n");

	i=0;
	while(i<totalNameLibfuncs){
		printf("%d: %s\n",i ,namedLibfuncs[i]);
		i++;
	}

	printf("||||USER FUNCTION TABLE||||\n");

	i=0;
	while(i<totalUserFuncs){
		printf("%d: %s\n",i, userFuncs[i].id);
		i++;
	}
}

void produceFile(){
	int i=0;
	FILE *f = fopen("BinaryCode.lol", "wb");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
	fwrite(&MagicNumber,sizeof(int),1,f);
	//pinakes const
	fwrite(&totalIntConsts,sizeof(int),1,f);
	while(i<totalIntConsts){
		fwrite(&intConsts[i],sizeof(int),1,f);
		i++;
	}
	fwrite(&totalDblConsts,sizeof(int),1,f);
	i=0;
	while(i<totalDblConsts){
		fwrite(&dblConsts[i],sizeof(double),1,f);
		i++;
	}
	fwrite(&totalStrConsts,sizeof(int),1,f);
	i=0;
	while(i<totalStrConsts){
		
		fwrite(&strConsts[i],sizeof(char*),1,f);
		i++;
	}
	fwrite(&totalNameLibfuncs,sizeof(int),1,f);
	i=0;
	while(i<totalNameLibfuncs){
		fwrite(&namedLibfuncs[i],sizeof(char*),1,f);
		i++;
	}	
	fwrite(&totalUserFuncs,sizeof(int),1,f);
	i=0;
	while(i<totalUserFuncs){
		fwrite(&userFuncs[i],sizeof(userfunc),1,f);
		fwrite(&userFuncs[i].id,sizeof(char*),1,f);
		fwrite(&userFuncs[i].address,sizeof(int),1,f);
		fwrite(&userFuncs[i].localSize,sizeof(int),1,f);
		i++;
	}
	fwrite(&currINST, sizeof(int),1,f);
	i=0;
	instruction *tmp=instructions;
	while(i<currINST){
		fwrite(&(tmp->opcode),sizeof(int),1,f);
		if(tmp->result){
			fwrite(&(tmp->result->type),sizeof(int),1,f);
			fwrite(&(tmp->result->val),sizeof(int),1,f);
		}
		if(tmp->arg1){
			fwrite(&(tmp->arg1->type),sizeof(int),1,f);
			fwrite(&(tmp->arg1->val),sizeof(int),1,f);
		}
		if(tmp->arg2){
			fwrite(&(tmp->arg2->type),sizeof(int),1,f);
			fwrite(&(tmp->arg2->val),sizeof(int),1,f);
		}
		tmp=tmp+1;
		i++;
	}
	fclose(f);
}



