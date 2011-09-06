#include "polygonmodule.h"
#include "node/primitivenode.h"
#include "vectorvalue.h"
#include "numbervalue.h"

PolygonModule::PolygonModule() : Module("polygon")
{
}

Node * PolygonModule::evaluate(Context* ctx, QList<Node*>)
{
	VectorValue* pointsVec=dynamic_cast<VectorValue*>(ctx->getArgument(0,"points"));
	VectorValue* linesVec=dynamic_cast<VectorValue*>(ctx->getArgumentDeprecated(1,"lines","paths"));

	QList<Value*> points=pointsVec->getChildren();
	QList<Value*> lines=linesVec->getChildren();

	PrimitiveNode* p=new PrimitiveNode();
	p->createPolygon();

	VectorValue* line;
	line=dynamic_cast<VectorValue*>(lines.at(0));
	if(!line)
		line=linesVec;

	foreach(Value* indexVal,line->getChildren()) {
		NumberValue* indexNum=dynamic_cast<NumberValue*>(indexVal);
		double index = indexNum->getNumber();
		VectorValue* point=dynamic_cast<VectorValue*>(points.at(index));
		Point pt = point->getPoint();
		p->appendVertex(pt);
	}

	return p;
}
