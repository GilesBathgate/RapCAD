#include "acosfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

AcosFunction::AcosFunction() : Function("acos")
{
	addParameter("value");
}

Value* AcosFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_acos_deg(num));
	}
	return Value::undefined();
}
