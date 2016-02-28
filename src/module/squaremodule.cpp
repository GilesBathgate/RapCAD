#include "squaremodule.h"
#include "vectorvalue.h"

SquareModule::SquareModule(Reporter* r) : PrimitiveModule(r,"square")
{
	addDescription(tr("Constructs a square or rectangle. It will be placed in the first quadrant."));
	addParameter("size",tr("The size of the square"));
	addParameter("center",tr("Specifies whether to center the square on the xy plane"));
}

Node* SquareModule::evaluate(Context* ctx)
{
	Value* sizeVal=getParameterArgument(ctx,0);
	Value* centerVal=getParameterArgument(ctx,1);
	bool center=false;
	if(centerVal)
		center = centerVal->isTrue();

	decimal x=1.0,y=1.0;
	if(sizeVal) {
		VectorValue* size=sizeVal->toVector(2);
		Point p = size->getPoint();
		p.getXY(x,y);
	}

	PrimitiveNode* p=new PrimitiveNode(reporter);
	p->setChildren(ctx->getInputNodes());

	Polygon* pg=p->createPolygon();
	p->createVertex(0, 0, 0);
	p->createVertex(x, 0, 0);
	p->createVertex(x, y, 0);
	p->createVertex(0, y, 0);
	pg->append(0);
	pg->append(1);
	pg->append(2);
	pg->append(3);

	if(center) {
		AlignNode* n=new AlignNode();
		n->setCenter(true);
		n->addChild(p);
		return n;
	}

	return p;
}
