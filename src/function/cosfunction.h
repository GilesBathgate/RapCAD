#ifndef COSFUNCTION_H
#define COSFUNCTION_H

#include "function.h"

class CosFunction : public Function
{
public:
	CosFunction();
	Value* evaluate(Context*) override;
};

#endif // COSFUNCTION_H
