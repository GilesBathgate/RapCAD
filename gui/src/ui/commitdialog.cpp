#include "commitdialog.h"
#include "ui_commitdialog.h"

CommitDialog::CommitDialog(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::CommitDialog)
{
	ui->setupUi(this);
}

CommitDialog::~CommitDialog()
{
	delete ui;
}

QString CommitDialog::getCommitMessage()
{
	return ui->plainTextEdit->toPlainText();
}
