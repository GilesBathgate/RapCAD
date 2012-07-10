#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
AboutDialog::AboutDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);
	this->setFixedSize(this->width(),this->height());

	QIcon rapcadIcon(":/icons/rapcad-16x16.png");
	this->setWindowIcon(rapcadIcon);

	QPixmap rapcadPicture(":/icons/rapcad-100x100.png");
	ui->picture->setPixmap(rapcadPicture);

	QString v=TOSTRING(RAPCAD_VERSION);
	ui->name->setText("RapCAD Version " + v);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
