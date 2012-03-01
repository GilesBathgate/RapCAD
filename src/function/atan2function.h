#ifndef ATAN2FUNCTION_H
#define ATAN2FUNCTION_H

#include "function.h"
#include "context.h"

class Atan2Function : public Function
{
public:
	Atan2Function();
	Value* evaluate(Context*);
};

#endif // ATAN2FUNCTION_H
