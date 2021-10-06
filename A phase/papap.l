%{
  #define YY_DECL int alpha_yylex (void* yylval)
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  int token=0;
  int line=0;

struct alpha_token_t{
     int lineno;
	 int tokenno;
	 char* type;
	 char* value;
	 struct alpha_token_t *next;
};
struct alpha_token_t *head=NULL;


struct alpha_token_t *AddKeyword(struct alpha_token_t *head, int line, int tokenno,char* value)
{
     struct alpha_token_t *h=head, *newkey;
	 newkey = (struct alpha_token_t *) malloc ( sizeof(struct alpha_token_t) );
	 newkey->lineno=line;
	 newkey->tokenno=tokenno;
	 newkey->type="KEYWORD";
	 newkey->value=strdup(value);
	 newkey->next=NULL;
	 if(h==NULL){
		return newkey;
	 }
	 while (h->next != NULL)
     {
           h = h->next;
     }
	 h->next=newkey;
	 return head;
}

struct alpha_token_t *AddOperator(struct alpha_token_t *head, int line, int tokenno,char* value)
{
     struct alpha_token_t *h=head, *newop;
	 newop = (struct alpha_token_t *) malloc ( sizeof(struct alpha_token_t) );
	 newop->lineno=line;
	 newop->tokenno=tokenno;
	 newop->type="OPERATOR";
	 newop->value=strdup(value);
	 newop->next=NULL;
	 if(h==NULL){
		return newop;
	 }
	 while (h->next != NULL)
     {
           h = h->next;
     }
	 h->next=newop;
	 return head;
}

struct alpha_token_t *AddNum(struct alpha_token_t *head, int line, int tokenno,char* value)
{
     struct alpha_token_t *h=head, *newnum;
	 newnum = (struct alpha_token_t *) malloc ( sizeof(struct alpha_token_t) );
	 newnum->lineno=line;
	 newnum->tokenno=tokenno;
	 newnum->type="NUMBER";
	 newnum->value=strdup(value);
	 newnum->next=NULL;
	 if(h==NULL){
		return newnum;
	 }
	 while (h->next != NULL)
     {
           h = h->next;
     }
	 h->next=newnum;
	 return head;
}

struct alpha_token_t *AddStiksi(struct alpha_token_t *head, int line, int tokenno,char* value)
{
     struct alpha_token_t *h=head, *newstik;
	 newstik = (struct alpha_token_t *) malloc ( sizeof(struct alpha_token_t) );
	 newstik->lineno=line;
	 newstik->tokenno=tokenno;
	 newstik->type="SHMEIA_STIKSHS";
	 newstik->value=strdup(value);
	 newstik->next=NULL;
	 if(h==NULL){
		return newstik;
	 }
	 while (h->next != NULL)
     {
           h = h->next;
     }
	 h->next=newstik;
	 return head;
}

struct alpha_token_t *AddString(struct alpha_token_t *head, int line, int tokenno,char* value)
{
     struct alpha_token_t *h=head, *newstr;
	 newstr = (struct alpha_token_t *) malloc ( sizeof(struct alpha_token_t) );
	 newstr->lineno=line;
	 newstr->tokenno=tokenno;
	 newstr->type="STRING";
	 newstr->value=strdup(value);
	 newstr->next=NULL;
	 if(h==NULL){
		return newstr;
	 }
	 while (h->next != NULL)
     {
           h = h->next;
     }
	 h->next=newstr;
	 return head;
}

struct alpha_token_t *AddID(struct alpha_token_t *head, int line, int tokenno,char* value)
{
     struct alpha_token_t *h=head, *newid;
	 newid = (struct alpha_token_t *) malloc ( sizeof(struct alpha_token_t) );
	 newid->lineno=line;
	 newid->tokenno=tokenno;
	 newid->type="IDENTIFIER";
	 newid->value=strdup(value);
	 newid->next=NULL;
	 if(h==NULL){
		return newid;
	 }
	 while (h->next != NULL)
     {
           h = h->next;
     }
	 h->next=newid;
	 return head;
}

struct alpha_token_t *AddNl(struct alpha_token_t *head, int line, int tokenno,char* value)
{
     struct alpha_token_t *h=head, *newnl;
	 newnl = (struct alpha_token_t *) malloc ( sizeof(struct alpha_token_t) );
	 newnl->lineno=line;
	 newnl->tokenno=tokenno;
	 newnl->type="NEW_LINE";
	 newnl->value=strdup(value);
	 newnl->next=NULL;
	 if(h==NULL){
		return newnl;
	 }
	 while (h->next != NULL)
     {
           h = h->next;
     }
	 h->next=newnl;
	 return head;
}

struct alpha_token_t *AddBs(struct alpha_token_t *head, int line, int tokenno,char* value)
{
     struct alpha_token_t *h=head, *newbs;
	 newbs = (struct alpha_token_t *) malloc ( sizeof(struct alpha_token_t) );
	 newbs->lineno=line;
	 newbs->tokenno=tokenno;
	 newbs->type="BLANK_SPACE";
	 newbs->value=strdup(value);
	 newbs->next=NULL;
	 if(h==NULL){
		return newbs;
	 }
	 while (h->next != NULL)
     {
           h = h->next;
     }
	 h->next=newbs;
	 return head;
}

struct alpha_token_t *AddUn(struct alpha_token_t *head, int line, int tokenno,char* value)
{
     struct alpha_token_t *h=head, *newun;
	 newun = (struct alpha_token_t *) malloc ( sizeof(struct alpha_token_t) );
	 newun->lineno=line;
	 newun->tokenno=tokenno;
	 newun->type="UNDEFINED";
	 newun->value=strdup(value);
	 newun->next=NULL;
	 if(h==NULL){
		return newun;
	 }
	 while (h->next != NULL)
     {
           h = h->next;
     }
	 h->next=newun;
	 return head;
}

void PrintTokens(struct alpha_token_t *head)
{
     struct alpha_token_t *tmp=head;
     while (tmp != NULL)
     {
      
	    if(!strcmp(tmp->value," ")){
	      printf("%3d: #%3d \t\t\t\t %s\n",tmp->lineno,tmp->tokenno,tmp->type);
	    }
	    else{
		printf("%3d: #%3d [\"%s\"] \t\t %s\n",tmp->lineno,tmp->tokenno,tmp->value,tmp->type);
	    }
	   tmp = tmp->next;
     }
     printf("\n");
}
    

%}
/*Flex options*/
%option noyywrap
%option yylineno

