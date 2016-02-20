#ifndef ATANFUNCTION_H
#define ATANFUNCTION_H

#include "function.h"

class AtanFunction : public Function
{
public:
	AtanFunction();
	Value* evaluate(Context*);
};

#endif // ATANFUNCTION_H
