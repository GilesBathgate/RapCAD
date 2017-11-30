#ifndef ISVECFUNCTION_H
#define ISVECFUNCTION_H

#include "function.h"

class IsVecFunction : public Function
{
public:
	IsVecFunction(int);
	Value* evaluate(Context*) override;
private:
	int size;
};

#endif // ISVECFUNCTION_H
