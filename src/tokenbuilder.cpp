/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010  Giles Bathgate
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

void TokenBuilder::BuildIncludeStart()
{
}

void TokenBuilder::BuildIncludeFile(QString str)
{
    filename = str;
}

void TokenBuilder::BuildIncludePath(QString str)
{
    filepath = str;
}

void TokenBuilder::BuildIncludeFinish()
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
    filename.clear();

    const char* fullpath = fileinfo.absoluteFilePath().toLocal8Bit();
    lexerinclude(fullpath);

}

void TokenBuilder::BuildUseStart()
{
}

unsigned int TokenBuilder::BuildUse(QString str)
{
    parserlval.text = new QString(str); return USE;
}

void TokenBuilder::BuildUseFinish()
{
}

unsigned int TokenBuilder::BuildModule()
{
    return MODULE;
}

unsigned int TokenBuilder::BuildFunction()
{
    return FUNCTION;
}

unsigned int TokenBuilder::BuildTrue()
{
    return TOK_TRUE;
}

unsigned int TokenBuilder::BuildFalse()
{
    return TOK_FALSE;
}

unsigned int TokenBuilder::BuildUndef()
{
    return UNDEF;
}

unsigned int TokenBuilder::BuildConst()
{
    return CONST;
}

unsigned int TokenBuilder::BuildParam()
{
    return PARAM;
}

unsigned int TokenBuilder::BuildIf()
{
    return IF;
}

unsigned int TokenBuilder::BuildAs()
{
    return AS;
}

unsigned int TokenBuilder::BuildElse()
{
    return ELSE;
}

unsigned int TokenBuilder::BuildFor()
{
    return FOR;
}

unsigned int TokenBuilder::BuildReturn()
{
    return RETURN;
}

unsigned int TokenBuilder::BuildLessEqual()
{
    return LE;
}

unsigned int TokenBuilder::BuildGreatEqual()
{
    return GE;
}

unsigned int TokenBuilder::BuildEqual()
{
    return EQ;
}

unsigned int TokenBuilder::BuildNotEqual()
{
    return NE;
}

unsigned int TokenBuilder::BuildAnd()
{
    return AND;
}

unsigned int TokenBuilder::BuildOr()
{
    return OR;
}

unsigned int TokenBuilder::BuildComponentwiseMultiply()
{
    return CM;
}

unsigned int TokenBuilder::BuildComponentwiseDivide()
{
    return CD;
}

unsigned int TokenBuilder::BuildLegalChar(unsigned int c)
{
    return c;
}

unsigned int TokenBuilder::BuildNumber(QString str)
{
    parserlval.number = str.toDouble(); return NUMBER;
}

unsigned int TokenBuilder::BuildIdentifier(QString str)
{
    parserlval.text = new QString(str); return IDENTIFIER;
}

void TokenBuilder::BuildStringStart()
{
    stringcontents = new QString();
}

void TokenBuilder::BuildString(QChar c)
{
    stringcontents->append(c);
}

void TokenBuilder::BuildString(QString s)
{
    stringcontents->append(s);
}

unsigned int TokenBuilder::BuildStringFinish()
{
    parserlval.text = stringcontents; return STRING;
}

void TokenBuilder::BuildCommentStart()
{
}

unsigned int TokenBuilder::BuildComment(QString)
{
    return YY_NULL;
}

void TokenBuilder::BuildCommentFinish()
{
}

void TokenBuilder::BuildWhiteSpaceError()
{
}

void TokenBuilder::BuildFileStart(QDir pth)
{
    path_stack.push(pth);
}

void TokenBuilder::BuildFileFinish()
{
    path_stack.pop();
}
