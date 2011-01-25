/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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
#include "abstracttokenbuilder.h"
#include "tokenbuilder.h"

void lexererror();
void lexerinclude(const char*);
void lexerinit(QString path);

AbstractTokenBuilder* tbuilder;
%}

%option yylineno
%option noyywrap
%option nounput
%x include use import
%x comment string
D [0-9]
WS [ \t\r\n]
%%
"include"{WS}*"<"	{ BEGIN(include); tbuilder->buildIncludeStart(); }
<include>{
[^\t\r\n>]+"/"		{ tbuilder->buildIncludePath(lexertext); }
[^\t\r\n>/]+		{ tbuilder->buildIncludeFile(lexertext); }
">"			{ BEGIN(INITIAL); tbuilder->buildIncludeFinish(); }
}
"use"{WS}*"<"		{ BEGIN(use); tbuilder->buildUseStart(); }
<use>[^\t\r\n>]+	{ return tbuilder->buildUse(lexertext); }
<use>">"		{ BEGIN(INITIAL); tbuilder->buildUseFinish(); }
"import"{WS}*"<"	{ BEGIN(import); tbuilder->buildImportStart(); }
<import>[^\t\r\n>]+	{ return tbuilder->buildImport(lexertext); }
<import>">"		{ BEGIN(INITIAL); tbuilder->buildImportFinish(); }
<<EOF>>			{ tbuilder->buildFileFinish();
			  lexerpop_buffer_state();
			  if(!YY_CURRENT_BUFFER)
			    yyterminate(); }
"module"		{ return tbuilder->buildModule(); }
"function"		{ return tbuilder->buildFunction(); }
"true"			{ return tbuilder->buildTrue(); }
"false"		{ return tbuilder->buildFalse(); }
"undef"		{ return tbuilder->buildUndef(); }
"const"		{ return tbuilder->buildConst(); }
"param"		{ return tbuilder->buildParam(); }
"if"			{ return tbuilder->buildIf(); }
"as"			{ return tbuilder->buildAs(); }
"else"			{ return tbuilder->buildElse(); }
"for"			{ return tbuilder->buildFor(); }
"return"		{ return tbuilder->buildReturn(); }
"<="			{ return tbuilder->buildLessEqual();  }
">="			{ return tbuilder->buildGreatEqual();  }
"=="			{ return tbuilder->buildEqual();  }
"!="			{ return tbuilder->buildNotEqual();  }
"&&"			{ return tbuilder->buildAnd(); }
"||"			{ return tbuilder->buildOr();  }
"**"			{ return tbuilder->buildOuterProduct(); }
".*"			{ return tbuilder->buildComponentwiseMultiply(); }
"./"			{ return tbuilder->buildComponentwiseDivide(); }
"::"			{ return tbuilder->buildNamespace(); }
[\[\]<>.:!;#${}(),=*/%?\+\-] %{
			{ return tbuilder->buildLegalChar(lexertext[0]); }
			%}
{D}+|{D}*\.{D}+|{D}+\.{D}* { return tbuilder->buildNumber(lexertext); }
[a-zA-Z0-9_]+		{ return tbuilder->buildIdentifier(lexertext); }
\"			{ BEGIN(string); tbuilder->buildStringStart(); }
<string>{
\\n			{ tbuilder->buildString('\n'); }
\\t			{ tbuilder->buildString('\t'); }
\\r			{ tbuilder->buildString('\r'); }
\\\\			{ tbuilder->buildString('\\'); }
\\\"			{ tbuilder->buildString('"'); }
[^\\\n\"]+		{ tbuilder->buildString(lexertext); }
\"			{ BEGIN(INITIAL); return tbuilder->buildStringFinish(); }
}
"//"[^\n]*\n?		{ tbuilder->buildComment(lexertext); }
"/*"			{ BEGIN(comment); tbuilder->buildCommentStart(); }
<comment>{
[^*]*			{ tbuilder->buildComment(lexertext); }
"*"+[^*/]*		{ tbuilder->buildComment(lexertext); }
"*/"			{ BEGIN(INITIAL); tbuilder->buildCommentFinish(); }
}
[ \t]+\n		{ tbuilder->buildWhiteSpaceError(); }
{WS}+			//ignore whitespace
.			{ lexererror();  }
%%

void lexererror()
{
    fprintf(stderr,"%d: illegal token `%s'.\n",lexerlineno,lexertext);
}

void lexerinit(QString path)
{
    QFileInfo fileinfo(path);
    const char* fullpath = fileinfo.absoluteFilePath().toLocal8Bit();
    lexerin = fopen(fullpath,"r");
    tbuilder = new TokenBuilder();
    tbuilder->buildFileStart(fileinfo.absoluteDir());
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
