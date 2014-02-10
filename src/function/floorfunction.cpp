#include "floorfunction.h"
#include "numbervalue.h"
#include "math.h"

FloorFunction::FloorFunction() : Function("floor")
{
	addParameter("value");
}

Value* FloorFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(floor(num));
	}
	return new Value();
}
