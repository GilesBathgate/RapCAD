#include "tanfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

TanFunction::TanFunction() : Function("tan")
{
	addParameter("value");
}

Value* TanFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_tan_deg(num));
	}
	return Value::undefined();
}
