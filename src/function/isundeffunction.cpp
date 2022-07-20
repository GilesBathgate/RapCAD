#include "isundeffunction.h"
#include "context.h"

IsUndefFunction::IsUndefFunction() : Function("is_undef")
{
	addDescription(tr("Returns true when the given value is an undefined value"));
	addParameter("value");
}

Value& IsUndefFunction::evaluate(const Context& ctx) const
{
	auto* val=getParameterArgument(ctx,0);
	if(val)
		return ValueFactory::createBoolean(!val->isDefined());

	return ValueFactory::createUndefined();
}
