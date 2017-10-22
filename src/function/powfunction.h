#ifndef POWFUNCTION_H
#define POWFUNCTION_H

#include "function.h"

class PowFunction : public Function
{
public:
	PowFunction();
	Value* evaluate(Context*) override;
};

#endif // POWFUNCTION_H
