#include "sinfunction.h"
#include "numbervalue.h"
#include "math.h"

SinFunction::SinFunction() : Function("sin")
{
	addParameter("value");
}

Value* SinFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(sin(num));
	}
	return new Value();
}
