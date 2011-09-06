#ifndef VERSIONFUNCTION_H
#define VERSIONFUNCTION_H

#include "function.h"
#include "context.h"

class VersionFunction : public Function
{
public:
	VersionFunction();
	Value* evaluate(Context*);
private:
	QList<Value*> version;
};

#endif // VERSIONFUNCTION_H
