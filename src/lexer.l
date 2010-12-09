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
void setsourcepath(QDir);
void includefile();

QString* stringcontents;
QString filename;
QString filepath;
QStack<QDir> path_stack;

%}

%option yylineno
%option noyywrap
%option nounput
%x include use
%x comment string
D [0-9]
WS [ \t\r\n]
%%
"include"{WS}*"<"	{ BEGIN(include); }
<include>{
[^\t\r\n>]+"/"		{ filepath = lexertext; }
[^\t\r\n>/]+		{ filename = lexertext; }
">"			{ BEGIN(INITIAL); includefile(); }
}
"use"{WS}*"<"		{ BEGIN(use); }
<use>[^\t\r\n>]+	{ parserlval.text = new QString(lexertext); return USE; }
<use>">"		{ BEGIN(INITIAL); }
<<EOF>>			{ path_stack.pop();
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
[\[\]<>.:!;#${}(),=*/%?\+\-] %{
			{ return lexertext[0]; }
			%}
"<="			{ return LE;  }
">="			{ return GE;  }
"=="			{ return EQ;  }
"!="			{ return NE;  }
"&&"			{ return AND; }
"||"			{ return OR;  }
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

void setsourcepath(QDir pth)
{
    path_stack.push(pth);
}

void includefile()
{
    if(filename.isEmpty())
	return;

    QDir currentpath = path_stack.top();
    if(filepath.isEmpty()) {
	path_stack.push(currentpath);
    } else {
	QFileInfo dirinfo(currentpath,filepath);
	path_stack.push(dirinfo.dir());
	filepath.clear();
    }

    currentpath = path_stack.top();
    QFileInfo fileinfo(currentpath,filename);
    filename.clear();

    const char* fullpath = fileinfo.absoluteFilePath().toLocal8Bit();
    FILE* newinput=fopen(fullpath,"r");
    if(!newinput) {
	fprintf(stderr,"%d: Can't open input file `%s'.\n",lexerlineno,fullpath);
	return;
    }

    lexerin=newinput;
    lexerpush_buffer_state(lexer_create_buffer(lexerin, YY_BUF_SIZE));

}
