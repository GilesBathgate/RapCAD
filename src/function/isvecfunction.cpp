#include "isvecfunction.h"
#include "vectorvalue.h"
#include "booleanvalue.h"
#include "context.h"

IsVecFunction::IsVecFunction(int s) : Function(QString("is_vec%1").arg(s)), size(s)
{
	addParameter("value");
}

Value* IsVecFunction::evaluate(Context* ctx)
{
	auto* vec=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	if(!vec) {
		return new BooleanValue(false);
	} else {
		QList<Value*> vals=vec->getChildren();
		if(vals.count()!=size) {
			return new BooleanValue(false);
		} else {
			for(Value* v: vals) {
				auto* numVal=dynamic_cast<NumberValue*>(v);
				if(!numVal) {
					return new BooleanValue(false);
				}
			}
		}
	}

	return new BooleanValue(true);
}
