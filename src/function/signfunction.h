#ifndef SIGNFUNCTION_H
#define SIGNFUNCTION_H

#include "function.h"

class SignFunction : public Function
{
public:
	SignFunction();
	decimal sign(decimal);
	Value* evaluate(Context*) override;
};

#endif // SIGNFUNCTION_H
