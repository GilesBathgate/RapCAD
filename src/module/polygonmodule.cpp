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

	if(!pointsVec)
		return p;

	QList<Value*> points=pointsVec->getChildren();

	foreach(Value* point, points) {
		VectorValue* pointVec=dynamic_cast<VectorValue*>(point);
		if(pointVec) {
			Point pt = pointVec->getPoint();
			p->createVertex(pt);

		}
	}

	/* If we are just given a single argument of points
	 * build a polygon from that. */
	if(!linesVec) {
		Polygon* pg=p->createPolygon();
		for(int i=0; i<points.length(); ++i)
			pg->append(i);
		return p;
	}

	QList<Value*> lines=linesVec->getChildren();

	//This is to remove the need for double vector syntax in the lines argument
	// e.g. lines=[[0,1,2,3]] can just be writtern as lines=[0,1,2,3]
	if(!lines.isEmpty()) {
		VectorValue* single=dynamic_cast<VectorValue*>(lines.at(0));
		if(!single) {
			lines.clear();
			lines.append(linesVec);
		}
	}

	foreach(Value* line,lines) {
		VectorValue* lineVec=dynamic_cast<VectorValue*>(line);
		if(lineVec) {
			Polygon* pg=p->createPolygon();
			foreach(Value* indexVal,lineVec->getChildren()) {
				NumberValue* indexNum=dynamic_cast<NumberValue*>(indexVal);
				if(indexNum) {
					int index = indexNum->getNumber();
					if(index>=0&&index<points.count()) {
						pg->append(index);
					}
				}
			}
		}
	}

	return p;
}
