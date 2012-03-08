#include "signfunction.h"
#include "numbervalue.h"

SignFunction::SignFunction() : Function("sign")
{
}

int SignFunction::sign(int num)
{
	return num<0?-1.0:num>0?1.0:0.0;
}

Value* SignFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(sign(num));
	}
	return new Value();
}
