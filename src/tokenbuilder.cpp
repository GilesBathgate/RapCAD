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

#include "tokenbuilder.h"
#include "decimal.h"
#include "parser_yacc.h"

#define YY_NULL 0
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

TokenBuilder::TokenBuilder(Reporter& r,QFileInfo fileinfo) : TokenBuilder()
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

unsigned int TokenBuilder::buildUse(const QString& str)
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

unsigned int TokenBuilder::buildImport(const QString& str)
{
	parserlval.text = new QString(str);
	return IMPORT;
}

void TokenBuilder::buildImportFinish()
{
}

unsigned int TokenBuilder::buildModule()
{
	return MODULE;
}

unsigned int TokenBuilder::buildFunction()
{
	return FUNCTION;
}

unsigned int TokenBuilder::buildTrue()
{
	return TOK_TRUE;
}

unsigned int TokenBuilder::buildFalse()
{
	return TOK_FALSE;
}

unsigned int TokenBuilder::buildUndef()
{
	return UNDEF;
}

unsigned int TokenBuilder::buildConst()
{
	return CONST;
}

unsigned int TokenBuilder::buildParam()
{
	return PARAM;
}

unsigned int TokenBuilder::buildIf()
{
	return TOK_IF;
}

unsigned int TokenBuilder::buildAs()
{
	return TOK_AS;
}

unsigned int TokenBuilder::buildElse()
{
	return ELSE;
}

unsigned int TokenBuilder::buildFor()
{
	return FOR;
}

unsigned int TokenBuilder::buildReturn()
{
	return RETURN;
}

unsigned int TokenBuilder::buildLessEqual()
{
	return LE;
}

unsigned int TokenBuilder::buildGreatEqual()
{
	return GE;
}

unsigned int TokenBuilder::buildEqual()
{
	return EQ;
}

unsigned int TokenBuilder::buildNotEqual()
{
	return NE;
}

unsigned int TokenBuilder::buildAnd()
{
	return AND;
}

unsigned int TokenBuilder::buildOr()
{
	return OR;
}

unsigned int TokenBuilder::buildComponentwiseMultiply()
{
	return CM;
}

unsigned int TokenBuilder::buildComponentwiseDivide()
{
	return CD;
}

unsigned int TokenBuilder::buildIncrement()
{
	return INC;
}

unsigned int TokenBuilder::buildDecrement()
{
	return DEC;
}

unsigned int TokenBuilder::buildAddAssign()
{
	return ADDA;
}

unsigned int TokenBuilder::buildSubtractAssign()
{
	return SUBA;
}

unsigned int TokenBuilder::buildCrossProduct()
{
	return CP;
}

unsigned int TokenBuilder::buildNamespace()
{
	return NS;
}

unsigned int TokenBuilder::buildAppend()
{
	return APPEND;
}

unsigned int TokenBuilder::buildOperator(unsigned int c)
{
	return c;
}

unsigned int TokenBuilder::buildLegalChar(unsigned int c)
{
	return c;
}

unsigned int TokenBuilder::buildIllegalChar(const QString&)
{
	lexererror();
	return YY_NULL;
}

unsigned int TokenBuilder::buildNumber(const QString& str)
{
	parserlval.number = new decimal(to_decimal(str));
	return NUMBER;
}

unsigned int TokenBuilder::buildNumberExp(const QString& str)
{
	parserlval.number = new decimal(parse_numberexp(str));
	return NUMBER;
}

unsigned int TokenBuilder::buildRational()
{
	return UNDEF;
}

unsigned int TokenBuilder::buildRational(const QString& s)
{
	parserlval.number = new decimal(parse_rational(s));
	return NUMBER;
}

unsigned int TokenBuilder::buildIdentifier(const QString& str)
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

unsigned int TokenBuilder::buildStringFinish()
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

unsigned int TokenBuilder::buildCodeDocStart()
{
	return DOCSTART;
}

unsigned int TokenBuilder::buildCodeDoc(const QString& s)
{
	parserlval.text = new QString(s.trimmed());
	return DOCTEXT;
}

void TokenBuilder::buildCodeDoc()
{
}

unsigned int TokenBuilder::buildCodeDocParam(const QString& s)
{
	parserlval.text = new QString(s.trimmed());
	return DOCPARAM;
}

unsigned int TokenBuilder::buildCodeDocFinish()
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
