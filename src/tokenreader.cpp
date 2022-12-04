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
#include "tokenreader.h"
#include "reporter.h"

#define YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
#include "parser_yacc.h"

static constexpr int YY_NULL=0;
extern void lexerinit(yyscan_t*,AbstractTokenBuilder*,const QString&);
extern int lexerdestroy(yyscan_t);
extern int lexerlex(yyscan_t);

TokenReader::TokenReader(const QString& s) :
	scanner(nullptr)
{
	lexerinit(&scanner,this,s);
}

TokenReader::~TokenReader()
{
	lexerdestroy(scanner);
}

int TokenReader::nextToken()
{
	return lexerlex(scanner);
}

int TokenReader::buildUse(const QString&)
{
	return USE;
}

int TokenReader::buildImport(const QString&)
{
	return IMPORT;
}

int TokenReader::buildModule()
{
	return MODULE;
}

int TokenReader::buildFunction()
{
	return FUNCTION;
}

int TokenReader::buildTrue()
{
	return TRUE;
}

int TokenReader::buildFalse()
{
	return FALSE;
}

int TokenReader::buildUndef()
{
	return UNDEF;
}

int TokenReader::buildConst()
{
	return CONST;
}

int TokenReader::buildParam()
{
	return PARAM;
}

int TokenReader::buildIf()
{
	return IF;
}

int TokenReader::buildAs()
{
	return AS;
}

int TokenReader::buildElse()
{
	return ELSE;
}

int TokenReader::buildFor()
{
	return FOR;
}

int TokenReader::buildReturn()
{
	return RETURN;
}

int TokenReader::buildLessEqual()
{
	return LE;
}

int TokenReader::buildGreatEqual()
{
	return GE;
}

int TokenReader::buildEqual()
{
	return EQ;
}

int TokenReader::buildNotEqual()
{
	return NE;
}

int TokenReader::buildAnd()
{
	return AND;
}

int TokenReader::buildOr()
{
	return OR;
}

int TokenReader::buildComponentwiseMultiply()
{
	return CM;
}

int TokenReader::buildComponentwiseDivide()
{
	return CD;
}

int TokenReader::buildPlusMinus()
{
	return PM;
}

int TokenReader::buildIncrement()
{
	return INC;
}

int TokenReader::buildDecrement()
{
	return DEC;
}

int TokenReader::buildAddAssign()
{
	return ADDA;
}

int TokenReader::buildSubtractAssign()
{
	return SUBA;
}

int TokenReader::buildCrossProduct()
{
	return CP;
}

int TokenReader::buildNamespace()
{
	return NS;
}

int TokenReader::buildAppend()
{
	return APPEND;
}

int TokenReader::buildOperator(int c)
{
	return c;
}

int TokenReader::buildLegalChar(int c)
{
	return c;
}

int TokenReader::buildByteOrderMark()
{
	return BOM;
}

int TokenReader::buildIllegalChar(const QString&)
{
	return YY_NULL;
}

int TokenReader::buildNumber(const QString&)
{
	return NUMBER;
}

int TokenReader::buildNumberExp(const QString&)
{
	return NUMBER;
}

int TokenReader::buildRational()
{
	return UNDEF;
}

int TokenReader::buildRational(const QString&)
{
	return NUMBER;
}

int TokenReader::buildIdentifier(const QString&)
{
	return IDENTIFIER;
}

int TokenReader::buildStringFinish()
{
	return STRING;
}

int TokenReader::buildCodeDocStart()
{
	return DOCSTART;
}

int TokenReader::buildCodeDoc(const QString&)
{
	return DOCTEXT;
}

int TokenReader::buildCodeDocParam(const QString&)
{
	return DOCPARAM;
}

int TokenReader::buildCodeDocFinish()
{
	return DOCEND;
}
