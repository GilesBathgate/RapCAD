#include "rotateextrudemodule.h"
#include "node/rotateextrudenode.h"
#include "node/transformationnode.h"
#include "numbervalue.h"
#include "vectorvalue.h"

RotateExtrudeModule::RotateExtrudeModule(Reporter* r) : PrimitiveModule(r,"rotate_extrude")
{
	addDescription(tr("Extrudes its children about the given axis."));
	addParameter("radius",tr("The radius of the extrusion."));
	addParameter("axis",tr("The axis of rotation. When no axis is given the shape is first rotated about the x axis, then extruded about the z axis."));
}

Node* RotateExtrudeModule::evaluate(Context* ctx)
{
	decimal r=0.0;
	NumberValue* radius=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(radius)
		r=radius->getNumber();

	bool compatible=true;
	Point axis(0,0,1);
	VectorValue* vecVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,1));
	if(vecVal) {
		//TODO actually use the axis for rotated extrusions.
		axis=vecVal->getPoint();
		compatible=false;
	}

	RotateExtrudeNode* n=new RotateExtrudeNode();
	n->setRadius(r);
	n->setAxis(axis);

	Fragment* fg = Fragment::createFragment(ctx);
	n->setFragments(fg);
	n->setChildren(ctx->getInputNodes());

	if(compatible) {
		//if no axis is given we fall into legacy compatibility mode
		TransformMatrix* Rx90=new TransformMatrix(1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1);
		TransformationNode* t=new TransformationNode();
		t->setMatrix(Rx90);
		t->addChild(n);
		return t;
	}

	return n;
}
