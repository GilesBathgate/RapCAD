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

#ifndef ABSTRACTTOKENBUILDER_H
#define ABSTRACTTOKENBUILDER_H

#include <QString>
#include <QChar>
#include <QDir>

class AbstractTokenBuilder
{
public:
	virtual ~AbstractTokenBuilder() {}
	virtual void BuildIncludeStart()=0;
	virtual void BuildIncludeFile(QString)=0;
	virtual void BuildIncludePath(QString)=0;
	virtual void BuildIncludeFinish()=0;
	virtual void BuildUseStart()=0;
	virtual unsigned int BuildUse(QString)=0;
	virtual void BuildUseFinish()=0;
	virtual unsigned int BuildModule()=0;
	virtual unsigned int BuildFunction()=0;
	virtual unsigned int BuildTrue()=0;
	virtual unsigned int BuildFalse()=0;
	virtual unsigned int BuildUndef()=0;
	virtual unsigned int BuildConst()=0;
	virtual unsigned int BuildParam()=0;
	virtual unsigned int BuildIf()=0;
	virtual unsigned int BuildAs()=0;
	virtual unsigned int BuildElse()=0;
	virtual unsigned int BuildFor()=0;
	virtual unsigned int BuildReturn()=0;
	virtual unsigned int BuildLessEqual()=0;
	virtual unsigned int BuildGreatEqual()=0;
	virtual unsigned int BuildEqual()=0;
	virtual unsigned int BuildNotEqual()=0;
	virtual unsigned int BuildAnd()=0;
	virtual unsigned int BuildOr()=0;
	virtual unsigned int BuildComponentwiseMultiply()=0;
	virtual unsigned int BuildComponentwiseDivide()=0;
	virtual unsigned int BuildLegalChar(unsigned int)=0;
	virtual unsigned int BuildNumber(QString)=0;
	virtual unsigned int BuildIdentifier(QString)=0;
	virtual void BuildStringStart()=0;
	virtual void BuildString(QChar)=0;
	virtual void BuildString(QString)=0;
	virtual unsigned int BuildStringFinish()=0;
	virtual void BuildCommentStart()=0;
	virtual unsigned int BuildComment(QString)=0;
	virtual void BuildCommentFinish()=0;
	virtual void BuildWhiteSpaceError()=0;
	virtual void BuildFileStart(QDir)=0;
	virtual void BuildFileFinish()=0;
};

#endif // ABSTRACTTOKENBUILDER_H
