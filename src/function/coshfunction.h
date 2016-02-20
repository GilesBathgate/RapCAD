#ifndef COSHFUNCTION_H
#define COSHFUNCTION_H

#include "function.h"

class CoshFunction : public Function
{
public:
	CoshFunction();
	Value* evaluate(Context*);
};

#endif // COSHFUNCTION_H
