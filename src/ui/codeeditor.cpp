/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#include "codeeditor.h"
#include "linenumberarea.h"
#include "mainwindow.h"
#include "preferences.h"
#include <QApplication>
#include <QFileDialog>
#include <QPainter>
#include <QTextBlock>
#include <QTextDocumentWriter>
#include <QToolTip>
#include <QtGlobal>

CodeEditor::CodeEditor(QWidget* parent) :
	QPlainTextEdit(parent),
	showTooltips(true),
	highlightLine(false)
{
	preferencesUpdated();
	highlighter = new SyntaxHighlighter(document());
	lineNumberArea = new LineNumberArea(this);

	connect(this,&CodeEditor::blockCountChanged,this,&CodeEditor::updateLineNumberAreaWidth);
	connect(this,&CodeEditor::updateRequest,this,&CodeEditor::updateLineNumberArea);
	connect(this,&CodeEditor::cursorPositionChanged,this,&CodeEditor::highlightCurrentLine);

	updateLineNumberAreaWidth(0);
}

CodeEditor::~CodeEditor()
{
	delete highlighter;
}

int CodeEditor::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());
	while(max >= 10) {
		max /= 10;
		++digits;
	}

#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
	const int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
#else
	const int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
#endif

	return space;
}

void CodeEditor::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;
	if(highlightLine&&!isReadOnly()) {
		QTextEdit::ExtraSelection selection;
		selection.format.setBackground(QColor(240,240,240));
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}
	setExtraSelections(extraSelections);
}

void CodeEditor::setFileName(const QString& f)
{
	fileName=f;
	document()->setModified(false);
	emit fileNameChanged(f);
}

QString CodeEditor::getFileName() const
{
	return fileName;
}

bool CodeEditor::saveFile()
{
	if(fileName.isEmpty())
		return saveAsFile();

	QTextDocumentWriter writer(fileName);
	writer.setFormat("plaintext");
	const bool success = writer.write(document());
	if(success)
		document()->setModified(false);

	return success;
}

bool CodeEditor::saveAsFile()
{
	const QString& fileName=MainWindow::getSaveFileName(this, tr("Save as..."),
		QString(),tr("RapCAD Files (*.rcad);;All Files (*)"),"rcad");
	if(fileName.isEmpty())
		return false;
	setFileName(fileName);
	return saveFile();
}

bool CodeEditor::loadFile(const QString& f)
{
	if(!QFile::exists(f))
		return false;
	QFile file(f);
	if(!file.open(QFile::ReadOnly))
		return false;

	const QByteArray& data=file.readAll();
	const QString& str=QString::fromLocal8Bit(data);
	setPlainText(str);

	setFileName(f);
	return true;
}

bool CodeEditor::openFile()
{
	const QString& fileName=QFileDialog::getOpenFileName(this, tr("Open File..."),
		QString(),tr("RapCAD Files (*.rcad);;All Files (*)"));
	if(!fileName.isEmpty())
		loadFile(fileName);
	else
		return false;

	return true;
}

void CodeEditor::preferencesUpdated()
{
	auto& p=Preferences::getInstance();
	QFont font=p.getEditorFont();
	font.setFixedPitch(true);
	setFont(font);
	showTooltips = p.getShowTooltips();
	highlightLine = p.getHighlightLine();
	highlightCurrentLine();
	setVisibleWhiteSpace(p.getVisibleWhiteSpace());
}

void CodeEditor::setVisibleWhiteSpace(bool enabled)
{
	QTextOption option=document()->defaultTextOption();
	if(enabled){
		option.setFlags(option.flags()|QTextOption::ShowTabsAndSpaces);
	} else {
		option.setFlags(option.flags()&(~QTextOption::ShowTabsAndSpaces));
	}
	document()->setDefaultTextOption(option);
}

void CodeEditor::setModuleNames(const QHash<QString,Module*>& names)
{
	moduleNames = names;
	highlighter->setModuleNames(names);
}

