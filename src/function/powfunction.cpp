#include "powfunction.h"
#include "context.h"

PowFunction::PowFunction() : Function("pow")
{
	addParameter("value");
	addParameter("exponent");
}

Value* PowFunction::evaluate(Context* ctx)
{
	Value* val=getParameterArgument(ctx,0);
	Value* arg=getParameterArgument(ctx,1);
	if(val&&arg)
		return Value::operation(val,Expression::Exponent,arg);

	return Value::undefined();
}
