#ifndef SQUAREMODULE_H
#define SQUAREMODULE_H

#include "primitivemodule.h"
#include "context.h"

class SquareModule : public PrimitiveModule
{
public:
	SquareModule();
	Node* evaluate(Context*,QVector<Node*>);
};

#endif // SQUAREMODULE_H
