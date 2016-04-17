#include "expfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

ExpFunction::ExpFunction() : Function("exp")
{
	addParameter("value");
}

Value* ExpFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_exp(num));
	}
	return Value::undefined();
}
