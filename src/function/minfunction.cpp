#include "minfunction.h"
#include "numbervalue.h"
#include "vectorvalue.h"
#include "rangevalue.h"
#include "onceonly.h"
#include "math.h"

MinFunction::MinFunction() : Function("min")
{
	addParameter("values");
}

static decimal minimum(QList<Value*> values,bool& ok)
{
	decimal v;
	OnceOnly first;
	foreach(Value* a,values) {
		NumberValue* nextVal=dynamic_cast<NumberValue*>(a);
		if(nextVal) {
			if(first())
				v=nextVal->getNumber();
			else
				v=fmin(v,nextVal->getNumber());

			ok=true;
		}
		VectorValue* vecVal=dynamic_cast<VectorValue*>(a);
		if(vecVal) {
			v=fmin(v,minimum(vecVal->getChildren(),ok));
		}
		RangeValue* rngVal=dynamic_cast<RangeValue*>(a);
		if(rngVal) {
			QList<Value*> rng;
			rng.append(rngVal->getStart());
			rng.append(rngVal->getFinish());
			v=fmin(v,minimum(rng,ok));
		}
	}
	return v;
}

Value* MinFunction::evaluate(Context* ctx)
{
	QList<Value*> values=ctx->getArguments();

	bool ok=false;
	decimal v=minimum(values,ok);
	if(!ok)
		return new Value();

	return new NumberValue(v);
}
