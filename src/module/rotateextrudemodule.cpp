#include "rotateextrudemodule.h"
#include "node/rotateextrudenode.h"
#include "node/transformationnode.h"
#include "numbervalue.h"
#include "vectorvalue.h"

RotateExtrudeModule::RotateExtrudeModule(Reporter* r) : PrimitiveModule(r,"rotate_extrude")
{
	addDescription(tr("Extrudes its children about the given axis."));
	addParameter("angle",tr("The sweep angle for the extrusion."));
	addParameter("axis",tr("The axis of rotation. When no axis is given the shape is first rotated 90° about the x axis, then extruded about the z axis."));
	addParameter("radius",tr("The radius of the extrusion."));
	addParameter("height",tr("The helical height of the extrusion."));
}

Node* RotateExtrudeModule::evaluate(Context* ctx)
{
	decimal angle=360.0;
	auto* angleVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(angleVal)
		angle=angleVal->getNumber();

	bool compatible=true;
	Point axis(0,0,1);
	auto* vecVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,1));
	if(vecVal) {
		//TODO actually use the axis for rotated extrusions.
		axis=vecVal->getPoint();
		compatible=false;
	}

	decimal radius=0.0;
	auto* radiusVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,2));
	if(radiusVal)
		radius=radiusVal->getNumber();

	decimal height=0.0;
	auto* heightVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,3));
	if(heightVal)
		height=heightVal->getNumber();

	auto* n=new RotateExtrudeNode();
	n->setSweep(angle);
	n->setAxis(axis);
	n->setRadius(radius);
	n->setHeight(height);

	Fragment* fg = Fragment::createFragment(ctx);
	n->setFragments(fg);
	n->setChildren(ctx->getInputNodes());

	if(compatible) {
		//if no axis is given we fall into legacy compatibility mode
		auto* Rx90=new TransformMatrix(1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1);
		auto* t=new TransformationNode();
		t->setMatrix(Rx90);
		t->addChild(n);
		return t;
	}

	return n;
}
