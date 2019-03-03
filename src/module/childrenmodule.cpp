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

#include "childrenmodule.h"
#include "context.h"
#include "node/childrennode.h"
#include "vectorvalue.h"
#include "numbervalue.h"

ChildrenModule::ChildrenModule(Reporter& r) : Module(r,"children")
{
	addDescription(tr("Provides access to children as passed to a module."));
	addParameter("index",tr("A list of indices which determines which children to use."));
}

Node* ChildrenModule::evaluate(const Context& ctx) const
{
	auto* n=new ChildrenNode();

	Value* val=getParameterArgument(ctx,0);
	auto* vecVal=dynamic_cast<VectorValue*>(val);
	if(vecVal) {
		for(Value* v: vecVal->getChildren()) {
			auto* num=dynamic_cast<NumberValue*>(v);
			if(num)
				n->addIndex(num->toInteger());
		}
	}
	auto* numVal=dynamic_cast<NumberValue*>(val);
	if(numVal) {
		n->addIndex(numVal->toInteger());
	}

	n->setChildren(ctx.lookupChildren());
	return n;
}


