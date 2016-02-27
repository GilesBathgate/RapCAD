/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "syntaxhighlighter.h"

class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	CodeEditor(QWidget* parent=0);
	~CodeEditor();

	void lineNumberAreaPaintEvent(QPaintEvent*);
	int lineNumberAreaWidth();

	void stopHighlighting();
	void setFileName(const QString&);
	QString getFileName();
	bool saveFile();
	bool saveAsFile();
	bool loadFile(const QString& f);
	bool openFile();
signals:
	void fileNameChanged(QString fileName);
protected:
	void resizeEvent(QResizeEvent* event);

private slots:
	void updateLineNumberAreaWidth(int);
	void updateLineNumberArea(const QRect&, int);

private:
	SyntaxHighlighter* highlighter;
	QWidget* lineNumberArea;
	QString fileName;
};

#endif // CODEEDITOR_H
