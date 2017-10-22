#ifndef SINFUNCTION_H
#define SINFUNCTION_H

#include "function.h"

class SinFunction : public Function
{
public:
	SinFunction();
	Value* evaluate(Context*) override;
};

#endif // SINFUNCTION_H
