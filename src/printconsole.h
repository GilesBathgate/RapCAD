#ifndef PRINTCONSOLE_H
#define PRINTCONSOLE_H

#include <QDialog>

namespace Ui {
	class PrintConsole;
}

class PrintConsole : public QDialog
{
	Q_OBJECT

public:
	explicit PrintConsole(QWidget *parent = 0);
	~PrintConsole();
private:
	QString getStyleString(QString,QString);
	Ui::PrintConsole *ui;
};

#endif // PRINTCONSOLE_H
