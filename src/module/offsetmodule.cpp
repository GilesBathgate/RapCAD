#include "offsetmodule.h"
#include "node/offsetnode.h"
#include "numbervalue.h"

OffsetModule::OffsetModule() : Module("offset")
{
}

Node* OffsetModule::evaluate(Context* ctx)
{
	double a=1.0;
	NumberValue* amountVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"amount"));
	if(amountVal)
		a=amountVal->getNumber();

	OffsetNode* n = new OffsetNode();
	n->setAmount(a);
	n->setChildren(ctx->inputNodes);
	return n;
}
