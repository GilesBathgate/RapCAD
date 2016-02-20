#ifndef ACOSFUNCTION_H
#define ACOSFUNCTION_H

#include "function.h"

class AcosFunction : public Function
{
public:
	AcosFunction();
	Value* evaluate(Context*);
};

#endif // ACOSFUNCTION_H