void CodeEditor::updateLineNumberAreaWidth(int)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect& rect, int dy)
{
	if(dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if(rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}

void CodeEditor::keyPressEvent(QKeyEvent* e)
{
	if(e->key()==Qt::Key_Tab) {
		if(e->modifiers()==Qt::ControlModifier)
			return decreaseSelectionIndent();
		if(textCursor().hasSelection())
			return increaseSelectionIndent();
	}

	return QPlainTextEdit::keyPressEvent(e);
}

int CodeEditor::getSelectionBlockCount()
{
	QTextCursor cursor=textCursor();
	if(!cursor.hasSelection())
		return 0;

	const int finish=cursor.blockNumber();
	cursor.setPosition(cursor.anchor());
	const int start=cursor.blockNumber();

	return std::abs(finish-start);
}

void CodeEditor::increaseSelectionIndent()
{
	const int blockCount=getSelectionBlockCount();

	QTextCursor cursor=textCursor();
	cursor.setPosition(std::min(cursor.anchor(),cursor.position()));

	cursor.beginEditBlock();
	auto& p=Preferences::getInstance();
	const QString& indent=p.getIndent();
	for(auto i=0; i<=blockCount; ++i) {
		cursor.movePosition(QTextCursor::StartOfBlock);
		QTextCursor current(cursor);
		current.movePosition(QTextCursor::EndOfBlock,QTextCursor::KeepAnchor);
		if(current.hasSelection())
			cursor.insertText(indent);
		cursor.movePosition(QTextCursor::NextBlock);
	}
	cursor.endEditBlock();
}

void CodeEditor::decreaseSelectionIndent()
{
	const int blockCount=getSelectionBlockCount();

	QTextCursor cursor=textCursor();
	cursor.setPosition(std::min(cursor.anchor(),cursor.position()));

	cursor.beginEditBlock();
	auto& p=Preferences::getInstance();
	const QString& indent=p.getIndent();
	for(auto i=0; i<=blockCount; ++i) {
		cursor.movePosition(QTextCursor::StartOfBlock);
		QTextCursor current(cursor);
		current.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
		if(current.selectedText()==indent)
			current.removeSelectedText();
		cursor.movePosition(QTextCursor::NextBlock);
	}
	cursor.endEditBlock();
}

void CodeEditor::resizeEvent(QResizeEvent* e)
{
	QPlainTextEdit::resizeEvent(e);

	const QRect& cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

bool CodeEditor::event(QEvent* event)
{
	if(!showTooltips)
		return QPlainTextEdit::event(event);

	if(event->type()==QEvent::ToolTip) {
		auto* helpEvent = dynamic_cast<QHelpEvent*>(event);
		QTextCursor cursor = cursorForPosition(helpEvent->pos());
		cursor.select(QTextCursor::WordUnderCursor);
		Module* m = moduleNames.value(cursor.selectedText());
		if(m) {
			const QRect& r=cursorRect(cursor);
			QToolTip::showText(mapToGlobal(QPoint(r.x(),r.y())), m->getDescription());
		} else {
			QToolTip::hideText();
		}
		return true;
	}
	if(event->type()==QEvent::KeyPress) {
		QToolTip::hideText();
	}
	return QPlainTextEdit::event(event);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event)
{
	const auto& p = QApplication::palette();

	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), p.color(QPalette::Window));

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	const int currentBlock = textCursor().block().blockNumber();
	int top = int(blockBoundingGeometry(block).translated(contentOffset()).top());
	int bottom = top + int(blockBoundingRect(block).height());

	const bool readOnly=isReadOnly();
	while(block.isValid() && top <= event->rect().bottom()) {
		if(block.isVisible() && bottom >= event->rect().top()) {
			const QString& number = QString::number(blockNumber + 1);
			if(!readOnly&&blockNumber==currentBlock)
				painter.setPen(Qt::black);
			else
				painter.setPen(Qt::darkGray);
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
							 Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + int(blockBoundingRect(block).height());
		++blockNumber;
	}
}
