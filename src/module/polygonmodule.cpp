#include "polygonmodule.h"
#include "node/primitivenode.h"
#include "vectorvalue.h"
#include "numbervalue.h"

PolygonModule::PolygonModule() : Module("polygon")
{
	addParameter("points");
	addParameter("lines");
}

Node* PolygonModule::evaluate(Context* ctx)
{
	VectorValue* pointsVec=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	VectorValue* linesVec=dynamic_cast<VectorValue*>(ctx->getArgumentDeprecated(1,"lines","paths"));

	PrimitiveNode* p=new PrimitiveNode();

	if(!linesVec||!pointsVec)
		return p;

	QList<Value*> points=pointsVec->getChildren();
	QList<Value*> lines=linesVec->getChildren();

	//This is to remove the need for double vector syntax in the lines argument
	// e.g. lines=[[0,1,2,3]] can just be writtern as lines=[0,1,2,3]
	VectorValue* line=NULL;
	if(lines.count()>1)
		line=dynamic_cast<VectorValue*>(lines.at(0));
	if(!line)
		line=linesVec;
	else
		return p;

	p->createPolygon();
	foreach(Value* indexVal,line->getChildren()) {
		NumberValue* indexNum=dynamic_cast<NumberValue*>(indexVal);
		double index = indexNum->getNumber();
		VectorValue* point=dynamic_cast<VectorValue*>(points.at(index));
		Point pt = point->getPoint();
		p->appendVertex(pt);
	}

	return p;
}
