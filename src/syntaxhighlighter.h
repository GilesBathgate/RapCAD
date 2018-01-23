/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2018 Giles Bathgate
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

#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "abstracttokenbuilder.h"

class SyntaxHighlighter : public QSyntaxHighlighter, private AbstractTokenBuilder
{
	Q_OBJECT
public:
	explicit SyntaxHighlighter(QTextDocument* parent = nullptr);
	void stop();
protected:
	void highlightBlock(const QString& text) override;
private:
	enum Blockstate_e {
		Initial=-1,
		Comment,
		CodeDoc
	};

	int nextToken() override;
	int getPosition() const override;
	int getLineNumber() const override;
	void buildIncludeStart() override;
	void buildIncludeFile(QString) override;
	void buildIncludePath(QString) override;
	void buildIncludeFinish() override;
	void buildUseStart() override;
	unsigned int buildUse(QString) override;
	void buildUseFinish() override;
	void buildImportStart() override;
	unsigned int buildImport(QString) override;
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
	unsigned int buildAssign() override;
	unsigned int buildAdd() override;
	unsigned int buildSubtract() override;
	unsigned int buildTernaryCondition() override;
	unsigned int buildTernaryAlternate() override;
	unsigned int buildNot() override;
	unsigned int buildMultiply() override;
	unsigned int buildDivide() override;
	unsigned int buildLength() override;
	unsigned int buildModulus() override;
	unsigned int buildConcatenate() override;
	unsigned int buildAppend() override;
	unsigned int buildLegalChar(unsigned int) override;
	unsigned int buildIllegalChar() override;
	unsigned int buildNumber(QString) override;
	unsigned int buildNumberExp(QString) override;
	unsigned int buildRational() override;
	unsigned int buildRational(QString) override;
	unsigned int buildIdentifier(QString) override;
	void buildStringStart() override;
	void buildString(QChar) override;
	void buildString(QString) override;
	unsigned int buildStringFinish() override;
	void buildCommentStart() override;
	void buildComment(QString) override;
	void buildCommentFinish() override;
	unsigned int buildCodeDocStart() override;
	unsigned int buildCodeDoc(QString) override;
	void buildCodeDoc() override;
	unsigned int buildCodeDocParam(QString) override;
	unsigned int buildCodeDocFinish() override;
	void buildWhiteSpaceError() override;
	void buildWhiteSpace() override;
	void buildNewLine() override;
	void buildFileStart(QDir) override;
	void buildFileFinish() override;
	QString getToken() const override;

	QTextCharFormat keywordFormat;
	QTextCharFormat	numberFormat;
	QTextCharFormat stringFormat;
	QTextCharFormat errorFormat;
	QTextCharFormat operatorFormat;
	QTextCharFormat codeDocFormat;
	QTextCharFormat codeDocParamFormat;
	int startIndex;
};
#endif // SYNTAXHIGHLIGHTER_H
