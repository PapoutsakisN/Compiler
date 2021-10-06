#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define hashTSize 100

// oi typoi simbolon poy kanoyme insert mesa sto ST
typedef enum SymbolTableType {
	GLOBAL, LOCCAL, FORMAL,
	USERFUNC, LIBFUNC
}ST_type;

typedef enum scopespace_t{
	programvar,fuctionlocal,formalarg
}scopespace_t;

//mia lista po krata ta formal arguments twn function
typedef struct ListArguments{
	const char *name;
	struct ListArguments *next;
}ListArguments;

typedef struct ScopeLists{
	int scope;
	struct ST_node *ScopeNode;
	struct ScopeLists *next;
}ScopeLists;

//to struct poy krata kathe simbolo kai tis idiotites toy mesa ston ST 
typedef struct ST_node{
	char *name;
	unsigned int scope;
	unsigned int line;
	ST_type type;
	bool isActive;
	ListArguments *funArgs;//ean den einai user function i den
//exei arguments einai null
	struct ST_node *hashNext;
	struct ST_node *scopeNext;
	//new adds fix
	unsigned int offset;
	unsigned int FunctionAddress;
	unsigned int VariablesNO;
	unsigned int taddress;//here check if using--------------------------------------------
	scopespace_t scopespace;
}ST_node;

typedef struct PatchList{
	int quadNo;
	struct PatchList *next;
}PatchList;

typedef struct NestLoops{
	PatchList *ContinueList;
	PatchList *BreakList;
	struct NestLoops *next;
}NestLoops;


PatchList* add_node(PatchList *head,int quadNo){
	PatchList *tmp=(PatchList*)malloc(sizeof(PatchList));
	tmp->quadNo=quadNo;
	tmp->next=head;
	return tmp;
}

//i hash function i opoia epistrefei to index poy tha prosthesoye to symbol analoga me to onoma toy
int HashFunction(const char *SymbolName){
	unsigned int sum=0;

	while(*SymbolName!='\0'){
	    sum=sum*31+*SymbolName;
            *SymbolName++;	
	}
	return sum%hashTSize;
}

ScopeLists *SearchScope(ScopeLists *head,int scope)
{
	
	ScopeLists *tmp=head;
	while(tmp!=NULL){
		if(tmp->scope==scope) return tmp;
		tmp=tmp->next;	
	}
	return NULL;
}

//eisagei ena neo simbolo ston Hashtable analoga me to index poy epistrefei i hasfunction
ST_node* InsertSymbol(ST_node *HashTable[],ScopeLists *HeadNTail[],const char *name,int scope,int line,ST_type type,int offset,int FunctionAddress,int VariablesNO,scopespace_t scopespace)
{
	ST_node *New_node;
	int location=HashFunction(name);
//dimiourgoume ton neo kombo kai kanoyme arxikopoiisi
	New_node=(ST_node*)malloc(sizeof (ST_node));
	New_node->name=strdup(name);
	New_node->scope=scope;
	New_node->line=line;	
	New_node->type=type;
	New_node->isActive=true;
	New_node->funArgs=NULL;
	New_node->hashNext=NULL;
	New_node->scopeNext=NULL;
	//new adds
	New_node->offset=offset;
	New_node->FunctionAddress=FunctionAddress;
	New_node->VariablesNO=VariablesNO;
	New_node->scopespace=scopespace;
	
	if(HeadNTail[0]==NULL)//proti fora dld
	{
		ScopeLists *scope_node=(ScopeLists*)malloc(sizeof(ScopeLists*));
		scope_node->next=NULL;
		scope_node->scope=scope;
		scope_node->ScopeNode=New_node;
		HeadNTail[0]=HeadNTail[1]=scope_node;
	}else{
		ScopeLists *search=SearchScope(HeadNTail[0],scope);
		if(search==NULL){
			
			ScopeLists *scope_node=(ScopeLists*)malloc(sizeof(ScopeLists*));
			scope_node->scope=scope;
			scope_node->ScopeNode=New_node;
			scope_node->next=NULL;
			HeadNTail[1]->next=scope_node;
			HeadNTail[1]=scope_node;
		}else
		{
			New_node->scopeNext=search->ScopeNode;
			search->ScopeNode=New_node;
		}
	}
//kai to prosthetoyme sto head tis colision list poy mas exei ipodeiksei i hashfunction
	if(HashTable[location]==NULL)
	{
		HashTable[location]=New_node;
	}else
	{
		New_node->hashNext=HashTable[location];
		HashTable[location]=New_node;
	}
	return New_node;
}
//ean to onoma tou simobolou einia kapoio apo ayta simainei oti einai 
//library function
bool isLibFunc(char *name)
{
	if(!strcmp(name,"print")) return true;
	else if(!strcmp(name,"input")) return true;
	else if(!strcmp(name,"objectmemberkeys")) return true;
	else if(!strcmp(name,"objecttotalmembers")) return true;
	else if(!strcmp(name,"objectcopy")) return true;
	else if(!strcmp(name,"totalarguments")) return true;
	else if(!strcmp(name,"argument")) return true;
	else if(!strcmp(name,"typeof")) return true;
	else if(!strcmp(name,"strtonum")) return true;
	else if(!strcmp(name,"sqrt")) return true;
	else if(!strcmp(name,"cos")) return true;
	else if(!strcmp(name,"sin")) return true;
	else return false;
}

