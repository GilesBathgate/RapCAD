#include "minfunction.h"
#include "context.h"

MinFunction::MinFunction() : Function("min")
{
	addParameter("values");
}

Value* MinFunction::evaluate(Context* ctx)
{
	QList<Value*> values=ctx->getArguments();

	return Value::compareAll(values,Expression::LessThan);
}
