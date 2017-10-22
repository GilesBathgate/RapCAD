#ifndef TANFUNCTION_H
#define TANFUNCTION_H

#include "function.h"

class TanFunction : public Function
{
public:
	TanFunction();
	Value* evaluate(Context*) override;
};

#endif // TANFUNCTION_H
