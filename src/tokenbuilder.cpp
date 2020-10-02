/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2020 Giles Bathgate
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

#include "tokenbuilder.h"
#include "decimal.h"
#include "parser_yacc.h"

static constexpr int YY_NULL=0;
extern void lexerinit(AbstractTokenBuilder*,Reporter*,const QString&);
extern void lexerinit(AbstractTokenBuilder*,Reporter*,QFileInfo);
extern int lexerdestroy();
extern void lexerinclude(QFileInfo);
extern void lexererror();
extern int lexerlex();
extern char* lexertext;
extern int lexerleng;
extern int lexerlineno;

TokenBuilder::TokenBuilder() :
	stringcontents(nullptr),
	position(0)
{
}

QString TokenBuilder::getToken() const
{
	return token;
}

TokenBuilder::TokenBuilder(const QString& s) : TokenBuilder()
{
	lexerinit(this,nullptr,s);
}

TokenBuilder::TokenBuilder(Reporter& r,const QString& s) : TokenBuilder()
{
	lexerinit(this,&r,s);
}

TokenBuilder::TokenBuilder(Reporter& r,const QFileInfo& fileinfo) : TokenBuilder()
{
	lexerinit(this,&r,fileinfo);
}

TokenBuilder::~TokenBuilder()
{
	lexerdestroy();
}

int TokenBuilder::nextToken()
{
	position+=lexerleng;
	int next=lexerlex();
	if(next) token=QString::fromUtf8(lexertext,lexerleng);
	return next;
}

int TokenBuilder::getPosition() const
{
	return position;
}

int TokenBuilder::getLineNumber() const
{
	return lexerlineno;
}

void TokenBuilder::buildIncludeStart()
{
}

void TokenBuilder::buildIncludeFile(const QString& str)
{
	filename = str;
}

void TokenBuilder::buildIncludePath(const QString& str)
{
	filepath = str;
}

void TokenBuilder::buildIncludeFinish()
{
	if(filename.isEmpty())
		return;

	QDir currentpath = path_stack.top();
	if(filepath.isEmpty()) {
		path_stack.push(currentpath);
	} else {
		QFileInfo dirinfo(currentpath,filepath);
		path_stack.push(dirinfo.dir());
		filepath.clear();
	}

	currentpath = path_stack.top();
	QFileInfo fileinfo(currentpath,filename);

	if(!fileinfo.exists())
		path_stack.pop();

	filename.clear();

	lexerinclude(fileinfo);

}

void TokenBuilder::buildUseStart()
{
}

int TokenBuilder::buildUse(const QString& str)
{
	parserlval.text = new QString(str);
	return USE;
}

void TokenBuilder::buildUseFinish()
{
}

void TokenBuilder::buildImportStart()
{
}

int TokenBuilder::buildImport(const QString& str)
{
	parserlval.text = new QString(str);
	return IMPORT;
}

void TokenBuilder::buildImportFinish()
{
}

int TokenBuilder::buildModule()
{
	return MODULE;
}

int TokenBuilder::buildFunction()
{
	return FUNCTION;
}

int TokenBuilder::buildTrue()
{
	return TOK_TRUE;
}

int TokenBuilder::buildFalse()
{
	return TOK_FALSE;
}

int TokenBuilder::buildUndef()
{
	return UNDEF;
}

int TokenBuilder::buildConst()
{
	return CONST;
}

int TokenBuilder::buildParam()
{
	return PARAM;
}

int TokenBuilder::buildIf()
{
	return TOK_IF;
}

int TokenBuilder::buildAs()
{
	return TOK_AS;
}

int TokenBuilder::buildElse()
{
	return ELSE;
}

int TokenBuilder::buildFor()
{
	return FOR;
}

int TokenBuilder::buildReturn()
{
	return RETURN;
}

int TokenBuilder::buildLessEqual()
{
	return LE;
}

int TokenBuilder::buildGreatEqual()
{
	return GE;
}

int TokenBuilder::buildEqual()
{
	return EQ;
}

int TokenBuilder::buildNotEqual()
{
	return NE;
}

int TokenBuilder::buildAnd()
{
	return AND;
}

int TokenBuilder::buildOr()
{
	return OR;
}

int TokenBuilder::buildComponentwiseMultiply()
{
	return CM;
}

int TokenBuilder::buildComponentwiseDivide()
{
	return CD;
}

int TokenBuilder::buildIncrement()
{
	return INC;
}

int TokenBuilder::buildDecrement()
{
	return DEC;
}

int TokenBuilder::buildAddAssign()
{
	return ADDA;
}

int TokenBuilder::buildSubtractAssign()
{
	return SUBA;
}

int TokenBuilder::buildCrossProduct()
{
	return CP;
}

int TokenBuilder::buildNamespace()
{
	return NS;
}

int TokenBuilder::buildAppend()
{
	return APPEND;
}

int TokenBuilder::buildOperator(int c)
{
	return c;
}

int TokenBuilder::buildLegalChar(int c)
{
	return c;
}

int TokenBuilder::buildByteOrderMark()
{
	return BOM;
}

int TokenBuilder::buildIllegalChar(const QString&)
{
	lexererror();
	return YY_NULL;
}

int TokenBuilder::buildNumber(const QString& str)
{
	parserlval.number = new decimal(to_decimal(str));
	return NUMBER;
}

int TokenBuilder::buildNumberExp(const QString& str)
{
	parserlval.number = new decimal(parse_numberexp(str));
	return NUMBER;
}

int TokenBuilder::buildRational()
{
	return UNDEF;
}

int TokenBuilder::buildRational(const QString& s)
{
	parserlval.number = new decimal(parse_rational(s));
	return NUMBER;
}

int TokenBuilder::buildIdentifier(const QString& str)
{
	parserlval.text = new QString(str);
	return IDENTIFIER;
}

void TokenBuilder::buildStringStart()
{
	stringcontents = new QString();
}

void TokenBuilder::buildString(QChar c)
{
	stringcontents->append(c);
}

void TokenBuilder::buildString(const QString& s)
{
	stringcontents->append(s);
}

int TokenBuilder::buildStringFinish()
{
	parserlval.text = stringcontents;
	return STRING;
}

void TokenBuilder::buildCommentStart()
{
}

void TokenBuilder::buildComment(const QString&)
{
}

void TokenBuilder::buildCommentFinish()
{
}

int TokenBuilder::buildCodeDocStart()
{
	return DOCSTART;
}

int TokenBuilder::buildCodeDoc(const QString& s)
{
	parserlval.text = new QString(s.trimmed());
	return DOCTEXT;
}

void TokenBuilder::buildCodeDoc()
{
}

int TokenBuilder::buildCodeDocParam(const QString& s)
{
	parserlval.text = new QString(s.trimmed());
	return DOCPARAM;
}

int TokenBuilder::buildCodeDocFinish()
{
	return DOCEND;
}

void TokenBuilder::buildWhiteSpaceError()
{
	position+=lexerleng;
}

void TokenBuilder::buildWhiteSpace()
{
	position+=lexerleng;
}

void TokenBuilder::buildNewLine()
{
	position=1;
}

void TokenBuilder::buildFileStart(QDir pth)
{
	path_stack.push(pth);
}

void TokenBuilder::buildFileFinish()
{
	if(!path_stack.isEmpty())
		path_stack.pop();
}
