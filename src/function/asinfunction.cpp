#include "asinfunction.h"
#include "numbervalue.h"
#include "math.h"

AsinFunction::AsinFunction() : Function("asin")
{
	addParameter("value");
}

Value* AsinFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(asin(num));
	}
	return new Value();
}
