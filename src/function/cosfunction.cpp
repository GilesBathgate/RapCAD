#include "cosfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

CosFunction::CosFunction() : Function("cos")
{
	addParameter("value");
}

Value* CosFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_cos_deg(num));
	}
	return Value::undefined();
}
