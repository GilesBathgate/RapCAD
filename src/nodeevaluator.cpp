/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include "nodeevaluator.h"
#include "cgalbuilder.h"

NodeEvaluator::NodeEvaluator()
{
}

void NodeEvaluator::visit(PrimitiveNode* n)
{
	CGALBuilder b(n);
	result=b.build();
}

void NodeEvaluator::visit(OperationNode* op)
{
	QString name=op->getName();
	CGAL::NefPolyhedron3* first=NULL;
	foreach(Node* n, op->getChildren()) {
		n->accept(*this);
		if(!first) {
			first=result;
		} else {
			if(name=="union")
				*first=first->join(*result);
			else if(name=="difference")
				*first=first->difference(*result);
			else if(name=="intersection")
				*first=first->intersection(*result);
		}
	}

	result=first;
}

void NodeEvaluator::visit(TransformationNode*)
{
}

CGAL::NefPolyhedron3* NodeEvaluator::getResult() const
{
	return result;
}
