#include "atan2function.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

Atan2Function::Atan2Function() : Function("atan2")
{
	addParameter("y");
	addParameter("x");
}

Value* Atan2Function::evaluate(Context* ctx)
{
	NumberValue* yVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	NumberValue* xVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	if(yVal&&xVal) {
		decimal y=yVal->getNumber();
		decimal x=xVal->getNumber();

		return new NumberValue(r_atan2_deg(y,x));
	}
	return Value::undefined();
}
