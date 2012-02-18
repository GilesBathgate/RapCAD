#include "powfunction.h"
#include "expression.h"

PowFunction::PowFunction() : Function("pow")
{
}

Value* PowFunction::evaluate(Context* ctx)
{
	Value* val=ctx->getArgument(0,"value");
	Value* arg=ctx->getArgument(1,"exponent");
	if(val&&arg)
		return Value::operation(val,Expression::Exponent,arg);

	return new Value();
}
