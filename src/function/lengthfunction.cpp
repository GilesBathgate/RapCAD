#include "lengthfunction.h"
#include "vectorvalue.h"
#include "textvalue.h"
#include "numbervalue.h"

LengthFunction::LengthFunction() : Function("len")
{
	addParameter("value");
}

Value* LengthFunction::evaluate(Context* ctx)
{
	VectorValue* vecVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	if(vecVal) {
		return new NumberValue(vecVal->getChildren().count());
	}
	TextValue* txtVal=dynamic_cast<TextValue*>(getParameterArgument(ctx,0));
	if(txtVal) {
		return new NumberValue(txtVal->getValueString().length());
	}
	return new Value();
}
