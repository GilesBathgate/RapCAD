#include "childmodule.h"
#include "context.h"
#include "numbervalue.h"

ChildModule::ChildModule() : Module("child")
{
	addParameter("index");
}

Node* ChildModule::evaluate(Context* ctx)
{
	int index=0;
	NumberValue* indexValue = dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(indexValue)
		index=indexValue->toInteger();

	return ctx->lookupChild(index);
}
