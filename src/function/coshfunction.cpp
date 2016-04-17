#include "coshfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

CoshFunction::CoshFunction() : Function("cosh")
{
	addParameter("value");
}

Value* CoshFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_cosh_deg(num));
	}
	return Value::undefined();
}
