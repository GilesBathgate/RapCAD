#ifndef SINHFUNCTION_H
#define SINHFUNCTION_H

#include "function.h"
#include "context.h"

class SinhFunction : public Function
{
public:
	SinhFunction();
	Value* evaluate(Context*);
};

#endif // SINHFUNCTION_H
