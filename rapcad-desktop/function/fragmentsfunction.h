#ifndef FRAGMENTSFUNCTION_H
#define FRAGMENTSFUNCTION_H

#include "function.h"

class FragmentsFunction : public Function
{
	Q_DECLARE_TR_FUNCTIONS(FragmentsFunction)
public:
	FragmentsFunction();
	Value& evaluate(const Context&) const override;
};

#endif // FRAGMENTSFUNCTION_H
