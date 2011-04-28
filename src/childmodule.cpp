#include "childmodule.h"
#include "numbervalue.h"

ChildModule::ChildModule() : Module("child")
{
}

Node* ChildModule::evaluate(Context* ctx,QList<Node*> childs)
{
	int index=0;
	NumberValue* indexValue = dynamic_cast<NumberValue*>(ctx->getArgument(0,"index"));
	if(indexValue)
		index=indexValue->getNumber();

	return childs.at(index);
}
