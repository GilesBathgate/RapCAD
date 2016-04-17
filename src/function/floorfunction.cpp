#include "floorfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

FloorFunction::FloorFunction() : Function("floor")
{
	addParameter("value");
}

Value* FloorFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_floor(num));
	}
	return Value::undefined();
}
