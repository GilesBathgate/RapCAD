/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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

#include "strategy.h"
#include "invocation.h"

Strategy::Strategy(Reporter& r) :
	reporter(r),
	output(r.output)
{
}

Callback* Strategy::addCallback(const QString& name, Script& s, const QList<Argument*>& args)
{
	auto* c=new Callback();
	auto* l=new Invocation();
	if(args.length()>0)
		l->setArguments(args);
	l->setName(name);
	c->setExpression(l);
	s.appendDeclaration(c);

	return c;
}
