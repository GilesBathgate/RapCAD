#ifndef MINFUNCTION_H
#define MINFUNCTION_H

#include "function.h"

class MinFunction : public Function
{
public:
	MinFunction();
	Value* evaluate(Context*) override;
};

#endif // MINFUNCTION_H
