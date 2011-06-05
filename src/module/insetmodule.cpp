#include "insetmodule.h"
#include "insetnode.h"
#include "numbervalue.h"

InsetModule::InsetModule() : Module("inset")
{
}

Node* InsetModule::evaluate(Context* ctx,QList<Node*> childs)
{
	double a=1.0;
	NumberValue* amountVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"amount"));
	if(amountVal)
		a=amountVal->getNumber();

	InsetNode* n = new InsetNode();
	n->setAmount(a);
	n->setChildren(childs);
	return n;
}
