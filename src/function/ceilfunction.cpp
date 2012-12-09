#include "ceilfunction.h"
#include "numbervalue.h"
#include "math.h"

CeilFunction::CeilFunction() : Function("ceil")
{
	addParameter("value");
}

Value* CeilFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(ceil(num));
	}
	return new Value();
}
