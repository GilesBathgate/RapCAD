#include "sinhfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

SinhFunction::SinhFunction() : Function("sinh")
{
	addParameter("value");
}

Value* SinhFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_sinh_deg(num));
	}
	return Value::undefined();
}
