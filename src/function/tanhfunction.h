#ifndef TANHFUNCTION_H
#define TANHFUNCTION_H

#include "function.h"

class TanhFunction : public Function
{
public:
	TanhFunction();
	Value* evaluate(Context*) override;
};

#endif // TANHFUNCTION_H
