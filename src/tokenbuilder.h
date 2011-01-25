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

#ifndef TOKENBUILDER_H
#define TOKENBUILDER_H

#include <QString>
#include <QChar>
#include <QStack>
#include <QDir>
#include "abstracttokenbuilder.h"

class TokenBuilder : public AbstractTokenBuilder
{
public:
	TokenBuilder();
	void buildIncludeStart();
	void buildIncludeFile(QString);
	void buildIncludePath(QString);
	void buildIncludeFinish();
	void buildUseStart();
	unsigned int buildUse(QString);
	void buildUseFinish();
	void buildImportStart();
	unsigned int buildImport(QString);
	void buildImportFinish();
	unsigned int buildModule();
	unsigned int buildFunction();
	unsigned int buildTrue();
	unsigned int buildFalse();
	unsigned int buildUndef();
	unsigned int buildConst();
	unsigned int buildParam();
	unsigned int buildIf();
	unsigned int buildAs();
	unsigned int buildElse();
	unsigned int buildFor();
	unsigned int buildReturn();
	unsigned int buildLessEqual();
	unsigned int buildGreatEqual();
	unsigned int buildEqual();
	unsigned int buildNotEqual();
	unsigned int buildAnd();
	unsigned int buildOr();
	unsigned int buildComponentwiseMultiply();
	unsigned int buildComponentwiseDivide();
	unsigned int buildOuterProduct();
	unsigned int buildNamespace();
	unsigned int buildLegalChar(unsigned int);
	unsigned int buildNumber(QString);
	unsigned int buildIdentifier(QString);
	void buildStringStart();
	void buildString(QChar);
	void buildString(QString);
	unsigned int buildStringFinish();
	void buildCommentStart();
	unsigned int buildComment(QString);
	void buildCommentFinish();
	void buildWhiteSpaceError();
	void buildFileStart(QDir);
	void buildFileFinish();
private:
	QString* stringcontents;
	QString filename;
	QString filepath;
	QStack<QDir> path_stack;
};

#endif // TOKENBUILDER_H
