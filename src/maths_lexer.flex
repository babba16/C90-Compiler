%option noyywrap

%{
// Avoid error "error: `fileno' was not declared in this scope"
extern "C" int fileno(FILE *stream);
#include <iostream>
#include "maths_parser.tab.hpp"

void debug();
bool DEBUG = true;
%}

%%


"int"			{ debug(); yylval.string=new std::string("int"); return T_INT; }
"void"			{ debug(); yylval.string=new std::string("void"); return T_VOID; }
"return"		{ debug(); yylval.string = new std::string("return"); return T_RETURN; }
"if"			{ debug(); yylval.string = new std::string("if"); return T_IF; }
"else"			{ debug(); yylval.string = new std::string("else"); return T_ELSE; }
"while"			{ debug(); yylval.string = new std::string("while"); return T_WHILE; }
"for"			{ debug(); yylval.string = new std::string("for"); return T_FOR; }
"do"			{ debug(); yylval.string = new std::string("do"); return T_DO; }
"break"			{ debug(); yylval.string = new std::string("break"); return T_BREAK; }
"continue"		{ debug(); yylval.string = new std::string("continue"); return T_CONTINUE; }
"go to"			{ debug(); yylval.string = new std::string("continue"); return T_GO_TO; }

[*]             { debug(); yylval.string = new std::string("*"); return T_TIMES; }
[-]             {  debug(); yylval.string = new std::string("-"); return T_MINUS; }
[/]             {  debug(); return T_DIVIDE; }
[+]             {  debug(); return T_PLUS; }
[=]				{  debug(); return T_EQUAL; }

[(]             {  debug(); return T_LBRACKET; }
[)]             {  debug(); return T_RBRACKET; }

[{]             {  debug(); return T_LCBRACKET; }
[}]             {  debug(); return T_RCBRACKET; }

[\[]			{  debug(); return T_LSBRACKET; }
[\]]			{  debug(); return T_RSBRACKET; }

[,]				{  debug(); return T_COMM; }
[;]				{  debug(); return T_SEMI_COLON; }
[:]				{  debug(); return T_COLON; }

[<]				{  debug(); yylval.string = new std::string("<="); return T_LT; }
[>]				{  debug(); yylval.string = new std::string(">="); return T_GT; }
[<][=]			{  debug(); yylval.string = new std::string("<="); return T_LE; }
[>][=]			{  debug(); yylval.string = new std::string(">="); return T_GE; }
[=][=]			{  debug(); yylval.string = new std::string("=="); return T_EQ; }
[!][=]			{  debug(); yylval.string = new std::string("!="); return T_NEQ; }

[!]				{  debug(); return T_LEXCLAIM; }
[&][&]			{  debug(); return T_LAND; }
[|][|]			{  debug(); return T_LOR; }
[<][<]			{  debug(); return T_LSHIFT; }
[>][>]			{  debug(); return T_RSHIFT; }

[&]				{  debug(); return T_AND; }
[|]				{  debug(); return T_OR; }
[%]				{  debug(); return T_REM; }
[\^]				{  debug(); return T_XOR; }

[~]				{ debug(); return T_NOT; }

[-]?[0-9]+		{ debug(); yylval.number=strtod(yytext, 0); return I_INT; }



[-]?[0-9]+([.][0-9]*)? { debug(); yylval.number=strtod(yytext, 0); return I_FLOAT;}

log             { return T_LOG;   }
exp             { return T_EXP; }
sqrt            { return T_SQRT; }

[a-z]+[a-z0-9]*          {  debug(); yylval.string=new std::string(yytext); return T_VARIABLE; }

[ \t\r\n]+		{ debug();}

.               { fprintf(stderr, "Invalid token\n"); exit(1); }
%%

void yyerror (char const *s)
{
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}

void debug() {
	if(DEBUG) {
		std::cout << yytext;
	}
	return;
}