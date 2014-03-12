#include "squaremodule.h"
#include "vectorvalue.h"

SquareModule::SquareModule() : PrimitiveModule("square")
{
	addParameter("size");
	addParameter("center");
}

Node* SquareModule::evaluate(Context* ctx)
{
	Value* sizeVal=getParameterArgument(ctx,0);
	Value* centerVal=getParameterArgument(ctx,1);
	decimal center=false;
	if(centerVal)
		center = centerVal->isTrue();

	decimal x=1.0,y=1.0;
	if(sizeVal) {
		VectorValue* size=sizeVal->toVector(2);
		Point p = size->getPoint();
		p.getXY(x,y);
	}

	PrimitiveNode* p=new PrimitiveNode();
	decimal x1, x2, y1, y2;
	x1 = y1 = 0;
	x2 = x;
	y2 = y;

	p->createPolygon();
	p->appendVertex(x1, y1, 0);
	p->appendVertex(x2, y1, 0);
	p->appendVertex(x2, y2, 0);
	p->appendVertex(x1, y2, 0);

	if(center) {
		AlignNode* n=new AlignNode();
		n->setCenter(true);
		n->addChild(p);
		return n;
	}

	return p;
}
