#include "roundfunction.h"
#include "numbervalue.h"
#include "rmath.h"

RoundFunction::RoundFunction() : Function("round")
{
	addParameter("value");
}

Value* RoundFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_round(num));
	}
	return new Value();
}
