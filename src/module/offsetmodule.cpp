#include "offsetmodule.h"
#include "node/offsetnode.h"
#include "numbervalue.h"

OffsetModule::OffsetModule() : Module("offset")
{
	addParameter("amount");
}

Node* OffsetModule::evaluate(Context* ctx)
{
	decimal a=1.0;
	NumberValue* amountVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(amountVal)
		a=amountVal->getNumber();

	OffsetNode* n = new OffsetNode();
	n->setAmount(a);
	n->setChildren(ctx->getInputNodes());
	return n;
}
