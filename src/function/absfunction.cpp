#include "absfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

AbsFunction::AbsFunction() : Function("abs")
{
	addParameter("value");
}

Value* AbsFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		return Value::operation(numVal,Expression::Length);
	}
	return Value::undefined();
}
