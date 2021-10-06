%{
/*Declarations*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

struct alpha_token_t{
	unsigned int lineno;
	unsigned int tokenno;
	char *type;
	char *content;
	struct alpha_token_t *next;
};

int tokenno=0,lineno=0;
int comment=1,unclosedC,str_error;
int str_length=-1;
size_t size=0;
char* char_array;

struct alpha_token_t *head=NULL;

void AddToken(int lineno,int tokenno,char *value,char type)
{
	struct alpha_token_t *newNode,*tmp=head;

	newNode=(struct alpha_token_t *)malloc(sizeof(struct alpha_token_t));
	newNode->lineno=lineno;
	newNode->tokenno=tokenno;
	switch(type){
		case 'K':
				newNode->type="KEYWORD";
				break;
		case 'P':
				newNode->type="PUNCTUATION";
				break; 
		case 'O':
				newNode->type="OPERATOR";
				break;
		case 'S': 
				newNode->type="STRING";
				break;
		case 'N':  
				newNode->type="INTCONST";
				break;
		case 'I':
				newNode->type="IDENTIFIER";
				break;  
		case 'C':
				newNode->type="LINECOMMENT";
				break;
		case 'M':
				newNode->type="MLINECOMMENT";
				break; 
		case 'F':
				newNode->type="NESTED_MLINECOMMENT";
				break;
		case 'R':
				newNode->type="REALCONST";
				break;
		default:
				newNode->type="UNDEFINED";
	}
	newNode->content=strdup(value);
	newNode->next=NULL;
	if(head==NULL)
	{
		head=newNode;
	}else
	{
		while(tmp->next!=NULL)
		{
			tmp=tmp->next;
		}
		tmp->next=newNode;
	}
}

void PrintTokenList()
{
	struct alpha_token_t *tmp=head;
	while(tmp!=NULL)
	{
		printf("%d: #%d \"%s\" \t\t %s\n",tmp->lineno,tmp->tokenno,tmp->content,tmp->type);
		tmp = tmp->next;
	}
}

void AddChar(char input)
{
	str_length++;
	size=str_length*sizeof(char)+sizeof(char);
	char_array=(char*)realloc(char_array,size);
	char_array[str_length]=input; 
}

%}

/*Flex options*/
%option noyywrap
%option yylineno

/*Flex macros*/

/*KeyWords*/
if "if"
else "else"
while "while"
for	"for"
function "function"
return "return"
break "break"
continue "continue"
and "and"
not "not"
or "or"
local "local"
true "true"
false "false"
nil "nil"

/*Operators*/
assign "="
plus "+"
minus "-"
multi "*"
div "/"
mod "%"
equal "=="
notequal "!="
incr "++"
decr "--"
less "<"
great ">"
greatequal ">="
lessequal "<="

/*Punctuation*/
lpar "("
rpar ")"
lbracket "["
rbracket "]"
langle "{"
rangle "}"
semicolon ";"
comma ","
colon ":"
dcolon "::"
dot "."
ddot ".."

/*Others*/
id [a-zA-Z][a-zA-Z_0-9]*
int [0-9]+
real [0-9]+\.[0-9]+
linecomment "//".*
blankspace [ \t\v]
newline "\n"
unknown .

%x COMMENT
%x STRING ESCAPE

%%
<<EOF>>			{	if(comment>=0&&unclosedC==1) {
					fprintf(stderr,"Lexical Error: Unclosed MlineComment\n");
					exit(0);
				}
				if(str_error){
				 	fprintf(stderr,"Lexical Error: Unclosed String Quotes\n");
					exit(0);
				}
				return 0;
			}
"/*" {comment = 1,unclosedC=1;  BEGIN(COMMENT);}
<COMMENT>"*"+[^*/]*
<COMMENT>"/"+"*" {unclosedC=1;comment++;}
<COMMENT>"*"+"/" {
					
					if(comment==1){
						AddToken(yylineno,++tokenno,"/**/",'M');
						unclosedC=0;
						BEGIN(INITIAL);
					}
					else{
						AddToken(yylineno,++tokenno,"/**/",'F');
						comment--;
					}
}
<COMMENT>.
<COMMENT>\n


\" {char_array=(char*)malloc(sizeof(char));str_error=1;BEGIN(STRING);
}
<STRING>"\\" {BEGIN(ESCAPE);}
<STRING>\n
<STRING>\" {
			AddChar('\0');
		   	AddToken(yylineno,++tokenno,char_array,'S');
			yylval.strValue=strdup(char_array);
			free(char_array);
			str_length=-1;
			str_error=0;
			BEGIN(INITIAL);
			return YYSTRING;
			
}
<STRING>. {	AddChar((char)yytext[0]);}

