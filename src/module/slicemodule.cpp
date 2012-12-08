/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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

#include "slicemodule.h"
#include "node/slicenode.h"
#include "numbervalue.h"

SliceModule::SliceModule() : Module("slice")
{
}

Node* SliceModule::evaluate(Context* ctx)
{
	double h=0.0;
	NumberValue* height=dynamic_cast<NumberValue*>(ctx->getArgument(0,"height"));
	if(height)
		h=height->getNumber();

	SliceNode* d = new SliceNode();
	d->setHeight(h);
	d->setChildren(ctx->getInputNodes());
	return d;
}