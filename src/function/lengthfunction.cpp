#include "lengthfunction.h"
#include "context.h"
#include "vectorvalue.h"
#include "textvalue.h"
#include "numbervalue.h"
#include "rangevalue.h"

LengthFunction::LengthFunction() : Function("len")
{
	addParameter("value");
}

Value* LengthFunction::evaluate(Context* ctx)
{
	Value* v=getParameterArgument(ctx,0);

	RangeValue* rngVal=dynamic_cast<RangeValue*>(v);
	if(rngVal) {
		return Value::operation(rngVal,Expression::Length);
	}

	VectorValue* vecVal=dynamic_cast<VectorValue*>(v);
	if(vecVal) {
		return new NumberValue(vecVal->getChildren().count());
	}

	TextValue* txtVal=dynamic_cast<TextValue*>(v);
	if(txtVal) {
		return Value::operation(txtVal,Expression::Length);
	}
	return Value::undefined();
}
