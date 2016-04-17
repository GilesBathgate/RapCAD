#include "roundfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

RoundFunction::RoundFunction() : Function("round")
{
	addParameter("value");
	addParameter("places");
}

Value* RoundFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		NumberValue* placesVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
		if(placesVal) {
			int places=placesVal->toInteger();
			return new NumberValue(r_round(num,places));
		}

		return new NumberValue(r_round(num));
	}
	return Value::undefined();
}
