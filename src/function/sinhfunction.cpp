#include "sinhfunction.h"
#include "numbervalue.h"
#include "math.h"

SinhFunction::SinhFunction() : Function("sinh")
{
	addParameter("value");
}

Value* SinhFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(sinh(num));
	}
	return new Value();
}
