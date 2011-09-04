#ifndef SAVEITEMSDIALOG_H
#define SAVEITEMSDIALOG_H

#include <QDialog>

namespace Ui {
	class SaveItemsDialog;
}

class SaveItemsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SaveItemsDialog(QWidget *parent,bool,QList<QString>);
	~SaveItemsDialog();

	QList<QString> getItemsToSave();

private slots:
	void collectItemsToSave();
	void discardAll();
	void updateSaveButton();
private:
	Ui::SaveItemsDialog *ui;
	QList<QString> itemsToSave;
};

#endif // SAVEITEMSDIALOG_H
