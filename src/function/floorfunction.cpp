#include "floorfunction.h"
#include "numbervalue.h"
#include "math.h"

FloorFunction::FloorFunction() : Function("floor")
{
}

Value* FloorFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		NumberValue* result;
		result = new NumberValue(floor(num));
		return result;
	}
	return new Value();
}
