/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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
#include "reporter.h"
#include "decimal.h"
#include "unistd.h"

void lexererror();
void lexerinclude(QFileInfo);
void lexerinit(AbstractTokenBuilder*,Reporter*,QFileInfo);
void lexerinit(AbstractTokenBuilder*,Reporter*,const QString&);
void lexerdestroy();
void lexerbegin();
void lexercomment();
void lexercodedoc();

static bool openfile(QFileInfo);
static AbstractTokenBuilder* tokenizer;
static Reporter* reporter;
static QList<FILE*> openfiles;

#define yyfinish()\
	lexerpop_buffer_state();\
	if(!YY_CURRENT_BUFFER)\
		yyterminate()
%}

%option yylineno
%option noyywrap
%option nounput
%option noinput

%x include use import
%x codedoc
%x comment string
I [a-zA-Z0-9_]
L [a-zA-Z_]
D [0-9]
E [eE][+-]?{D}+
Z 0+|0*\.0+|0+\.0*
N {D}+|{D}*\.{D}+|{D}+\.{D}*
R {N}\/
WS [ \t]
NL \n|\r\n
U1 [\x80-\xbf]
U2 [\xc2-\xdf]
U3 [\xe0-\xef]
U4 [\xf0-\xf4]
U {U2}{U1}|{U3}{U1}{U1}|{U4}{U1}{U1}{U1}
O [\?\+\-!\*\|/%~<>]
T [\[\].;:#${}(),^=]

%%
"include"{WS}*"<"              { BEGIN(include); tokenizer->buildIncludeStart(); }
<include>{
[^\t\r\n>]*"/"                 { tokenizer->buildIncludePath(lexertext); }
[^\t\r\n>/]+                   { tokenizer->buildIncludeFile(lexertext); }
">"                            { BEGIN(INITIAL); tokenizer->buildIncludeFinish(); }
}
"use"{WS}*"<"                  { BEGIN(use); tokenizer->buildUseStart(); }
<use>[^\t\r\n>]+               { return tokenizer->buildUse(lexertext); }
<use>">"                       { BEGIN(INITIAL); tokenizer->buildUseFinish(); }
"import"{WS}*"<"               { BEGIN(import); tokenizer->buildImportStart(); }
<import>[^\t\r\n>]+            { return tokenizer->buildImport(lexertext); }
<import>">"                    { BEGIN(INITIAL); tokenizer->buildImportFinish(); }
<<EOF>>                        { tokenizer->buildFileFinish(); yyfinish(); }
"module"                       { return tokenizer->buildModule(); }
"function"                     { return tokenizer->buildFunction(); }
"true"                         { return tokenizer->buildTrue(); }
"false"                        { return tokenizer->buildFalse(); }
"undef"                        { return tokenizer->buildUndef(); }
"const"                        { return tokenizer->buildConst(); }
"param"                        { return tokenizer->buildParam(); }
"if"                           { return tokenizer->buildIf(); }
"as"                           { return tokenizer->buildAs(); }
"else"                         { return tokenizer->buildElse(); }
"for"                          { return tokenizer->buildFor(); }
"return"                       { return tokenizer->buildReturn(); }
"<="                           { return tokenizer->buildLessEqual(); }
">="                           { return tokenizer->buildGreatEqual(); }
"=="                           { return tokenizer->buildEqual(); }
"!="                           { return tokenizer->buildNotEqual(); }
"&&"                           { return tokenizer->buildAnd(); }
"||"                           { return tokenizer->buildOr(); }
"++"                           { return tokenizer->buildIncrement(); }
"+="                           { return tokenizer->buildAddAssign(); }
"--"                           { return tokenizer->buildDecrement(); }
"-="                           { return tokenizer->buildSubtractAssign(); }
"**"                           { return tokenizer->buildCrossProduct(); }
".*"                           { return tokenizer->buildComponentwiseMultiply(); }
"./"                           { return tokenizer->buildComponentwiseDivide(); }
"::"                           { return tokenizer->buildNamespace(); }
"~="                           { return tokenizer->buildAppend(); }
{O}                            { return tokenizer->buildOperator(lexertext[0]); }
{T}                            { return tokenizer->buildLegalChar(lexertext[0]); }
{N}                            { return tokenizer->buildNumber(lexertext); }
{N}{E}                         { return tokenizer->buildNumberExp(lexertext); }
{N}\/{Z}{E}?                   { return tokenizer->buildRational(); }
{R}+{N}{E}?                    { return tokenizer->buildRational(lexertext); }
{L}{I}*                        { return tokenizer->buildIdentifier(lexertext); }
\"                             { BEGIN(string); tokenizer->buildStringStart(); }
<string>{
\\n                            { tokenizer->buildString('\n'); }
\\t                            { tokenizer->buildString('\t'); }
\\r                            { tokenizer->buildString('\r'); }
\\\\                           { tokenizer->buildString('\\'); }
\\\"                           { tokenizer->buildString('"'); }
[^\\\n\"]+                     { tokenizer->buildString(lexertext); }
\"                             { BEGIN(INITIAL); return tokenizer->buildStringFinish(); }
}
"//"[^\n]*\n?                  { tokenizer->buildComment(lexertext); }
"/**"                          { BEGIN(codedoc); return tokenizer->buildCodeDocStart(); }
<codedoc>{
"@"{I}+{WS}+                   { return tokenizer->buildCodeDocParam(lexertext); }
[^*@]*                         { return tokenizer->buildCodeDoc(lexertext); }
"*"+[^*/@]*|"@"                { tokenizer->buildCodeDoc(); }
"*/"                           { BEGIN(INITIAL); return tokenizer->buildCodeDocFinish(); }
}
"/*"                           { BEGIN(comment); tokenizer->buildCommentStart(); }
<comment>{
[^*]*                          { tokenizer->buildComment(lexertext); }
"*"+[^*/]*                     { tokenizer->buildComment(lexertext); }
"*/"                           { BEGIN(INITIAL); tokenizer->buildCommentFinish(); }
}
{WS}+$                         { tokenizer->buildWhiteSpaceError(); }
{WS}+                          { tokenizer->buildWhiteSpace(); }
{NL}                           { tokenizer->buildNewLine(); }
{U}                            { return tokenizer->buildIllegalChar(lexertext); }
.                              { return tokenizer->buildIllegalChar(lexertext); }
%%

void lexererror()
{
	if(reporter) //Reporter can be null
		reporter->reportLexicalError(*tokenizer,lexertext);
}

void lexerinit(AbstractTokenBuilder* b,Reporter* r,QFileInfo fileinfo)
{
	tokenizer=b;
	reporter=r;

	openfile(fileinfo);
	tokenizer->buildFileStart(fileinfo.absoluteDir());
}

void lexerinit(AbstractTokenBuilder* b,Reporter* r,const QString& input)
{
	tokenizer=b;
	reporter=r;
	lexer_scan_string(input.toUtf8().constData());
}

void lexerdestroy()
{
	foreach(FILE* f, openfiles)
		fclose(f);
	openfiles.clear();

	lexerlex_destroy();
}

void lexerbegin()
{
	BEGIN(INITIAL);
}

void lexercomment()
{
	BEGIN(comment);
}

void lexercodedoc()
{
	BEGIN(codedoc);
}

bool openfile(QFileInfo f)
{
	QString fullpath=f.absoluteFilePath();
	FILE* fd=fopen(QFile::encodeName(fullpath),"r");
	if(!fd) {
		if(reporter) //Reporter can be null
			reporter->reportFileMissingError(fullpath);
		return false;
	}
	openfiles.append(fd);
	lexerin=fd;
	return true;
}

void lexerinclude(QFileInfo f)
{
	if(openfile(f))
		lexerpush_buffer_state(lexer_create_buffer(lexerin, YY_BUF_SIZE));
}
