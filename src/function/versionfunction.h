#ifndef VERSIONFUNCTION_H
#define VERSIONFUNCTION_H

#include "function.h"

class VersionFunction : public Function
{
public:
	VersionFunction();
	Value* evaluate(Context*) override;
};

#endif // VERSIONFUNCTION_H
