#include "absfunction.h"
#include "numbervalue.h"
#include "rmath.h"

AbsFunction::AbsFunction() : Function("abs")
{
	addParameter("value");
}

Value* AbsFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_abs(num));
	}
	return new Value();
}
