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
#include "tokenreader.h"
#include "reporter.h"

#define YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
#include "parser_yacc.h"

#define YY_NULL 0
extern void lexerinit(AbstractTokenBuilder*,Reporter*,const QString&);
extern int lexerdestroy();
extern int lexerlex();

TokenReader::TokenReader(const QString& s)
{
	lexerinit(this,nullptr,s);
}

TokenReader::~TokenReader()
{
	lexerdestroy();
}

int TokenReader::nextToken()
{
	return lexerlex();
}

unsigned int TokenReader::buildUse(const QString&)
{
	return USE;
}

unsigned int TokenReader::buildImport(const QString&)
{
	return IMPORT;
}

unsigned int TokenReader::buildModule()
{
	return MODULE;
}

unsigned int TokenReader::buildFunction()
{
	return FUNCTION;
}

unsigned int TokenReader::buildTrue()
{
	return TOK_TRUE;
}

unsigned int TokenReader::buildFalse()
{
	return TOK_FALSE;
}

unsigned int TokenReader::buildUndef()
{
	return UNDEF;
}

unsigned int TokenReader::buildConst()
{
	return CONST;
}

unsigned int TokenReader::buildParam()
{
	return PARAM;
}

unsigned int TokenReader::buildIf()
{
	return TOK_IF;
}

unsigned int TokenReader::buildAs()
{
	return TOK_AS;
}

unsigned int TokenReader::buildElse()
{
	return ELSE;
}

unsigned int TokenReader::buildFor()
{
	return FOR;
}

unsigned int TokenReader::buildReturn()
{
	return RETURN;
}

unsigned int TokenReader::buildLessEqual()
{
	return LE;
}

unsigned int TokenReader::buildGreatEqual()
{
	return GE;
}

unsigned int TokenReader::buildEqual()
{
	return EQ;
}

unsigned int TokenReader::buildNotEqual()
{
	return NE;
}

unsigned int TokenReader::buildAnd()
{
	return AND;
}

unsigned int TokenReader::buildOr()
{
	return OR;
}

unsigned int TokenReader::buildComponentwiseMultiply()
{
	return CM;
}

unsigned int TokenReader::buildComponentwiseDivide()
{
	return CD;
}

unsigned int TokenReader::buildIncrement()
{
	return INC;
}

unsigned int TokenReader::buildDecrement()
{
	return DEC;
}

unsigned int TokenReader::buildAddAssign()
{
	return ADDA;
}

unsigned int TokenReader::buildSubtractAssign()
{
	return SUBA;
}

unsigned int TokenReader::buildCrossProduct()
{
	return CP;
}

unsigned int TokenReader::buildNamespace()
{
	return NS;
}

unsigned int TokenReader::buildAppend()
{
	return APPEND;
}

unsigned int TokenReader::buildOperator(unsigned int c)
{
	return c;
}

unsigned int TokenReader::buildLegalChar(unsigned int c)
{
	return c;
}

unsigned int TokenReader::buildByteOrderMark()
{
	return BOM;
}

unsigned int TokenReader::buildIllegalChar(const QString&)
{
	return YY_NULL;
}

unsigned int TokenReader::buildNumber(const QString&)
{
	return NUMBER;
}

unsigned int TokenReader::buildNumberExp(const QString&)
{
	return NUMBER;
}

unsigned int TokenReader::buildRational()
{
	return UNDEF;
}

unsigned int TokenReader::buildRational(const QString&)
{
	return NUMBER;
}

unsigned int TokenReader::buildIdentifier(const QString&)
{
	return IDENTIFIER;
}

unsigned int TokenReader::buildStringFinish()
{
	return STRING;
}

unsigned int TokenReader::buildCodeDocStart()
{
	return DOCSTART;
}

unsigned int TokenReader::buildCodeDoc(const QString&)
{
	return DOCTEXT;
}

unsigned int TokenReader::buildCodeDocParam(const QString&)
{
	return DOCPARAM;
}

unsigned int TokenReader::buildCodeDocFinish()
{
	return DOCEND;
}
