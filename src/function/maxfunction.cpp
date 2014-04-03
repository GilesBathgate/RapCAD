#include "maxfunction.h"
#include "numbervalue.h"
#include "vectorvalue.h"
#include "rangevalue.h"
#include "onceonly.h"
#include "math.h"

MaxFunction::MaxFunction() : Function("max")
{
	addParameter("values");
}

static decimal maximum(QList<Value*> values,bool& ok)
{
	decimal v;
	OnceOnly first;
	foreach(Value* a,values) {
		NumberValue* nextVal=dynamic_cast<NumberValue*>(a);
		if(nextVal) {
			if(first())
				v=nextVal->getNumber();
			else
				v=fmax(v,nextVal->getNumber());

			ok=true;
		}
		VectorValue* vecVal=dynamic_cast<VectorValue*>(a);
		if(vecVal) {
			v=fmax(v,maximum(vecVal->getChildren(),ok));
		}
		RangeValue* rngVal=dynamic_cast<RangeValue*>(a);
		if(rngVal) {
			QList<Value*> rng;
			rng.append(rngVal->getStart());
			rng.append(rngVal->getFinish());
			v=fmax(v,maximum(rng,ok));
		}
	}
	return v;
}

Value* MaxFunction::evaluate(Context* ctx)
{
	QList<Value*> values=ctx->getArguments();

	bool ok=false;
	decimal v=maximum(values,ok);
	if(!ok)
		return new Value();

	return new NumberValue(v);
}
