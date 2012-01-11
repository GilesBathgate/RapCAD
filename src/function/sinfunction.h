#ifndef SINFUNCTION_H
#define SINFUNCTION_H

#include "function.h"
#include "context.h"

class SinFunction : public Function
{
public:
	SinFunction();
	Value* evaluate(Context*);
};

#endif // SINFUNCTION_H
