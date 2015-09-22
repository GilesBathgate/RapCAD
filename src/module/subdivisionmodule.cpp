#include "subdivisionmodule.h"
#include "node/subdivisionnode.h"
#include "numbervalue.h"

SubDivisionModule::SubDivisionModule() : Module("subdiv")
{
	addParameter("level");
}

Node* SubDivisionModule::evaluate(Context* ctx)
{
	int level=0;
	NumberValue* levelVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(levelVal)
		level=levelVal->toInteger();

	SubDivisionNode* d = new SubDivisionNode();
	d->setChildren(ctx->getInputNodes());
	d->setLevel(level);
	return d;
}
