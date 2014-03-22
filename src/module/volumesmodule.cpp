#include "volumesmodule.h"
#include "context.h"
#include "booleanvalue.h"
#include "node/volumesnode.h"

VolumesModule::VolumesModule() : Module("volume")
{
	auxilary=true;
	addParameter("mass");
}

Node* VolumesModule::evaluate(Context* ctx)
{
	bool mass=false;
	BooleanValue* massVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,0));
	if(massVal)
		mass=massVal->isTrue();

	VolumesNode* n=new VolumesNode();
	n->setCalcMass(mass);
	n->setChildren(ctx->getInputNodes());
	return n;
}
