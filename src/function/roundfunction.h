#ifndef ROUNDFUNCTION_H
#define ROUNDFUNCTION_H

#include "function.h"

class RoundFunction : public Function
{
public:
	RoundFunction();
	Value* evaluate(Context*) override;
};

#endif // ROUNDFUNCTION_H
