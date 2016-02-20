#ifndef CEILFUNCTION_H
#define CEILFUNCTION_H

#include "function.h"

class CeilFunction : public Function
{
public:
	CeilFunction();
	Value* evaluate(Context*);
};

#endif // CEILFUNCTION_H
