#ifndef SQUAREMODULE_H
#define SQUAREMODULE_H

#include "primitivemodule.h"
#include "context.h"

class SquareModule : public PrimitiveModule
{
public:
	SquareModule();
	Node* evaluate(Context*,QList<Node*>);
};

#endif // SQUAREMODULE_H
