#include "atanfunction.h"
#include "numbervalue.h"
#include "math.h"

AtanFunction::AtanFunction() : Function("atan")
{
	addParameter("value");
}

Value* AtanFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(atan(num));
	}
	return new Value();
}
