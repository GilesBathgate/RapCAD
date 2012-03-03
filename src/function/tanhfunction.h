#ifndef TANHFUNCTION_H
#define TANHFUNCTION_H

#include "function.h"
#include "context.h"

class TanhFunction : public Function
{
public:
	TanhFunction();
	Value* evaluate(Context*);
};

#endif // TANHFUNCTION_H
