#include "volumesmodule.h"
#include "context.h"
#include "node/volumesnode.h"

VolumesModule::VolumesModule() : Module("volume")
{
	auxilary=true;
}

Node* VolumesModule::evaluate(Context* ctx)
{
	VolumesNode* n=new VolumesNode();
	n->setChildren(ctx->getInputNodes());
	return n;
}
