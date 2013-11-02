/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2013 Giles Bathgate
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

EchoModule::EchoModule(QTextStream& s) : Module("echo"), output(s)
{
}

bool EchoModule::depricateWarning=true;

Node* EchoModule::evaluate(Context* ctx)
{
	if(depricateWarning) {
		output << "Warning: 'echo' module is deprecated please use 'write' or 'writeln'\n";
		depricateWarning=false;
	}
	output << "ECHO: ";
	QList<Value*> args=ctx->getArguments();
	for(int i=0; i<args.size(); i++) {
		if(i>0)
			output << ", ";
		Value* a=args.at(i);
		TextValue* t=dynamic_cast<TextValue*>(a);
		if(t) output << "\"";
		output << a->getValueString();
		if(t) output << "\"";
	}
	output << "\n";

	return NULL;
}
