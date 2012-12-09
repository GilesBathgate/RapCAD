#include "maxfunction.h"
#include "numbervalue.h"
#include "math.h"

MaxFunction::MaxFunction() : Function("max")
{
	addParameter("a");
	addParameter("b");
}

Value* MaxFunction::evaluate(Context* ctx)
{
	NumberValue* aVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	NumberValue* bVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	if(aVal&&bVal) {
		double a=aVal->getNumber();
		double b=bVal->getNumber();

		return new NumberValue(fmax(a,b));
	}
	return new Value();
}
