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
		decimal a=aVal->getNumber();
		decimal b=bVal->getNumber();

		return new NumberValue(fmax(a,b));
	}
	return new Value();
}