<ESCAPE>"t" {AddChar('\t'); 
			BEGIN(STRING);
}
<ESCAPE>"n" {AddChar('\n'); 
			BEGIN(STRING);
}
<ESCAPE>"\\" {AddChar('\\');
			BEGIN(STRING);
}
<ESCAPE>\" {AddChar('\"'); 
			BEGIN(STRING);
}

<ESCAPE>.	{fprintf(stderr,"Lexical Error:Unknown escape character in line: %d\n", yylineno); exit(0);}

{if} {AddToken(yylineno,++tokenno,"if",'K');return IF;}
{else} {AddToken(yylineno,++tokenno,"else",'K'); return ELSE;}
{while} {AddToken(yylineno,++tokenno,"while",'K');return WHILE;}
{for} {AddToken(yylineno,++tokenno,"for",'K');return FOR;}
{function} {AddToken(yylineno,++tokenno,"function",'K');return FUNCTION;}
{return} {AddToken(yylineno,++tokenno,"rreturn",'K');return RETURN;}
{break} {AddToken(yylineno,++tokenno,"break",'K');return BREAK;}
{continue} {AddToken(yylineno,++tokenno,"continue",'K');return CONTINUE;}
{and} {AddToken(yylineno,++tokenno,"and",'K');return AND;}
{not} {AddToken(yylineno,++tokenno,"not",'K');return NOT;}
{or} {AddToken(yylineno,++tokenno,"or",'K');return OR;}
{local} {AddToken(yylineno,++tokenno,"local",'K');return LOCAL;}
{true} {AddToken(yylineno,++tokenno,"true",'K');return TRUE;}
{false} {AddToken(yylineno,++tokenno,"false",'K');return FALSE;}
{nil} {AddToken(yylineno,++tokenno,"nil",'K');return NIL;}

{assign} {AddToken(yylineno,++tokenno,"=",'O');return ASSIGN;}
{plus} {AddToken(yylineno,++tokenno,"+",'O');return PLUS;}
{minus} {AddToken(yylineno,++tokenno,"-",'O');return MINUS;}
{multi} {AddToken(yylineno,++tokenno,"*",'O');return MULTI;}
{div} {AddToken(yylineno,++tokenno,"/",'O');return DIV;}
{mod} {AddToken(yylineno,++tokenno,"%",'O');return MOD;}
{equal} {AddToken(yylineno,++tokenno,"==",'O');return EQUAL;}
{notequal} {AddToken(yylineno,++tokenno,"!=",'O');return NOTEQUAL;}
{incr} {AddToken(yylineno,++tokenno,"++",'O');return INCR;}
{decr} {AddToken(yylineno,++tokenno,"--",'O');return DECR;}
{less} {AddToken(yylineno,++tokenno,"<",'O');return LESS;}
{great} {AddToken(yylineno,++tokenno,">",'O');return GREAT;}
{greatequal} {AddToken(yylineno,++tokenno,">=",'O');return GREATEQUAL;}
{lessequal} {AddToken(yylineno,++tokenno,"<=",'O');return LESSEQUAL;}

{lpar} {AddToken(yylineno,++tokenno,"(",'P');return LPAR;}
{rpar} {AddToken(yylineno,++tokenno,")",'P');return RPAR;}
{lbracket} {AddToken(yylineno,++tokenno,"[",'P');return LBRACKET;}
{rbracket} {AddToken(yylineno,++tokenno,"]",'P');return RBRACKET;}
{langle} {AddToken(yylineno,++tokenno,"{",'P');return LANGLE;}
{rangle} {AddToken(yylineno,++tokenno,"}",'P');return RANGLE;}
{semicolon} {AddToken(yylineno,++tokenno,";",'P');return SEMICOLON;}
{comma} {AddToken(yylineno,++tokenno,",",'P');return COMMA;}
{colon} {AddToken(yylineno,++tokenno,":",'P');return COLON;}
{dcolon} {AddToken(yylineno,++tokenno,"::",'P');return DCOLON;}
{dot} {AddToken(yylineno,++tokenno,".",'P');return DOT;}
{ddot} {AddToken(yylineno,++tokenno,"..",'P');return DDOT;}

{id} {AddToken(yylineno,++tokenno,yytext,'I');
		yylval.strValue=strdup(yytext);
		return ID;}
{int}  {AddToken(yylineno,++tokenno,yytext,'N');
		yylval.intValue=atoi(yytext);	
		return INT;}
{real} {AddToken(yylineno,++tokenno,yytext,'R');
		yylval.realValue=atof(yytext);
		return REAL;}

{linecomment} {AddToken(yylineno,++tokenno,"//",'C');}

{blankspace}  {}
{newline} {}
{unknown}  {AddToken(yylineno,++tokenno,yytext,'U');
		fprintf(stderr,"Lexical Error:Unknown character used in line: %d\n",yylineno);
		exit(0);
		}
