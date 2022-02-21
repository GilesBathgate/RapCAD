/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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
#ifndef TOKENREADER_H
#define TOKENREADER_H

#include "abstracttokenbuilder.h"

class TokenReader : public AbstractTokenBuilder
{
public:
	explicit TokenReader(const QString&);
	~TokenReader() override;
	int nextToken() override;
	int getPosition() const override { return 0; }
	int getLineNumber() const override { return 0; }
	void buildIncludeStart() override {}
	void buildIncludeFile(const QString&) override {}
	void buildIncludePath(const QString&) override {}
	void buildIncludeFinish() override {}
	void buildUseStart() override {}
	int buildUse(const QString&) override;
	void buildUseFinish() override {}
	void buildImportStart() override {}
	int buildImport(const QString&) override;
	void buildImportFinish() override {}
	int buildModule() override;
	int buildFunction() override;
	int buildTrue() override;
	int buildFalse() override;
	int buildUndef() override;
	int buildConst() override;
	int buildParam() override;
	int buildIf() override;
	int buildAs() override;
	int buildElse() override;
	int buildFor() override;
	int buildReturn() override;
	int buildLessEqual() override;
	int buildGreatEqual() override;
	int buildEqual() override;
	int buildNotEqual() override;
	int buildAnd() override;
	int buildOr() override;
	int buildComponentwiseMultiply() override;
	int buildComponentwiseDivide() override;
	int buildIncrement() override;
	int buildDecrement() override;
	int buildAddAssign() override;
	int buildSubtractAssign() override;
	int buildCrossProduct() override;
	int buildNamespace() override;
	int buildAppend() override;
	int buildOperator(int c) override;
	int buildLegalChar(int) override;
	int buildByteOrderMark() override;
	int buildIllegalChar(const QString&) override;
	int buildNumber(const QString&) override;
	int buildNumberExp(const QString&) override;
	int buildRational() override;
	int buildRational(const QString&) override;
	int buildIdentifier(const QString&) override;
	void buildStringStart() override {}
	void buildString(QChar) override {}
	void buildString(const QString&) override {}
	int buildStringFinish() override;
	void buildCommentStart() override {}
	void buildComment(const QString&) override {}
	void buildCommentFinish() override {}
	int buildCodeDocStart() override;
	int buildCodeDoc(const QString&) override;
	void buildCodeDoc() override {}
	int buildCodeDocParam(const QString&) override;
	int buildCodeDocFinish() override;
	void buildWhiteSpaceError() override {}
	void buildWhiteSpace() override {}
	void buildNewLine() override {}
	void buildFileStart(QFileInfo) override {}
	void buildFileFinish() override {}
	QString getToken() const override { return QString(); }
	void setParser(union YYSTYPE*) override {}
};

#endif // TOKENREADER_H
