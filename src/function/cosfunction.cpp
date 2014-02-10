#include "cosfunction.h"
#include "numbervalue.h"
#include "math.h"

CosFunction::CosFunction() : Function("cos")
{
	addParameter("value");
}

Value* CosFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(cos(num));
	}
	return new Value();
}
