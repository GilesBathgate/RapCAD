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

#ifndef TOKENBUILDER_H
#define TOKENBUILDER_H

#include <QString>
#include <QChar>
#include <QStack>
#include <QDir>
#include "abstracttokenbuilder.h"
#include "reporter.h"

class TokenBuilder : public AbstractTokenBuilder
{
public:
	explicit TokenBuilder(const QString&);
	TokenBuilder(Reporter&,const QString&);
	TokenBuilder(Reporter&,QFileInfo);
	~TokenBuilder() override;
	int nextToken() override;
	int getPosition() const override;
	int getLineNumber() const override;
	void buildIncludeStart() override;
	void buildIncludeFile(const QString&) override;
	void buildIncludePath(const QString&) override;
	void buildIncludeFinish() override;
	void buildUseStart() override;
	unsigned int buildUse(const QString&) override;
	void buildUseFinish() override;
	void buildImportStart() override;
	unsigned int buildImport(const QString&) override;
	void buildImportFinish() override;
	unsigned int buildModule() override;
	unsigned int buildFunction() override;
	unsigned int buildTrue() override;
	unsigned int buildFalse() override;
	unsigned int buildUndef() override;
	unsigned int buildConst() override;
	unsigned int buildParam() override;
	unsigned int buildIf() override;
	unsigned int buildAs() override;
	unsigned int buildElse() override;
	unsigned int buildFor() override;
	unsigned int buildReturn() override;
	unsigned int buildLessEqual() override;
	unsigned int buildGreatEqual() override;
	unsigned int buildEqual() override;
	unsigned int buildNotEqual() override;
	unsigned int buildAnd() override;
	unsigned int buildOr() override;
	unsigned int buildComponentwiseMultiply() override;
	unsigned int buildComponentwiseDivide() override;
	unsigned int buildIncrement() override;
	unsigned int buildDecrement() override;
	unsigned int buildAddAssign() override;
	unsigned int buildSubtractAssign() override;
	unsigned int buildCrossProduct() override;
	unsigned int buildNamespace() override;
	unsigned int buildAppend() override;
	unsigned int buildOperator(unsigned int c) override;
	unsigned int buildLegalChar(unsigned int) override;
	unsigned int buildIllegalChar(const QString&) override;
	unsigned int buildNumber(const QString&) override;
	unsigned int buildNumberExp(const QString&) override;
	unsigned int buildRational() override;
	unsigned int buildRational(const QString&) override;
	unsigned int buildIdentifier(const QString&) override;
	void buildStringStart() override;
	void buildString(QChar) override;
	void buildString(const QString&) override;
	unsigned int buildStringFinish() override;
	void buildCommentStart() override;
	void buildComment(const QString&) override;
	void buildCommentFinish() override;
	unsigned int buildCodeDocStart() override;
	unsigned int buildCodeDoc(const QString&) override;
	void buildCodeDoc() override;
	unsigned int buildCodeDocParam(const QString&) override;
	unsigned int buildCodeDocFinish() override;
	void buildWhiteSpaceError() override;
	void buildWhiteSpace() override;
	void buildNewLine() override;
	void buildFileStart(QDir) override;
	void buildFileFinish() override;
	QString getToken() const override;
private:
	TokenBuilder();
	QString token;
	QString* stringcontents;
	QString filename;
	QString filepath;
	QStack<QDir> path_stack;
	int position;
};

#endif // TOKENBUILDER_H
