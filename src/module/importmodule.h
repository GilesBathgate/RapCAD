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

#ifndef IMPORTMODULE_H
#define IMPORTMODULE_H

#include "module.h"
#include <QString>

class ImportModule : public Module
{
	Q_DECLARE_TR_FUNCTIONS(ImportModule)
public:
	explicit ImportModule(Reporter&);
	Node* evaluate(const Context&) const override;
	void setImport(const QString&);
private:
	QString import;
};

#endif // IMPORTMODULE_H
