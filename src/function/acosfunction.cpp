#include "acosfunction.h"
#include "numbervalue.h"
#include "math.h"

AcosFunction::AcosFunction() : Function("acos")
{
	addParameter("value");
}

Value* AcosFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(acos(num));
	}
	return new Value();
}
