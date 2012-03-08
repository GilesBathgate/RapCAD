#include "atan2function.h"
#include "numbervalue.h"
#include "math.h"

Atan2Function::Atan2Function() : Function("atan2")
{
}

Value* Atan2Function::evaluate(Context* ctx)
{
	NumberValue* yVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"y"));
	NumberValue* xVal=dynamic_cast<NumberValue*>(ctx->getArgument(1,"x"));
	if(yVal&&xVal) {
		double y=yVal->getNumber();
		double x=xVal->getNumber();

		return new NumberValue(atan2(y,x));
	}
	return new Value();
}
