#include "searchwidget.h"
#include "ui_searchwidget.h"

SearchWidget::SearchWidget(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::SearchWidget)
{
	ui->setupUi(this);
	textEdit=nullptr;
	currentSearchResult=0;
	searchResultCount=0;
	setupActions();
}

void SearchWidget::setupActions()
{
	connect(ui->closeButton,&QPushButton::clicked,this,&SearchWidget::hide);
	connect(ui->searchUpButton,&QPushButton::clicked,this,&SearchWidget::searchUp);
	connect(ui->searchDownButton,&QPushButton::clicked,this,&SearchWidget::searchDown);
	connect(ui->replaceToggleButton,&QPushButton::toggled,this,&SearchWidget::setReplaceMode);
	connect(ui->searchLineEdit,&QLineEdit::textEdited,this,&SearchWidget::searchLineEditTextChanged);
	connect(ui->replaceButton,&QPushButton::clicked,this,&SearchWidget::replace);
	connect(ui->replaceAllButton,&QPushButton::clicked,this,&SearchWidget::replaceAll);
}

SearchWidget::~SearchWidget()
{
	delete ui;
}

void SearchWidget::keyPressEvent(QKeyEvent* event)
{
	switch(event->key()) {
		case Qt::Key_Escape:
			hide();
			break;
		case Qt::Key_F3:
		case Qt::Key_Return:
			if(event->modifiers()==Qt::ShiftModifier)
				searchUp();
			else
				searchDown();
			break;
		case Qt::Key_Up:
			searchUp();
			break;
		case Qt::Key_Down:
			searchDown();
			break;
	}

	QWidget::keyPressEvent(event);
}

void SearchWidget::setReplaceMode(bool enabled)
{
	ui->replaceToggleButton->setChecked(enabled);
	ui->replaceLabel->setVisible(enabled);
	ui->replaceLineEdit->setVisible(enabled);
	ui->replaceButton->setVisible(enabled);
	ui->replaceAllButton->setVisible(enabled);
	if(!enabled||ui->searchLineEdit->text().isEmpty())
		ui->searchLineEdit->setFocus();
	else
		ui->replaceLineEdit->setFocus();
}

void SearchWidget::searchCount()
{
	QTextCursor cursor=textEdit->textCursor();
	textEdit->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);

	searchResultCount=0;
	currentSearchResult=0;

	while(search(true,false))
		++searchResultCount;

	updateSearchCountLabelText();
	textEdit->setTextCursor(cursor);
}

bool SearchWidget::search(bool searchDown,bool allowWrap)
{
	const QString text=ui->searchLineEdit->text();

	if(text.isEmpty()) return false;

	QFlags<QTextDocument::FindFlag> options;

	if(!searchDown)
		options|=QTextDocument::FindBackward;

	const bool caseSensitive=ui->matchCaseSensitiveButton->isChecked();
	if(caseSensitive)
		options |= QTextDocument::FindCaseSensitively;

	bool found=textEdit->find(text,options);
	if(found) {
		updateSearchCount(searchDown);
	} else if(allowWrap&&ui->wrapAroundButton->isChecked()) {
		resetSearchCursor(searchDown);

		found=textEdit->find(text,options);
		if(found)
			updateSearchCount(searchDown);
	}

	return found;
}

void SearchWidget::replaceAll()
{
	if(textEdit->isReadOnly())
		return;

	textEdit->moveCursor(QTextCursor::Start);
	while(search(true, false) && replace(true));
}

bool SearchWidget::replace(bool forAll)
{
	if(textEdit->isReadOnly())
		return false;

	QTextCursor cursor=textEdit->textCursor();

	if(!forAll&&cursor.selectedText().isEmpty())
		return false;

	bool blocked=textEdit->blockSignals(true);
	cursor.insertText(ui->replaceLineEdit->text());
	textEdit->blockSignals(blocked);

	if(!forAll) {
		const int position=cursor.position();

		if(!search(true,false)) {
			cursor.setPosition(position);
			textEdit->setTextCursor(cursor);
		}
	}

	return true;
}

void SearchWidget::resetSearchCursor(bool searchDown)
{
	currentSearchResult=searchDown?0:searchResultCount+1;
	textEdit->moveCursor(searchDown?QTextCursor::Start:QTextCursor::End);
}

void SearchWidget::setTextEdit(QPlainTextEdit* value)
{
	if(textEdit)
		disconnect(textEdit,&QPlainTextEdit::textChanged,this,&SearchWidget::hide);
	textEdit=value;
	restartSearch();
	connect(textEdit,&QPlainTextEdit::textChanged,this,&SearchWidget::hide);
}

void SearchWidget::searchLineEditTextChanged(const QString&)
{
	restartSearch();
}

void SearchWidget::restartSearch()
{
	searchCount();
	resetSearchCursor(true);
	search(true,false);
}

void SearchWidget::updateSearchCount(bool searchDown)
{
	const int result=searchDown?++currentSearchResult:--currentSearchResult;
	currentSearchResult=std::min(result,searchResultCount);

	updateSearchCountLabelText();
}

void SearchWidget::updateSearchCountLabelText()
{
	ui->searchCountLabel->setEnabled(true);
	ui->searchCountLabel->setText(QString("%1/%2").arg(
			currentSearchResult==0?QChar('-'):QString::number(currentSearchResult),
			searchResultCount==0?QChar('-'):QString::number(searchResultCount)));
}

void SearchWidget::showSearch()
{
	setReplaceMode(false);
	restartSearch();
	show();
}

void SearchWidget::showReplace()
{
	setReplaceMode(true);
	restartSearch();
	show();
}

void SearchWidget::searchUp()
{
	search(false,true);
}

void SearchWidget::searchDown()
{
	search(true,true);
}
