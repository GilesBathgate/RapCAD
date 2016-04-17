#include "ceilfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

CeilFunction::CeilFunction() : Function("ceil")
{
	addParameter("value");
}

Value* CeilFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_ceil(num));
	}
	return Value::undefined();
}
