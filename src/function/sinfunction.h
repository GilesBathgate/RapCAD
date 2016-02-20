#ifndef SINFUNCTION_H
#define SINFUNCTION_H

#include "function.h"

class SinFunction : public Function
{
public:
	SinFunction();
	Value* evaluate(Context*);
};

#endif // SINFUNCTION_H
