#include "childmodule.h"
#include "context.h"
#include "numbervalue.h"

ChildModule::ChildModule(Reporter* r) : Module(r,"child")
{
	addDeprecated(tr("The child module is deprecated, use the children module instead."));
	addParameter("index",tr("The index of the child to use."));
}

Node* ChildModule::evaluate(Context* ctx)
{
	int index=0;
	NumberValue* indexValue = dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(indexValue)
		index=indexValue->toInteger();

	return ctx->lookupChild(index);
}
