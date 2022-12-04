/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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
#include "assertmodule.h"
#include "assertexception.h"

AssertModule::AssertModule(Reporter& r) : Module(r,"assert")
{
	addDescription(tr("Checks the expected value and stops execution when false"));
	addParameter("condition","bool",tr("A value that is expected to be true"));
	addParameter("message","str",tr("A message that is displayed when the assertion fails"));
}

Node* AssertModule::evaluate(const Context& ctx) const
{
	auto* expectedVal=getParameterArgument<Value>(ctx,0);
	auto* messageVal=getParameterArgument<Value>(ctx,1);

	QString message;
	if(messageVal)
		message=messageVal->getValueString();
	else
		message=tr("Assertion failed");

	if(expectedVal&&expectedVal->isFalse())
		throw AssertException(message);

	return nullptr;
}
