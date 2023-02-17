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
#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include "preferences.h"
#include <QDialog>
#include <QAbstractButton>

namespace Ui
{
class Preferences;
}

class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PreferencesDialog(QWidget* parent = nullptr);
	~PreferencesDialog() override;
private slots:
	void reset(QAbstractButton*);
	void colorButtonPressed(QWidget*);
	void vertexSizeChanged(double);
	void edgeSizeChanged(double);
	void autoSaveOnCompileChanged(int);
	void darkThemeChanged(int);
	void showTooltipsChanged(int);
	void placesChanged(int);
	void bitsChanged(int);
	void precisionType(bool);
	void functionRoundingChanged(bool);
	void outputFormatChanged(bool);
	void volumeChanged();
	void originChanged();
	void appearanceChanged(int);
	void fontChanged(QFont);
	void fontSizeChanged(int);
	void highlightLineChanged(int s);
	void launchCommandChanged(const QString&);
	void launchCommandUpdated();
	void showGCODEButtonChanged(int);
	void translateChanged(int);
	void processingScriptUpdated();
	void indentRadioChanged(bool);
	void indentSpacesChanged(int);
	void threadPoolSizeChanged(int);
	void useCGALAssertionsChanged(int);
	void visibleWhiteSpaceChanged(int);
	void softwareOpenGLChanged(int);
private:
	void accept() override;
	void reject() override;
	void setupWidgets();
	static void setColor(QWidget*,const QColor&);
	void setupButtons();
	void updatePrecision();
	Ui::Preferences* ui;
	Preferences& preferences;
};

#endif // PREFERENCESDIALOG_H
