#include "signfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

SignFunction::SignFunction() : Function("sign")
{
	addParameter("value");
}

Value* SignFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_sign(num));
	}
	return Value::undefined();
}
