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
#include "tokenbuilder.h"

void lexererror();
void lexerinclude(const char*);
void lexerinit(QDir sourcepath);

TokenBuilder* tbuilder;
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
[^\t\r\n>]+"/"		{ tbuilder->BuildIncludePath(lexertext); }
[^\t\r\n>/]+		{ tbuilder->BuildIncludeFile(lexertext); }
">"			{ BEGIN(INITIAL); tbuilder->BuildIncludeFinish(); }
}
"use"{WS}*"<"		{ BEGIN(use); }
<use>[^\t\r\n>]+	{ return tbuilder->BuildUse(lexertext); }
<use>">"		{ BEGIN(INITIAL); }
<<EOF>>			{ tbuilder->BuildFileFinish();
			  lexerpop_buffer_state();
			  if(!YY_CURRENT_BUFFER)
			    yyterminate(); }
"module"		{ return tbuilder->BuildModule(); }
"function"		{ return tbuilder->BuildFunction(); }
"true"			{ return tbuilder->BuildTrue(); }
"false"		{ return tbuilder->BuildFalse(); }
"undef"		{ return tbuilder->BuildUndef(); }
"const"		{ return tbuilder->BuildConst(); }
"param"		{ return tbuilder->BuildParam(); }
"if"			{ return tbuilder->BuildIf(); }
"else"			{ return tbuilder->BuildElse(); }
"for"			{ return tbuilder->BuildFor(); }
"return"		{ return tbuilder->BuildReturn(); }
{WS}+			//ignore whitespace
[\[\]<>.:!;#${}(),=*/%?\+\-] %{
			{ return lexertext[0]; }
			%}
"<="			{ return tbuilder->BuildLessEqual();  }
">="			{ return tbuilder->BuildGreatEqual();  }
"=="			{ return tbuilder->BuildEqual();  }
"!="			{ return tbuilder->BuildNotEqual();  }
"&&"			{ return tbuilder->BuildAnd(); }
"||"			{ return tbuilder->BuildOr();  }
{D}+|{D}*\.{D}+|{D}+\.{D}* { return tbuilder->BuildNumber(lexertext); }
[a-zA-Z0-9_]+		{ return tbuilder->BuildIdentifier(lexertext); }
\"			{ BEGIN(string); tbuilder->BuildStringStart(); }
<string>{
\\n			{ tbuilder->BuildString('\n'); }
\\t			{ tbuilder->BuildString('\t'); }
\\r			{ tbuilder->BuildString('\r'); }
\\\\			{ tbuilder->BuildString('\\'); }
\\\"			{ tbuilder->BuildString('"'); }
[^\\\n\"]+		{ tbuilder->BuildString(lexertext); }
\"			{ BEGIN(INITIAL); return tbuilder->BuildStringFinish(); }
}
"//"[^\n]*\n?		{ tbuilder->BuildComment(lexertext); }
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

void lexerinit(QDir sourcepath)
{
    tbuilder = new TokenBuilder();
    tbuilder->BuildFileStart(sourcepath);
}

void lexerinclude(const char* fullpath)
{
    FILE* newinput=fopen(fullpath,"r");
    if(!newinput) {
	fprintf(stderr,"%d: Can't open input file `%s'.\n",lexerlineno,fullpath);
	return;
    }

    lexerin=newinput;
    lexerpush_buffer_state(lexer_create_buffer(lexerin, YY_BUF_SIZE));
}
