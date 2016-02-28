#include "offsetmodule.h"
#include "context.h"
#include "node/offsetnode.h"
#include "numbervalue.h"

OffsetModule::OffsetModule(Reporter* r) : Module(r,"offset")
{
	addDescription(tr("Offset moves the outlines of polygons outward or inward by a given amount."));
	addParameter("delta",tr("The amount to offset the polygon. The polygon is offset inwards, when negative."));
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
