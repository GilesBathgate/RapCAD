#ifndef TANFUNCTION_H
#define TANFUNCTION_H

#include "function.h"

class TanFunction : public Function
{
public:
	TanFunction();
	Value* evaluate(Context*);
};

#endif // TANFUNCTION_H
