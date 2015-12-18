/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

#include "syntaxhighlighter.h"
#include "reporter.h"

extern int lexerlex_destroy();
extern void lexerinit(AbstractTokenBuilder*,Reporter*,QString,bool);
extern int lexerlex();
extern void lexerbegin();
extern void lexercomment();
extern void lexercodedoc();
extern int lexerleng;
#define YY_CONTINUE 1;

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
	: QSyntaxHighlighter(parent)
{
	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);

	numberFormat.setForeground(Qt::red);

	operatorFormat.setForeground(Qt::darkMagenta);

	errorFormat.setBackground(Qt::red);

	stringFormat.setForeground(Qt::darkGreen);

	codeDocFormat.setForeground(Qt::darkBlue);

	codeDocParamFormat.setForeground(Qt::blue);
	codeDocParamFormat.setFontWeight(QFont::Bold);
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
	startIndex=0;
	lexerinit(this,NULL,text,false);

	//Force lexer into correct state
	switch(previousBlockState()) {
	case Initial:
		lexerbegin();
		break;
	case Comment:
		lexercomment();
		break;
	case CodeDoc:
		lexercodedoc();
		break;
	}

	while(nextToken());

}

void SyntaxHighlighter::stop()
{
	lexerlex_destroy();
}

int SyntaxHighlighter::nextToken()
{
	int res=lexerlex();
	startIndex+=lexerleng;
	return res;
}

int SyntaxHighlighter::getPosition() const
{
	return startIndex;
}

int SyntaxHighlighter::getLineNumber() const
{
	return 1; /*TODO for now we don't really care what line
	we are on for syntax highlighting */
}

void SyntaxHighlighter::buildIncludeStart()
{
	setFormat(startIndex,lexerleng-1,keywordFormat);
	startIndex+=lexerleng;
}

void SyntaxHighlighter::buildIncludeFile(QString)
{
	setFormat(startIndex,lexerleng,stringFormat);
	startIndex+=lexerleng;
}

void SyntaxHighlighter::buildIncludePath(QString)
{
	setFormat(startIndex,lexerleng,stringFormat);
	startIndex+=lexerleng;
}

void SyntaxHighlighter::buildIncludeFinish()
{
	startIndex++;
}

void SyntaxHighlighter::buildUseStart()
{
	setFormat(startIndex,lexerleng-1,keywordFormat);
	startIndex+=lexerleng;
}

unsigned int SyntaxHighlighter::buildUse(QString)
{
	setFormat(startIndex,lexerleng,stringFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildUseFinish()
{
	startIndex++;
}

void SyntaxHighlighter::buildImportStart()
{
	setFormat(startIndex,lexerleng-1,keywordFormat);
	startIndex+=lexerleng;
}

unsigned int SyntaxHighlighter::buildImport(QString)
{
	setFormat(startIndex,lexerleng,stringFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildImportFinish()
{
	startIndex++;
}

unsigned int SyntaxHighlighter::buildModule()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildFunction()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildTrue()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildFalse()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildUndef()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildConst()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildParam()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildIf()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildAs()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildElse()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildFor()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildReturn()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildLessEqual()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildGreatEqual()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildEqual()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildNotEqual()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildAnd()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildOr()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildComponentwiseMultiply()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildComponentwiseDivide()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildIncrement()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildDecrement()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildAddAssign()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildSubtractAssign()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildCrossProduct()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildNamespace()
{
	setFormat(startIndex,lexerleng,keywordFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildAssign()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildAdd()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildSubtract()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildTernaryCondition()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildTernaryAlternate()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildNot()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildMultiply()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildDivide()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildLength()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildModulus()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildConcatenate()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildAppend()
{
	setFormat(startIndex,lexerleng,operatorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildLegalChar(unsigned int)
{
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildIllegalChar()
{
	setFormat(startIndex,lexerleng,errorFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildNumber(QString)
{
	setFormat(startIndex,lexerleng,numberFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildIdentifier(QString)
{
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildStringStart()
{
	stringStart=startIndex;
	startIndex++;
}

void SyntaxHighlighter::buildString(QChar)
{
	startIndex++;
}

void SyntaxHighlighter::buildString(QString)
{
	startIndex+=lexerleng;
}

unsigned int SyntaxHighlighter::buildStringFinish()
{
	int stringLen=(startIndex+1)-stringStart;
	setFormat(stringStart,stringLen,stringFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildCommentStart()
{
	setCurrentBlockState(Comment);
	setFormat(startIndex,lexerleng,stringFormat);
	startIndex+=lexerleng;
}

unsigned int SyntaxHighlighter::buildComment(QString)
{
	if(previousBlockState()==Comment)
		setCurrentBlockState(Comment);

	setFormat(startIndex,lexerleng,stringFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildCommentFinish()
{
	setCurrentBlockState(Initial);
	setFormat(startIndex,lexerleng,stringFormat);
	startIndex+=lexerleng;
}

unsigned int SyntaxHighlighter::buildCodeDocStart()
{
	setCurrentBlockState(CodeDoc);
	setFormat(startIndex,lexerleng,codeDocFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildCodeDoc(QString)
{
	if(previousBlockState()==CodeDoc)
		setCurrentBlockState(CodeDoc);

	setFormat(startIndex,lexerleng,codeDocFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildCodeDoc()
{
	if(previousBlockState()==CodeDoc)
		setCurrentBlockState(CodeDoc);

	setFormat(startIndex,lexerleng,codeDocFormat);
	startIndex+=lexerleng;
}

unsigned int SyntaxHighlighter::buildCodeDocParam(QString)
{
	setFormat(startIndex,lexerleng,codeDocParamFormat);
	return YY_CONTINUE;
}

unsigned int SyntaxHighlighter::buildCodeDocFinish()
{
	setCurrentBlockState(Initial);
	setFormat(startIndex,lexerleng,codeDocFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildWhiteSpaceError()
{
	setFormat(startIndex,lexerleng,errorFormat);
}

void SyntaxHighlighter::buildWhiteSpace()
{
	startIndex+=lexerleng;
}

void SyntaxHighlighter::buildNewLine()
{
}

void SyntaxHighlighter::buildFileStart(QDir)
{
}

void SyntaxHighlighter::buildFileFinish()
{
}
