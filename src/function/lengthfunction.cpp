#include "lengthfunction.h"
#include "vectorvalue.h"
#include "textvalue.h"
#include "numbervalue.h"

LengthFunction::LengthFunction() : Function("len")
{
}

Value* LengthFunction::evaluate(Context* ctx)
{
	VectorValue* vecVal=dynamic_cast<VectorValue*>(ctx->getArgument(0,"value"));
	if(vecVal) {
		return new NumberValue(vecVal->getChildren().count());
	}
	TextValue* txtVal=dynamic_cast<TextValue*>(ctx->getArgument(0,"value"));
	if(txtVal) {
		return new NumberValue(txtVal->getValueString().length());
	}
	return new Value();
}
