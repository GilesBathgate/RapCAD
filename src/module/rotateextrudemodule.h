#ifndef ROTATEEXTRUDEMODULE_H
#define ROTATEEXTRUDEMODULE_H

#include "primitivemodule.h"

class RotateExtrudeModule : public PrimitiveModule
{
	Q_DECLARE_TR_FUNCTIONS(RotateExtrudeModule)
public:
	RotateExtrudeModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // ROTATEEXTRUDEMODULE_H
