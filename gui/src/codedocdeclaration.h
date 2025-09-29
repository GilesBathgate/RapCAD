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
#ifndef CODEDOCDECLARATION_H
#define CODEDOCDECLARATION_H

#include "declaration.h"
#include "codedocparam.h"
#include <QList>

class CodeDocDeclaration : public Declaration
{
	Q_DISABLE_COPY_MOVE(CodeDocDeclaration)
public:
	CodeDocDeclaration() = default;
	~CodeDocDeclaration() override=default;

	const QList<CodeDocParam*>& getParameters() const;
	void setParameters(const QList<CodeDocParam*>&);

	void accept(TreeVisitor&) override;
private:
	QList<CodeDocParam*> parameters;
};

#endif // CODEDOCDECLARATION_H
