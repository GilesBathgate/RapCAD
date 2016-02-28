#ifndef SQUAREMODULE_H
#define SQUAREMODULE_H

#include "primitivemodule.h"

class SquareModule : public PrimitiveModule
{
	Q_DECLARE_TR_FUNCTIONS(SquareModule)
public:
	SquareModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // SQUAREMODULE_H