void DisableSymbols(ScopeLists *HeadNTail[],int scopeNo)
{//briskoyme to scope list kai to diatrexoyme kai orizoyme olwn twn stoixeiwn to 
// field isActive se false
	ScopeLists *search=SearchScope(HeadNTail[0],scopeNo);
	if(search==NULL) return;
	ST_node *tmp=search->ScopeNode;
	while(tmp!=NULL){
		
		tmp->isActive=false;
		tmp=tmp->scopeNext;
	}
}

//scope-1 giati to scope exei ayksithei eno anoigoyne oi () 
void AddArgs(ScopeLists *HeadNTail[],char *name,int scopeNo)
{//prosthetoyme ta arguments enos function stin lista alla sto tail
	ListArguments *New_node,*tmp;
	ST_node *node;
	New_node=(ListArguments*)malloc(sizeof(ListArguments));
	New_node->name=strdup(name);
	New_node->next=NULL;

	ScopeLists *search=SearchScope(HeadNTail[0],scopeNo-1);
	if(search==NULL) return;
	node=search->ScopeNode;
	tmp=node->funArgs;
	
	if(tmp==NULL)//einai adeia i lista dld 
	{//prosthetoyme to komvo sto head tis listas
		node->funArgs=New_node;
	}else{
		while(tmp->next!=NULL){//prosthetoyme to neo argument sto telos tis listas
			tmp=tmp->next;		
		}
		tmp->next=New_node;
	}
}

//runs the scope list from the scope until currscope-1 and if a userfunctions that is active 
//is found returns true
bool PendingFunction(ScopeLists *HeadNTail[],int scope,int currScope)
{
	int i=scope;
	ST_node *tmp;
	ScopeLists *search;
	while(i<=currScope-1)
	{
		search=SearchScope(HeadNTail[0],i);
		if(search==NULL) return false;
		tmp=search->ScopeNode;
		while(tmp!=NULL){
			if(tmp->isActive&&tmp->type==USERFUNC) return true;
			tmp=tmp->scopeNext;
		}
		i++;
	}
	return false;
}

//prepei na einai kai active to onoma gia na isxyei 
//Searches a name in a specific scope 
ST_node* SearchNameInScope(ST_node *HashTable[],char *name,int scopeNO)
{
	ST_node *search=HashTable[HashFunction(name)];
	while(search!=NULL)
	{//ean einai active kai sto scope pou psaxnoyme kai idio onoma epistrefei true
		if(search->isActive&&search->scope==scopeNO&&!strcmp(name,search->name)) return search;
		search=search->hashNext;
	}
	return NULL;
}

