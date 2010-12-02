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
#include <QStack>
#include <QDir>
#include "parser_yacc.h"
void lexererror();
QString filename;
QStack<QDir> path_stack;
bool haspath;
QString* stringcontents;
void includefile();
void pushstack(QString);
void popstack();
void setfilename(QString);
QDir currentpath();
%}

%option yylineno
%option noyywrap
%option nounput
%x include use path
%x comment string
D [0-9]
WS [ \t\r\n]
%%
"include"{WS}*"<"	{ BEGIN(include); }
<include>{
[^\t\r\n>]+[\\/]	{ BEGIN(path); pushstack(lexertext); }
<path>[^\t\r\n>\\/]+	{ setfilename(lexertext); }
<path>">"		{ BEGIN(INITIAL); includefile(); }
}
"use"{WS}*"<"		{ BEGIN(use); }
<use>[^\t\r\n>]+	{ parserlval.text = new QString(lexertext); return USE; }
<use>">"		{ BEGIN(INITIAL); }
<<EOF>>			{ popstack();
			  lexerpop_buffer_state();
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
\"			{ BEGIN(string); stringcontents = new QString(); }
<string>{
\\n			{ stringcontents->append('\n'); }
\\t			{ stringcontents->append('\t'); }
\\r			{ stringcontents->append('\r'); }
\\\\			{ stringcontents->append('\\'); }
\\\"			{ stringcontents->append('"'); }
[^\\\n\"]+		{ stringcontents->append(lexertext); }
\"			{ BEGIN(INITIAL); parserlval.text = stringcontents; return STRING; }
}
"//"[^\n]*\n?		//ignore one line comments
"/*"			{ BEGIN(comment); }
<comment>{
[^*]*			//ignore anything that's not a '*'
"*"+[^*/]*		//ignore '*'s not followed by '/'s
"*/"			{ BEGIN(INITIAL); }
}
.			{ lexererror();  }
%%

void lexererror()
{
    fprintf(stderr,"%d: illegal token %s",lexerlineno,lexertext);
}

void pushstack(QString p)
{
	haspath=true;
	path_stack.push(QDir(p));
}

void popstack()
{
    if(!path_stack.isEmpty())
	path_stack.pop();
}

QDir currentpath()
{
    QDir curpath;
    if(!path_stack.isEmpty())
	curpath = path_stack.top();

    return curpath;
}

void setfilename(QString file)
{
    if(!haspath)
	path_stack.push(currentpath());
    filename = file;
    haspath=false;
}

void includefile()
{
    QFileInfo finfo(currentpath(),filename);
    const char* fullpath = finfo.absoluteFilePath().toLocal8Bit();

    FILE* newinput=fopen(fullpath,"r");
    if(!newinput)
	fprintf(stderr,"%d: Can't open input file `%s'.\n",lexerlineno,fullpath);
    else
    {
	lexerin=newinput;
	lexerpush_buffer_state(lexer_create_buffer(lexerin, YY_BUF_SIZE));
    }
}
