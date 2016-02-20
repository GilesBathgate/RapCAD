#ifndef POWFUNCTION_H
#define POWFUNCTION_H

#include "function.h"

class PowFunction : public Function
{
public:
	PowFunction();
	Value* evaluate(Context*);
};

#endif // POWFUNCTION_H
