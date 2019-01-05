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

#include "textmodule.h"
#include "textvalue.h"
#include "numbervalue.h"
#include "qpathtextbuilder.h"
#include "node/primitivenode.h"
#include "context.h"

TextModule::TextModule(Reporter& r) : Module(r, "text")
{
	addDescription(tr("Generates text."));
	addParameter("text",tr("The text to generate"));
	addParameter("font",tr("The font of the text"));
	addParameter("size",tr("The size of the text"));
}

Node* TextModule::evaluate(const Context& ctx) const
{
	auto* textVal=dynamic_cast<TextValue*>(getParameterArgument(ctx,0));
	if(!textVal)
		return nullptr;

	QString family;
	auto* fontVal=dynamic_cast<TextValue*>(getParameterArgument(ctx,1));
	if(fontVal)
		family=fontVal->getValueString();

	int size=12;
	auto* sizeVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,2));
	if(sizeVal)
		size=sizeVal->toInteger();

	QPathTextBuilder tb(reporter);
	tb.setText(textVal->getValueString());
	tb.setFamily(family);
	tb.setSize(size);

	auto* pn=new PrimitiveNode(reporter);
	Primitive* p=tb.buildPrimitive();
	p->setType(Primitive::Surface);
	p->setSanitized(false);
	pn->setPrimitive(p);
	pn->setChildren(ctx.getInputNodes());
	return pn;
}
