#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QPlainTextEdit>
#include <QWidget>

namespace Ui
{
class SearchWidget;
}

class SearchWidget : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(SearchWidget)
public:
	explicit SearchWidget(QWidget* parent=nullptr);
	~SearchWidget() override;
	void setTextEdit(QPlainTextEdit* value);
public slots:
	void showSearch();
	void showReplace();
private slots:
	void searchUp();
	void searchDown();
	bool replace(bool);
	void replaceAll();
	void searchLineEditTextChanged(const QString&);
private:
	void keyPressEvent(QKeyEvent* event) override;
	void updateSearchCount(bool);
	void updateSearchCountLabelText();
	void setReplaceMode(bool);
	void resetSearchCursor(bool);
	void restartSearch();
	void setupActions();
	void searchCount();
	bool search(bool,bool);
	int currentSearchResult;
	int searchResultCount;
	QPlainTextEdit* textEdit;
	Ui::SearchWidget* ui;
};

#endif // SEARCHWIDGET_H
