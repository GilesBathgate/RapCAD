#ifndef FLOORFUNCTION_H
#define FLOORFUNCTION_H

#include "function.h"

class FloorFunction : public Function
{
public:
	FloorFunction();
	Value* evaluate(Context*) override;
};

#endif // FLOORFUNCTION_H
