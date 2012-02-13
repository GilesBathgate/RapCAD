#ifndef SIGNFUNCTION_H
#define SIGNFUNCTION_H

#include "function.h"
#include "context.h"

class SignFunction : public Function
{
public:
	SignFunction();
	int sign(int);
	Value* evaluate(Context*);
};

#endif // SIGNFUNCTION_H
