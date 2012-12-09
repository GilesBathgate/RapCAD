#include "absfunction.h"
#include "numbervalue.h"
#include "math.h"

AbsFunction::AbsFunction() : Function("abs")
{
	addParameter("value");
}

Value* AbsFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(fabs(num));
	}
	return new Value();
}
