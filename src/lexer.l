/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010  Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

%{
#include <QString>
#include "parser_yacc.h"
void lexererror();
char* filename;
void startinclude(const char*);
void endinclude();
%}

%option yylineno
%option noyywrap
%option nounput
%x include
%x use
%x comment
D [0-9]
WS [ \t\r\n]
%%
"include"{WS}*"<"	{ BEGIN(include); }
<include>[^\t\r\n>]+	{ startinclude(lexertext); }
<include>">"		{ BEGIN(INITIAL); endinclude(); }
"use"{WS}*"<"		{ BEGIN(use); }
<use>[^\t\r\n>]+	{ parserlval.text = new QString(lexertext); return USE; }
<use>">"		{ BEGIN(INITIAL); }
<<EOF>>			{ lexerpop_buffer_state();
			  if(!YY_CURRENT_BUFFER)
			    yyterminate(); }
"module"		{ return MODULE; }
"function"		{ return FUNCTION; }
"true"			{ return TOK_TRUE; }
"false"			{ return TOK_FALSE; }
"undef"			{ return UNDEF; }
"const"			{ return CONST; }
"param"			{ return PARAM; }
"if"			{ return IF; }
"else"			{ return ELSE; }
"for"			{ return FOR; }
"return"                { return RETURN; }
{WS}+			//ignore whitespace
[.:!;#${}(),=*/%?\+-]	{ return lexertext[0]; }
"<"			{ return '<'; }
"<="			{ return LE;  }
">"			{ return '>'; }
">="			{ return GE;  }
"=="			{ return EQ;  }
"!="			{ return NE;  }
"&&"			{ return AND; }
"||"			{ return OR;  }
"["			{ return '['; }
"]"			{ return ']'; }
{D}+|{D}*\.{D}+|{D}+\.{D}* { parserlval.number = QString(lexertext).toDouble(); return NUMBER; }
[a-zA-Z0-9_]+		{ parserlval.text = new QString(lexertext); return IDENTIFIER; }
\"(\\[tnr\"\\]|[^\\"])*\" { QString str(lexertext+1);
			  str.chop(1);
			  str=str.replace("\\\\","\\");
			  str=str.replace("\\\"","\"");
			  str=str.replace("\\t","\t");
			  str=str.replace("\\n","\n");
			  str=str.replace("\\r","\r");
			  parserlval.text = new QString(str);
			  return STRING; }
"//"[^\n]*\n?		//ignore one line comments
"/*"			{ BEGIN(comment); }
<comment>[^*]*		//ignore anything that's not a '*'
<comment>"*"+[^*/]*	//ignore '*'s not followed by '/'s
<comment>"*/"		{ BEGIN(INITIAL); }
.			{ lexererror();  }
%%

void lexererror()
{
    fprintf(stderr,"%d: illegal token %s",lexerlineno,lexertext);
}

void startinclude(const char* s)
{
    filename = strdup(s);
}

void endinclude()
{
    FILE* newinput=fopen(filename,"r");
    if(!newinput)
	fprintf(stderr,"%d: Can't open input file `%s'.\n",lexerlineno,filename);
    else
    {
	lexerin=newinput;
	lexerpush_buffer_state(lexer_create_buffer( lexerin, YY_BUF_SIZE ));
	BEGIN(INITIAL);
    }
    free(filename);
}
