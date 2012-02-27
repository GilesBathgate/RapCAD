#include "strfunction.h"
#include "textvalue.h"

StrFunction::StrFunction() : Function("str")
{
}

Value* StrFunction::evaluate(Context* ctx)
{
	Value* val=NULL;
	foreach(Value* arg, ctx->arguments) {
		if(val)
			val=Value::operation(val,Expression::Concatenate,arg);
		else
			val=arg;
	}

	return val;
}
