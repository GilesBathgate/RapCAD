#include "fragmentsfunction.h"
#include "context.h"
#include "fragment.h"
#include "numbervalue.h"
#include "valuefactory.h"

FragmentsFunction::FragmentsFunction() : Function("fragments")
{
	addDescription(tr("Returns the number of fragments for a given radius."));
	addParameter("radius","num",tr("The radius for which to provide fragments."));
}

Value& FragmentsFunction::evaluate(const Context& ctx) const
{
	auto* rValue=getParameterArgument<NumberValue>(ctx,0);

	decimal r=1.0;
	if(rValue) {
		r=rValue->getNumber();
	} else {
		NumberValue* dValue = dynamic_cast<NumberValue*>(ctx.getArgument(1,"diameter"));
		if(dValue)
			r=(dValue->getNumber()/2.0);
	}

	const int f=Fragment::getFragments(ctx,r);
	return ValueFactory::createNumber(f);
}
