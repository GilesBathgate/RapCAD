#include "squaremodule.h"
#include "vectorvalue.h"

SquareModule::SquareModule() : PrimitiveModule("square")
{
}

Node* SquareModule::evaluate(Context* ctx,QList<Node*>)
{
	VectorValue* size=ctx->getArgument(0,"size")->toVector(2);
	Value* centerValue=ctx->getArgument(1,"center");
	double center=false;
	if(centerValue)
		center = centerValue->isTrue();

	double x=0,y=0;
	if(size) {
		Point p = size->getPoint();
		p.getXY(x,y);
	}

	PrimitiveNode* p=new PrimitiveNode();
	double x1, x2, y1, y2;
	if(center) {
		x1 = -x/2;
		x2 = +x/2;
		y1 = -y/2;
		y2 = +y/2;
	} else {
		x1 = y1 = 0;
		x2 = x;
		y2 = y;
	}

	p->createPolygon();
	p->appendVertex(x1, y1, 0);
	p->appendVertex(x2, y1, 0);
	p->appendVertex(x2, y2, 0);
	p->appendVertex(x1, y2, 0);

	return p;
}
