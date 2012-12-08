#include "rotateextrudemodule.h"
#include "node/rotateextrudenode.h"
#include "numbervalue.h"

RotateExtrudeModule::RotateExtrudeModule() : Module("rotate_extrude")
{
}

Node* RotateExtrudeModule::evaluate(Context* ctx)
{
	double r=1.0;
	NumberValue* radius=dynamic_cast<NumberValue*>(ctx->getArgument(0,"radius"));
	if(radius)
		r=radius->getNumber();

	RotateExtrudeNode* n=new RotateExtrudeNode();
	n->setRadius(r);
	n->setChildren(ctx->getInputNodes());
	return n;
}
