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

#include "syntaxhighlighter.h"

extern void lexerinit(AbstractTokenBuilder*,QString,bool);
extern int lexerlex();
extern int lexerleng;
#define YY_CONTINUE 1;

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
	: QSyntaxHighlighter(parent)
{
	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);

	numberFormat.setForeground(Qt::red);

	operatorFormat.setForeground(Qt::darkMagenta);

	stringFormat.setForeground(Qt::darkGreen);
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
	startIndex=0;
	lexerinit(this,text,false);
	while(lexerlex())
		startIndex+=lexerleng;
}

void SyntaxHighlighter::buildIncludeStart()
{
}

void SyntaxHighlighter::buildIncludeFile(QString)
{
}

void SyntaxHighlighter::buildIncludePath(QString)
{
}

void SyntaxHighlighter::buildIncludeFinish()
{
}

void SyntaxHighlighter::buildUseStart()
{
}

unsigned int SyntaxHighlighter::buildUse(QString)
{
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildUseFinish()
{
}

void SyntaxHighlighter::buildImportStart()
{
}

unsigned int SyntaxHighlighter::buildImport(QString)
{
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildImportFinish()
{
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

unsigned int SyntaxHighlighter::buildOuterProduct()
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

unsigned int SyntaxHighlighter::buildLegalChar(unsigned int)
{
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
}

unsigned int SyntaxHighlighter::buildComment(QString)
{
	setFormat(startIndex,lexerleng,stringFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildCommentFinish()
{
}

void SyntaxHighlighter::buildWhiteSpaceError()
{
}

void SyntaxHighlighter::buildWhiteSpace()
{
	startIndex+=lexerleng;
}

void SyntaxHighlighter::buildFileStart(QDir)
{
}

void SyntaxHighlighter::buildFileFinish()
{
}
