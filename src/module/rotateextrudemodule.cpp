#include "rotateextrudemodule.h"
#include "node/rotateextrudenode.h"
#include "numbervalue.h"

RotateExtrudeModule::RotateExtrudeModule() : PrimitiveModule("rotate_extrude")
{
	addParameter("radius");
}

Node* RotateExtrudeModule::evaluate(Context* ctx)
{
	decimal r=0.0;
	NumberValue* radius=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(radius)
		r=radius->getNumber();

	RotateExtrudeNode* n=new RotateExtrudeNode();
	n->setRadius(r);

	Fragment fg=getSpecialVariables(ctx);
	n->setFragments(fg);
	n->setChildren(ctx->getInputNodes());
	return n;
}
