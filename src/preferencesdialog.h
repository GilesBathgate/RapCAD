#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QSignalMapper>

namespace Ui
{
class Preferences;
}

class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PreferencesDialog(QWidget* parent = 0);
	~PreferencesDialog();
private slots:
	void colorButtonPressed(QWidget*);
private:
	Ui::Preferences* ui;
	void setupColors();
	void setColor(QWidget*,QColor);
	void setupButtons();
	QSignalMapper* signalMapper;
};

#endif // PREFERENCESDIALOG_H
