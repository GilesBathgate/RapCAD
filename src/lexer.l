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
%}

%option yylineno
%option noyywrap
%option nounput
%x comment
D [0-9]

%%
"module"		{ return MODULE; }
"function"		{ return FUNCTION; }
"true"			{ return TOK_TRUE; }
"false"			{ return TOK_FALSE; }
"undef"			{ return UNDEF; }
"const"			{ return CONST; }
"param"			{ return PARAM; }
[ \t\n\r]+		//ignore whitespace
[${}(),=*/%\+-]		{ return lexertext[0]; }
";"			{ return ';'; }
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
<comment>"*"+[^/]*	//ignore '*'s not followed by '/'s
<comment>"*/"		{ BEGIN(INITIAL); }
.			{ lexererror();  }
%%

void lexererror()
{
    fprintf(stderr,"%d: illegal token %s",lexerlineno,lexertext);
}
