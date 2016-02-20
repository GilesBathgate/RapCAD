#ifndef STRFUNCTION_H
#define STRFUNCTION_H

#include "function.h"

class StrFunction : public Function
{
public:
	StrFunction();
	Value* evaluate(Context*);
};

#endif // STRFUNCTION_H