//psaxnei gia ena entry apo to scope poy dinoyme mexri global
ST_node *ReturnNode(ST_node *HashTable[],char *name,int scopeNO)
{//psaxnei diadoxika se ola ta scopes pros ta pano(pros 0 global)
//mexri na brei ena simbolo me to idio onoma kai na einai active
//allios epistrefei null
	ST_node *search;
	int i=scopeNO;
	while(i>=0){
		search=HashTable[HashFunction(name)];
		while(search!=NULL)
		{
			if(search->isActive&&search->scope==i&&!strcmp(name,search->name)) return search;
			search=search->hashNext;
		}
		i--;
	}
	return NULL;;
}
//metatrepei to ST_type se string oste na to ektiposoume stin print hashtable
char *Type2String(ST_type type)
{
	if(type==GLOBAL) return "Global";
	else if(type==LOCCAL) return "Local";
	else if(type==FORMAL) return "Formal";
	else if(type==USERFUNC) return "UserFunction";
	else if(type==LIBFUNC) return "LibraryFunction";
}
//ektiponei ola ta simvola toy hashtable kai ta fields toys me basi
// ta bucket poy briskontai 
void Print_HashTable(ST_node *HashTable[])
{
	printf("	######SYMBOL TABLE ENTRIES######\n");
	
	int i;
	ST_node *tmp=NULL;
	ListArguments *tmp2=NULL;
	for(i=0;i<hashTSize;i++)
	{	
		tmp=HashTable[i];
		if(tmp!=NULL) printf("Table Bucket No: %d\n",i);
		while(tmp!=NULL){				
			printf("\t>Name: %s\n\tType: %s\n\tScope: %d\n\tLine: %d",tmp->name,Type2String(tmp->type),tmp->scope,tmp->line);
			if(strcmp("UserFunction",Type2String(tmp->type))==0||strcmp("LibraryFunction",Type2String(tmp->type))==0){
				if(tmp->funArgs==NULL) printf("\n\twith No Formal Arguments\n");
				else{
					tmp2=tmp->funArgs;
					printf("\n\tArguments: ");
					while(tmp2!=NULL)
					{
						printf("%s ",tmp2->name);
						tmp2=tmp2->next;
					}
					 printf("\n");
				}		
			}else printf("\n");
			tmp=tmp->hashNext;
		}
	}
}	

void ScopeNode(ScopeLists *HeadNTail[])
{
	printf("	######DEBUG MODE ###### \n");
	ScopeLists *tmp=HeadNTail[0];
	ST_node *tmp2;
	while(tmp!=NULL)
	{
		tmp2=tmp->ScopeNode;
		if(tmp2==NULL)break;
		printf("Scope: %d\n",tmp2->scope);
		while(tmp2!=NULL)
		{
			printf("\t>Name: %s\n\tType: %s\n",tmp2->name,Type2String(tmp2->type));
			
			if(strcmp("LibraryFunction",Type2String(tmp2->type))==0) {tmp2=tmp2->scopeNext;continue;}
			printf("\tLine: %d\n\tActive: %d\n",tmp2->line,tmp2->isActive);
			
			if(strcmp("UserFunction",Type2String(tmp2->type))==0){
				printf("\tFunction Address: %d\n",tmp2->FunctionAddress);
			}
			else printf("\tOffset: %d\n",tmp2->offset);
			if(tmp2->scopespace==0)	printf("\tScopeSpace: programvar\n");
			else if(tmp2->scopespace==1) printf("\tScopeSpace: fuctionlocal\n");
			else if(tmp2->scopespace==2) printf("\tScopeSpace: formalarg\n");
			tmp2=tmp2->scopeNext;
		}
		tmp=tmp->next;
	}
}