/*Flex macros*/
if		"if"
else		"else"
while		"while"
for		"for"
function	"function"
return		"return"
break		"break"
continue	"continue"
and		"and"
not		"not"
or		"or"
local		"local"
true		"true"
false		"false"
nil		"nil"
id		[a-zA-Z][a-zA-Z_0-9]*
ekx		"="
plus		"+"
plin		"-"
epi		"*"
dia		"/"
mod		"%"
ison		"=="
diaforo		"!="
pplus		"++"
pplin		"--"
meg		">"
mik		"<"
megison		">="
mikison		"<="
int		[0-9]+
real		[0-9]+\.[0-9]+
string		\"[A-Za-z0-9]*\"
aangistro	\{
dangistro	\}
aangili		\[
dangili		\]
apar		\(
dpar		\)
erwt		\;
koma		\,
aktel		\:
tel		\.
ttel		".."
aktels		"::"
comment1	"//".*
comment2	"/*".*"*/"
keno   		[\t' '\r]*
enter		"\n"
allo		.
%x COMMENT INCLUDE

%%

[\t]+
{if} {head=AddKeyword(head, yylineno,++token,"if");} 
{else} {head=AddKeyword(head, yylineno,++token,"else");}		
{while} {head=AddKeyword(head, yylineno,++token,"while");}
{for} {head=AddKeyword(head, yylineno,++token,"for");}
{function} {head=AddKeyword(head, yylineno,++token,"function");}
{return} {head=AddKeyword(head, yylineno,++token,"return");}
{break} {head=AddKeyword(head, yylineno,++token,"break");}
{continue} {head=AddKeyword(head, yylineno,++token,"continue");}
{and} {head=AddKeyword(head, yylineno,++token,"and");}
{not} {head=AddKeyword(head, yylineno,++token,"not");}
{or} {head=AddKeyword(head, yylineno,++token,"or");}
{local} {head=AddKeyword(head, yylineno,++token,"local");}
{true} {head=AddKeyword(head, yylineno,++token,"true");}
{false} {head=AddKeyword(head, yylineno,++token,"false");}
{nil} {head=AddKeyword(head, yylineno,++token,"nil");}
{id} {head=AddID(head, yylineno,++token,"id");}
{ekx} {head=AddOperator(head, yylineno,++token,"=");}
{plus} {head=AddOperator(head, yylineno,++token,"+");}
{plin} {head=AddOperator(head, yylineno,++token,"-");}
{epi} {head=AddOperator(head, yylineno,++token,"*");}
{dia} {head=AddOperator(head, yylineno,++token,"/");}
{mod} {head=AddOperator(head, yylineno,++token,"%");}
{ison} {head=AddOperator(head, yylineno,++token,"==");}
{diaforo} {head=AddOperator(head, yylineno,++token,"!=");}
{pplus} {head=AddOperator(head, yylineno,++token,"++");}
{pplin} {head=AddOperator(head, yylineno,++token,"--");}
{meg} {head=AddOperator(head, yylineno,++token,">");}
{mik} {head=AddOperator(head, yylineno,++token,"<");}
{megison} {head=AddOperator(head, yylineno,++token,">=");}
{mikison}	{head=AddOperator(head, yylineno,++token,"<=");}
{int} {head=AddNum(head, yylineno,++token,"int");}
{real} {head=AddNum(head, yylineno,++token,"real");}
{string} {head=AddString(head, yylineno,++token,"=");}
{aangistro} {head=AddStiksi(head, yylineno,++token,"{");}
{dangistro} {head=AddStiksi(head, yylineno,++token,"}");}
{aangili}	{head=AddStiksi(head, yylineno,++token,"[");}
{dangili}	{head=AddStiksi(head, yylineno,++token,"]");}
{apar} {head=AddStiksi(head, yylineno,++token,"(");}
{dpar} {head=AddStiksi(head, yylineno,++token,")");}
{erwt} {head=AddStiksi(head, yylineno,++token,";");}
{koma} {head=AddStiksi(head, yylineno,++token,",");}
{aktel} {head=AddStiksi(head, yylineno,++token,":");}
{aktels} {head=AddStiksi(head, yylineno,++token,"::");}
{tel} {head=AddStiksi(head, yylineno,++token,".");}
{ttel} {head=AddStiksi(head, yylineno,++token,"..");}
{comment1} {head=AddStiksi(head, yylineno,++token,"//");}
{comment2} {head=AddStiksi(head, yylineno,++token,"/*");}
{keno} {head=AddBs(head, yylineno,++token,"\t  ");}
{enter} {head=AddNl(head, yylineno,++token,"\n");}
{allo} {head=AddUn(head, yylineno,++token,"Undefined");}


%%

int main(int argc,char** argv)
{
	if(argc>1){
		if(!(yyin=fopen(argv[1],"r"))){
			fprintf(stderr,"Cannot read file: %s\n",argv[1]);
			return 1;
		}
	}
	else
		yyin=stdin;

	alpha_yylex(NULL);
		PrintTokens(head);		

	return 0;
}
