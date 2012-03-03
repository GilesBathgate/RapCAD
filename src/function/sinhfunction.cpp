#include "sinhfunction.h"
#include "numbervalue.h"
#include "math.h"

SinhFunction::SinhFunction() : Function("sinh")
{
}

Value* SinhFunction::evaluate(Context* ctx)
{
        NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
        if(numVal) {
                double num=numVal->getNumber();

                NumberValue* result;
                result = new NumberValue(sinh(num));
                return result;
        }
        return new Value();
}
