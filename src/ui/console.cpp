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

#include "console.h"

Console::Console(QWidget* parent) :
	QPlainTextEdit(parent),
	promptLength(0),
	promptBlock(0),
	historyPos(0)
{
	setUndoRedoEnabled(false);
}

void Console::displayPrompt()
{
	QTextCursor cursor=textCursor();
	if(cursor.columnNumber()>0)
		cursor.insertText("\n");
	cursor.insertText(prompt);
	cursor.movePosition(QTextCursor::EndOfLine);
	setTextCursor(cursor);

	promptBlock=cursor.blockNumber();
}

void Console::keyPressEvent(QKeyEvent* e)
{
	switch(e->key()) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
			if(inPromptBlock())
				handleReturn();
			return;
		case Qt::Key_Left:
		case Qt::Key_Backspace:
			if(handleBackspace()||!inPromptBlock())
				return;
			break;
		case Qt::Key_Down:
			handleHistory(historyPos+1);
			return;
		case Qt::Key_Up:
			handleHistory(historyPos-1);
			return;
		case Qt::Key_Right:
		case Qt::Key_Home:
		case Qt::Key_End:
			break;
		default:
			if(!inPromptBlock()) {
				moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
			}
	}

	QPlainTextEdit::keyPressEvent(e);
}

bool Console::inPromptBlock()
{
	QTextCursor cursor=textCursor();
	int block=cursor.blockNumber();
	int column=cursor.columnNumber();
	return (block > promptBlock) || ((block == promptBlock) && (column >= promptLength));
}

bool Console::handleBackspace()
{
	QTextCursor cursor=textCursor();
	int column=cursor.columnNumber();
	int block=cursor.blockNumber();
	if(block == promptBlock && column == promptLength)
		return true;
	return false;
}

void Console::setPrompt(const QString& p)
{
	prompt=p;
	promptLength=prompt.length();
	displayPrompt();
}

void Console::handleReturn()
{
	QString command=getCommand();
	if(!command.trimmed().isEmpty()) {
		commands.append(command);
		historyPos=commands.count();
	}
	moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
	insertPlainText("\n");
	emit execCommand(command);
	displayPrompt();
}

void Console::handleHistory(int pos)
{
	int c=commands.count();
	if(pos<0||pos>c) return;

	QTextCursor cursor=textCursor();
	cursor.select(QTextCursor::LineUnderCursor);
	cursor.removeSelectedText();
	displayPrompt();
	historyPos=pos;
	if(pos==c) return;

	insertPlainText(commands.at(pos));
}

QString Console::getCommand() const
{
	QTextCursor cursor=textCursor();
	cursor.movePosition(QTextCursor::StartOfLine);
	cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, promptLength);
	cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
	QString command=cursor.selectedText();
	cursor.clearSelection();
	return command;
}
