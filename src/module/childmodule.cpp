#include "childmodule.h"
#include "numbervalue.h"

ChildModule::ChildModule() : Module("child")
{
}

Node* ChildModule::evaluate(Context* ctx)
{
	int index=0;
	NumberValue* indexValue = dynamic_cast<NumberValue*>(ctx->getArgument(0,"index"));
	if(indexValue)
		index=indexValue->getNumber();

	QList<Node*> children=ctx->getInputNodes();
	return children.at(index);
}
