#include "atanfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

AtanFunction::AtanFunction() : Function("atan")
{
	addParameter("value");
}

Value* AtanFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_atan_deg(num));
	}
	return Value::undefined();
}
