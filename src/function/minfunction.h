#ifndef MINFUNCTION_H
#define MINFUNCTION_H

#include "function.h"
#include "context.h"

class MinFunction : public Function
{
public:
	MinFunction();
	Value* evaluate(Context*);
};

#endif // MINFUNCTION_H
