#include "coshfunction.h"
#include "numbervalue.h"
#include "math.h"

CoshFunction::CoshFunction() : Function("cosh")
{
	addParameter("value");
}

Value* CoshFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(cosh(num));
	}
	return new Value();
}
