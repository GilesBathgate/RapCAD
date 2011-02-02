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

#include "tokenbuilder.h"
#include "parser_yacc.h"
#define YY_NULL 0
extern void lexerinclude(const char*);


TokenBuilder::TokenBuilder()
{
}

void TokenBuilder::buildIncludeStart()
{
}

void TokenBuilder::buildIncludeFile(QString str)
{
	filename = str;
}

void TokenBuilder::buildIncludePath(QString str)
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

	const char* fullpath = fileinfo.absoluteFilePath().toLocal8Bit();
	lexerinclude(fullpath);

}

void TokenBuilder::buildUseStart()
{
}

unsigned int TokenBuilder::buildUse(QString str)
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

unsigned int TokenBuilder::buildImport(QString str)
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
	return IF;
}

unsigned int TokenBuilder::buildAs()
{
	return AS;
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

unsigned int TokenBuilder::buildOuterProduct()
{
	return CP;
}

unsigned int TokenBuilder::buildNamespace()
{
	return NS;
}

unsigned int TokenBuilder::buildAssign()
{
	return '=';
}

unsigned int TokenBuilder::buildAdd()
{
	return '+';
}

unsigned int TokenBuilder::buildSubtract()
{
	return '-';
}

unsigned int TokenBuilder::buildTernaryCondition()
{
	return '?';
}

unsigned int  TokenBuilder::buildTernaryAlternate()
{
	return ':';
}

unsigned int TokenBuilder::buildNot()
{
	return '!';
}

unsigned int TokenBuilder::buildLegalChar(unsigned int c)
{
	return c;
}

unsigned int TokenBuilder::buildNumber(QString str)
{
	parserlval.number = str.toDouble();
	return NUMBER;
}

unsigned int TokenBuilder::buildIdentifier(QString str)
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

void TokenBuilder::buildString(QString s)
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

unsigned int TokenBuilder::buildComment(QString)
{
	return YY_NULL;
}

void TokenBuilder::buildCommentFinish()
{
}

void TokenBuilder::buildWhiteSpaceError()
{
}

void TokenBuilder::buildWhiteSpace()
{
}

void TokenBuilder::buildFileStart(QDir pth)
{
	path_stack.push(pth);
}

void TokenBuilder::buildFileFinish()
{
	path_stack.pop();
}
