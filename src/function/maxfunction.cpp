#include "maxfunction.h"
#include "context.h"

MaxFunction::MaxFunction() : Function("max")
{
	addParameter("values");
}

Value* MaxFunction::evaluate(Context* ctx)
{
	QList<Value*> values=ctx->getArguments();

	return Value::compareAll(values,Expression::GreaterThan);
}
