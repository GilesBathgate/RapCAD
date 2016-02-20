#ifndef SQUAREMODULE_H
#define SQUAREMODULE_H

#include "primitivemodule.h"

class SquareModule : public PrimitiveModule
{
public:
	SquareModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // SQUAREMODULE_H
