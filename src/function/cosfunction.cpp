#include "cosfunction.h"
#include "numbervalue.h"
#include "math.h"

CosFunction::CosFunction() : Function("cos")
{
}

Value* CosFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		NumberValue* result;
		result = new NumberValue(cos(num));
		return result;
	}
	return new Value();
}
