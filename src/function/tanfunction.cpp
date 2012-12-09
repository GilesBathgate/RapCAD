#include "tanfunction.h"
#include "numbervalue.h"
#include "math.h"

TanFunction::TanFunction() : Function("tan")
{
	addParameter("value");
}

Value* TanFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(tan(num));
	}
	return new Value();
}
