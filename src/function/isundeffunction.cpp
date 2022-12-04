#include "isundeffunction.h"
#include "context.h"

IsUndefFunction::IsUndefFunction() : Function("is_undef")
{
	addDescription(tr("Returns true when the given value is an undefined value"));
	addParameter("value","undef",tr("The value to test."));
}

Value& IsUndefFunction::evaluate(const Context& ctx) const
{
	auto* val=getParameterArgument<Value>(ctx,0);
	if(val)
		return ValueFactory::createBoolean(!val->isDefined());

	return ValueFactory::createUndefined();
}
