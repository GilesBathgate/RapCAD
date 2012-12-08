#include "subdivisionmodule.h"
#include "node/subdivisionnode.h"
#include "numbervalue.h"

SubDivisionModule::SubDivisionModule() : Module("subdiv")
{
}

Node* SubDivisionModule::evaluate(Context* ctx)
{
	int level=0;
	NumberValue* levelVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"level"));
	if(levelVal)
		level=int(levelVal->getNumber());

	SubDivisionNode* d = new SubDivisionNode();
	d->setChildren(ctx->getInputNodes());
	d->setLevel(level);
	return d;
}
