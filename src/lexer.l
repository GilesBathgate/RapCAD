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
#include "parameter.h"
#include "parser_yacc.h"
%}
%option noyywrap
D [0-9]
%%
"module"		{ return MODULE; }
"function"		{ return FUNCTION; }
"true"			{ return TOK_TRUE; }
"false"			{ return TOK_FALSE; }
"undef"			{ return UNDEF; }
[ \t\n\r]+		//ignore whitespace
[{}(),=*/%\+-]		{ return yytext[0]; }
"<"			{ return '<'; }
"<="			{ return LE;  }
">"			{ return '>'; }
">="			{ return GE;  }
"=="			{ return EQ;  }
"!="			{ return NE;  }
"&&"			{ return AND; }
"||"			{ return OR;  }
{D}+|{D}*\.{D}+|{D}+\.{D}* { parserlval.number = QString(yytext).toDouble(); return NUMBER; }
"$"?[a-zA-Z0-9_]+	{ parserlval.text = strdup(yytext); return IDENTIFIER; }
.			{ printf("illegal token %s",yytext);  }
%%
