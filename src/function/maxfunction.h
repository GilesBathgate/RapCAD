#ifndef MAXFUNCTION_H
#define MAXFUNCTION_H

#include "function.h"

class MaxFunction : public Function
{
public:
	MaxFunction();
	Value* evaluate(Context*);
};

#endif // MAXFUNCTION_H
