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

#ifndef MODULEIMPORT_H
#define MODULEIMPORT_H

#include <QString>
#include <QList>
#include "declaration.h"
#include "parameter.h"

class ModuleImport : public Declaration
{
public:
	ModuleImport();
	~ModuleImport() override;
	void setImport(const QString&);
	QString getImport() const;
	void setName(const QString&);
	QString getName() const;
	void accept(TreeVisitor&) override;
	void setParameters(const QList<Parameter*>&);
	QList<Parameter*> getParameters() const;
private:
	QString import;
	QString name;
	QList<Parameter*> parameters;
};

#endif // MODULEIMPORT_H
