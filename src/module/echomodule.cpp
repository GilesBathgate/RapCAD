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

#include "echomodule.h"
#include "context.h"
#include "textvalue.h"

EchoModule::EchoModule(Reporter& r) : Module(r,"echo"), output(r.output)
{
	addDeprecated(tr("The echo module is deprecated please use 'write' or 'writeln' module instead."));
}

OnceOnly EchoModule::depricateWarning;

Node* EchoModule::evaluate(const Context& ctx) const
{
	if(depricateWarning())
		reporter.reportWarning(tr("'echo' module is deprecated please use 'write' or 'writeln'\n"));

	output << "ECHO: ";
	auto args=ctx.getArguments();

	OnceOnly first;
	for(auto a: args) {
		Value* v=a.second;
		if(!first())
			output << ", ";
		auto* t=dynamic_cast<TextValue*>(v);
		if(t) output << "\"";
		output << v->getValueString();
		if(t) output << "\"";
	}
	output << "\n";

	return nullptr;
}
