#include "volumesmodule.h"
#include "context.h"
#include "booleanvalue.h"
#include "numbervalue.h"
#include "node/volumesnode.h"

VolumesModule::VolumesModule(Reporter* r) : Module(r,"volume")
{
	auxilary=true;
	addDescription(tr("Provides information about the volume of its children."));
	addParameter("mass",tr("Specifies that the center of mass also be calculated."));
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
