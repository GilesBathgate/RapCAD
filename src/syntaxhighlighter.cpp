/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#include "preferences.h"
#include <contrib/qtcompat.h>

extern int lexerdestroy(yyscan_t);
extern void lexerinit(yyscan_t*,AbstractTokenBuilder*,const QString&);
extern int lexerlex(yyscan_t);
extern void lexerbegin(yyscan_t);
extern void lexercomment(yyscan_t);
extern void lexercodedoc(yyscan_t);
extern int lexerget_leng(yyscan_t);
static constexpr int YY_CONTINUE=1;

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent) :
	QSyntaxHighlighter(parent),
	startIndex(0),
	scanner(nullptr)
{
	auto& p=Preferences::getInstance();
	if(p.getDarkTheme()) {
		keywordFormat.setForeground(QColorConstants::Svg::dodgerblue);
		keywordFormat.setFontWeight(QFont::Bold);

		numberFormat.setForeground(QColorConstants::Svg::palegreen);

		operatorFormat.setForeground(QColorConstants::Svg::plum);

		errorFormat.setBackground(Qt::red);

		stringFormat.setForeground(QColorConstants::Svg::orange);

		commentFormat.setForeground(QColorConstants::Svg::springgreen);

		moduleFormat.setForeground(Qt::cyan);

		codeDocFormat.setForeground(QColorConstants::Svg::skyblue);

		codeDocParamFormat.setForeground(QColorConstants::Svg::deepskyblue);
		codeDocParamFormat.setFontWeight(QFont::Bold);
	} else {
		keywordFormat.setForeground(Qt::darkBlue);
		keywordFormat.setFontWeight(QFont::Bold);

		numberFormat.setForeground(Qt::red);

		operatorFormat.setForeground(Qt::darkMagenta);

		errorFormat.setBackground(Qt::red);

		stringFormat.setForeground(Qt::darkGreen);

		commentFormat.setForeground(Qt::darkGreen);

		moduleFormat.setForeground(Qt::darkCyan);

		codeDocFormat.setForeground(Qt::darkBlue);

		codeDocParamFormat.setForeground(Qt::blue);
		codeDocParamFormat.setFontWeight(QFont::Bold);
	}
}

void SyntaxHighlighter::setModuleNames(const QHash<QString,Module*>& names)
{
	moduleNames = names;
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
	startIndex=0;
	lexerinit(&scanner,this,text);

	//Force lexer into correct state
	switch(static_cast<BlockStates>(previousBlockState())) {
		case BlockStates::Initial:
			lexerbegin(scanner);
			break;
		case BlockStates::Comment:
			setCurrentBlockState(static_cast<int>(BlockStates::Comment));
			lexercomment(scanner);
			break;
		case BlockStates::CodeDoc:
			setCurrentBlockState(static_cast<int>(BlockStates::CodeDoc));
			lexercodedoc(scanner);
			break;
	}

	while(nextToken());

	lexerdestroy(scanner);
}

int SyntaxHighlighter::nextToken()
{
	const int res=lexerlex(scanner);
	startIndex+=lexerget_leng(scanner);
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
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len-1,keywordFormat);
	startIndex+=len;
}

void SyntaxHighlighter::buildIncludeFile(const QString&)
{
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len,stringFormat);
	startIndex+=len;
}

void SyntaxHighlighter::buildIncludePath(const QString&)
{
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len,stringFormat);
	startIndex+=len;
}

void SyntaxHighlighter::buildIncludeFinish()
{
	startIndex++;
}

void SyntaxHighlighter::buildUseStart()
{
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len-1,keywordFormat);
	startIndex+=len;
}

int SyntaxHighlighter::buildUse(const QString&)
{
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len,stringFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildUseFinish()
{
	startIndex++;
}

void SyntaxHighlighter::buildImportStart()
{
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len-1,keywordFormat);
	startIndex+=len;
}

