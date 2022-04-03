/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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
#include "decimal.h"
#include "unistd.h"

void lexerinclude();
void lexerinit(yyscan_t*,AbstractTokenBuilder*,const QFileInfo&);
void lexerinit(yyscan_t*,AbstractTokenBuilder*,const QString&);
void lexerdestroy();
void lexerbegin();
void lexercomment();
void lexercodedoc();

#define lexertext yytext
#define tokenizer yyextra

#define yyfinish()\
	lexerpop_buffer_state(yyscanner);\
	if(!YY_CURRENT_BUFFER)\
		yyterminate()
%}

%option reentrant
%option extra-type="AbstractTokenBuilder*"
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
UNICODE {U2}{U1}|{U3}{U1}{U1}|{U4}{U1}{U1}{U1}
B \xef\xbb\xbf
O [\?\+\-!\*\|/%~<>]
T [\[\].;:#${}(),^=]
U "um"|"mm"|"cm"|"m"|"th"|"in"|"ft"

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
{N}{U}?                        { return tokenizer->buildNumber(lexertext); }
{N}{E}{U}?                     { return tokenizer->buildNumberExp(lexertext); }
{R}{Z}{E}?{U}                  { return tokenizer->buildRational(); }
{R}+{N}{E}?{U}                 { return tokenizer->buildRational(lexertext); }
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
{B}                            { return tokenizer->buildByteOrderMark(); }
{UNICODE}                      { return tokenizer->buildIllegalChar(lexertext); }
.                              { return tokenizer->buildIllegalChar(lexertext); }
%%

void lexerinit(yyscan_t* s,AbstractTokenBuilder* b,const QFileInfo& fileinfo)
{
	lexerlex_init_extra(b,s);
	b->buildFileStart(fileinfo);
}

void lexerinit(yyscan_t* s,AbstractTokenBuilder* b, const QString& input)
{
	lexerlex_init_extra(b,s);
	lexer_scan_string(input.toUtf8().constData(),*s);
}

void lexerdestroy(yyscan_t s)
{
	lexerlex_destroy(s);
}

void lexerbegin(yyscan_t s)
{
	auto* yyg=static_cast<struct yyguts_t*>(s);
	BEGIN(INITIAL);
}

void lexercomment(yyscan_t s)
{
	auto* yyg=static_cast<struct yyguts_t*>(s);
	BEGIN(comment);
}

void lexercodedoc(yyscan_t s)
{
	auto* yyg=static_cast<struct yyguts_t*>(s);
	BEGIN(codedoc);
}

void lexerinclude(yyscan_t s)
{
	lexerpush_buffer_state(lexer_create_buffer(lexerget_in(s),YY_BUF_SIZE,s),s);
}
