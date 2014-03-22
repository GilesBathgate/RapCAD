#include "volumesmodule.h"
#include "context.h"
#include "booleanvalue.h"
#include "numbervalue.h"
#include "node/volumesnode.h"

VolumesModule::VolumesModule() : Module("volume")
{
	auxilary=true;
	addParameter("mass");
	addParameter("precision");
}

Node* VolumesModule::evaluate(Context* ctx)
{
	bool mass=false;
	BooleanValue* massVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,0));
	if(massVal)
		mass=massVal->isTrue();

	NumberValue* precVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));

	VolumesNode* n=new VolumesNode();
	if(precVal)
		n->setPrecision(precVal->getNumber());

	n->setCalcMass(mass);
	n->setChildren(ctx->getInputNodes());
	return n;
}