int SyntaxHighlighter::buildImport(const QString&)
{
	setFormat(startIndex,lexerget_leng(scanner),stringFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildImportFinish()
{
	startIndex++;
}

int SyntaxHighlighter::buildModule()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildFunction()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildTrue()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildFalse()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildUndef()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildConst()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildParam()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildIf()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildAs()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildElse()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildFor()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildReturn()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildLessEqual()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildGreatEqual()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildEqual()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildNotEqual()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildAnd()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildOr()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildComponentwiseMultiply()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildComponentwiseDivide()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildPlusMinus()
{
	// this adjusts for 2 byte unicode char
	setFormat(startIndex--,1,operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildIncrement()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildDecrement()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildAddAssign()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildSubtractAssign()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildCrossProduct()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildNamespace()
{
	setFormat(startIndex,lexerget_leng(scanner),keywordFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildAppend()
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildOperator(int)
{
	setFormat(startIndex,lexerget_leng(scanner),operatorFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildLegalChar(int)
{
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildByteOrderMark()
{
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildIllegalChar(const QString& s)
{
	const int stringLen=s.length();
	setFormat(startIndex,stringLen,errorFormat);
	startIndex-=(lexerget_leng(scanner)-stringLen);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildNumber(const QString&)
{
	setFormat(startIndex,lexerget_leng(scanner),numberFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildNumberExp(const QString&)
{
	setFormat(startIndex,lexerget_leng(scanner),numberFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildRational()
{
	setFormat(startIndex,lexerget_leng(scanner),numberFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildRational(const QString&)
{
	setFormat(startIndex,lexerget_leng(scanner),numberFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildIdentifier(const QString& i)
{
	if(moduleNames.contains(i))
		setFormat(startIndex,lexerget_leng(scanner),moduleFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildStringStart()
{
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len,stringFormat);
	startIndex+=len;
}

void SyntaxHighlighter::buildString(QChar)
{
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len,stringFormat);
	startIndex+=len;
}

void SyntaxHighlighter::buildString(const QString& s)
{
	const int stringLen=s.length();
	setFormat(startIndex,stringLen,stringFormat);
	startIndex+=stringLen;
}

int SyntaxHighlighter::buildStringFinish()
{
	setFormat(startIndex,lexerget_leng(scanner),stringFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildCommentStart()
{
	setCurrentBlockState(static_cast<int>(BlockStates::Comment));
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len,commentFormat);
	startIndex+=len;
}

void SyntaxHighlighter::buildComment(const QString& s)
{
	const int stringLen=s.length();
	setFormat(startIndex,stringLen,commentFormat);
	startIndex+=stringLen;
}

void SyntaxHighlighter::buildCommentFinish()
{
	setCurrentBlockState(static_cast<int>(BlockStates::Initial));
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len,commentFormat);
	startIndex+=len;
}

int SyntaxHighlighter::buildCodeDocStart()
{
	setCurrentBlockState(static_cast<int>(BlockStates::CodeDoc));
	setFormat(startIndex,lexerget_leng(scanner),codeDocFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildCodeDoc(const QString& s)
{
	const int stringLen=s.length();
	setFormat(startIndex,stringLen,codeDocFormat);
	/* Need to adjust back the index because this is a token and thus
	 * index is incremented in the NextToken() call */
	startIndex-=(lexerget_leng(scanner)-stringLen);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildCodeDoc()
{
	const int len=lexerget_leng(scanner);
	setFormat(startIndex,len,codeDocFormat);
	startIndex+=len;
}

int SyntaxHighlighter::buildCodeDocParam(const QString&)
{
	setFormat(startIndex,lexerget_leng(scanner),codeDocParamFormat);
	return YY_CONTINUE;
}

int SyntaxHighlighter::buildCodeDocFinish()
{
	setCurrentBlockState(static_cast<int>(BlockStates::Initial));
	setFormat(startIndex,lexerget_leng(scanner),codeDocFormat);
	return YY_CONTINUE;
}

void SyntaxHighlighter::buildWhiteSpaceError()
{
	setFormat(startIndex,lexerget_leng(scanner),errorFormat);
}

void SyntaxHighlighter::buildWhiteSpace()
{
	startIndex+=lexerget_leng(scanner);
}

void SyntaxHighlighter::buildNewLine()
{
}

void SyntaxHighlighter::buildFileStart(QFileInfo)
{
}

void SyntaxHighlighter::buildFileFinish()
{
}

QString SyntaxHighlighter::getToken() const
{
	return QString();
}
