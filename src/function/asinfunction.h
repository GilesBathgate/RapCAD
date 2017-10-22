#ifndef ASINFUNCTION_H
#define ASINFUNCTION_H

#include "function.h"

class AsinFunction : public Function
{
public:
	AsinFunction();
	Value* evaluate(Context*) override;
};

#endif // ASINFUNCTION_H
