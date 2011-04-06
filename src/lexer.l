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

void lexererror();
void lexerinclude(const char*);
void lexerinit(AbstractTokenBuilder*,QString,bool);
void lexerbegin();

AbstractTokenBuilder* tokenizer;
%}

%option yylineno
%option noyywrap
%option nounput
%x include use import
%x comment string
D [0-9]
WS [ \t]
NL \n|\r\n
%%
"include"{WS}*"<"		{ BEGIN(include); tokenizer->buildIncludeStart(); }
<include>{
[^\t\r\n>]+"/"			{ tokenizer->buildIncludePath(lexertext); }
[^\t\r\n>/]+			{ tokenizer->buildIncludeFile(lexertext); }
">"				{ BEGIN(INITIAL); tokenizer->buildIncludeFinish(); }
}
"use"{WS}*"<"			{ BEGIN(use); tokenizer->buildUseStart(); }
<use>[^\t\r\n>]+		{ return tokenizer->buildUse(lexertext); }
<use>">"			{ BEGIN(INITIAL); tokenizer->buildUseFinish(); }
"import"{WS}*"<"		{ BEGIN(import); tokenizer->buildImportStart(); }
<import>[^\t\r\n>]+		{ return tokenizer->buildImport(lexertext); }
<import>">"			{ BEGIN(INITIAL); tokenizer->buildImportFinish(); }
<<EOF>>				{ tokenizer->buildFileFinish();
					lexerpop_buffer_state();
					if(!YY_CURRENT_BUFFER)
						yyterminate(); }
"module"			{ return tokenizer->buildModule(); }
"function"			{ return tokenizer->buildFunction(); }
"true"				{ return tokenizer->buildTrue(); }
"false"				{ return tokenizer->buildFalse(); }
"undef"				{ return tokenizer->buildUndef(); }
"const"				{ return tokenizer->buildConst(); }
"param"				{ return tokenizer->buildParam(); }
"if"				{ return tokenizer->buildIf(); }
"as"				{ return tokenizer->buildAs(); }
"else"				{ return tokenizer->buildElse(); }
"for"				{ return tokenizer->buildFor(); }
"return"			{ return tokenizer->buildReturn(); }
"<="				{ return tokenizer->buildLessEqual();  }
">="				{ return tokenizer->buildGreatEqual();  }
"=="				{ return tokenizer->buildEqual();  }
"!="				{ return tokenizer->buildNotEqual();  }
"&&"				{ return tokenizer->buildAnd(); }
"||"				{ return tokenizer->buildOr();  }
"++"				{ return tokenizer->buildIncrement(); }
"--"				{ return tokenizer->buildDecrement(); }
"**"				{ return tokenizer->buildOuterProduct(); }
".*"				{ return tokenizer->buildComponentwiseMultiply(); }
"./"				{ return tokenizer->buildComponentwiseDivide(); }
"::"				{ return tokenizer->buildNamespace(); }
"="				{ return tokenizer->buildAssign(); }
"+"				{ return tokenizer->buildAdd(); }
"-"				{ return tokenizer->buildSubtract(); }
"?"				{ return tokenizer->buildTernaryCondition(); }
":"				{ return tokenizer->buildTernaryAlternate(); }
"!"				{ return tokenizer->buildNot(); }
"*"				{ return tokenizer->buildMultiply(); }
"/"				{ return tokenizer->buildDivide(); }
"%"				{ return tokenizer->buildModulus(); }
[\[\]<>.;#${}(),^]		{ return tokenizer->buildLegalChar(lexertext[0]); }
{D}+|{D}*\.{D}+|{D}+\.{D}*	{ return tokenizer->buildNumber(lexertext); }
[a-zA-Z0-9_]+			{ return tokenizer->buildIdentifier(lexertext); }
\"				{ BEGIN(string); tokenizer->buildStringStart(); }
<string>{
\\n				{ tokenizer->buildString('\n'); }
\\t				{ tokenizer->buildString('\t'); }
\\r				{ tokenizer->buildString('\r'); }
\\\\				{ tokenizer->buildString('\\'); }
\\\"				{ tokenizer->buildString('"'); }
[^\\\n\"]+			{ tokenizer->buildString(lexertext); }
\"				{ BEGIN(INITIAL); return tokenizer->buildStringFinish(); }
}
"//"[^\n]*\n?			{ tokenizer->buildComment(lexertext); }
"/*"				{ BEGIN(comment); tokenizer->buildCommentStart(); }
<comment>{
[^*]*				{ tokenizer->buildComment(lexertext); }
"*"+[^*/]*			{ tokenizer->buildComment(lexertext); }
"*/"				{ BEGIN(INITIAL); tokenizer->buildCommentFinish(); }
}
{WS}+$				{ tokenizer->buildWhiteSpaceError(); }
{WS}+				{ tokenizer->buildWhiteSpace(); }
{NL}				{ tokenizer->buildNewLine(); }
.				{ return tokenizer->buildIllegalChar();  }
%%

void lexererror()
{
	fprintf(stderr,"%d: illegal token '%s'.\n",lexerlineno,lexertext);
}

void lexerinit(AbstractTokenBuilder* b,QString input,bool file)
{
	tokenizer = b;

	if(file) {
		QFileInfo fileinfo(input);
		char* fullpath = strdup(fileinfo.absoluteFilePath().toLocal8Bit());
		lexerin = fopen(fullpath,"r");
		free(fullpath);
		tokenizer->buildFileStart(fileinfo.absoluteDir());
	} else {
		const char* t = input.toLocal8Bit();
		lexer_scan_string(t);
	}
}

void lexerbegin()
{
	BEGIN(INITIAL);
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
