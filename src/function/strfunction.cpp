#include "strfunction.h"
#include "context.h"
#include "context.h"
#include "textvalue.h"

StrFunction::StrFunction() : Function("str")
{
}

Value* StrFunction::evaluate(Context* ctx)
{
	TextValue* val=NULL;
	foreach(Value* argVal, ctx->getArguments()) {
		TextValue* arg = argVal->toText();
		if(!val) {
			val=arg;
		} else {
			Value* res=Value::operation(val,Expression::Concatenate,arg);
			val=res->toText();
		}
	}

	return val;
}
