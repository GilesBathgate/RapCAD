#include "rotateextrudemodule.h"
#include "node/rotateextrudenode.h"
#include "numbervalue.h"

RotateExtrudeModule::RotateExtrudeModule() : PrimitiveModule("rotate_extrude")
{
	addParameter("radius");
}

Node* RotateExtrudeModule::evaluate(Context* ctx)
{
	double r=1.0;
	NumberValue* radius=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(radius)
		r=radius->getNumber();

	RotateExtrudeNode* n=new RotateExtrudeNode();
	n->setRadius(r);
	int f =getFragments(r,ctx);
	n->setFragments(f);
	n->setChildren(ctx->getInputNodes());
	return n;
}
