#ifndef CEILFUNCTION_H
#define CEILFUNCTION_H

#include "function.h"
#include "context.h"

class CeilFunction : public Function
{
public:
	CeilFunction();
	Value* evaluate(Context*);
};

#endif // CEILFUNCTION_H
