#include "asinfunction.h"
#include "numbervalue.h"
#include "rmath.h"

AsinFunction::AsinFunction() : Function("asin")
{
	addParameter("value");
}

Value* AsinFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_asin_deg(num));
	}
	return new Value();
}
