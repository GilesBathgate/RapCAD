#ifndef ROTATEEXTRUDEMODULE_H
#define ROTATEEXTRUDEMODULE_H

#include "module.h"
#include "context.h"

class RotateExtrudeModule : public Module
{
public:
	RotateExtrudeModule();
	Node* evaluate(Context*);
};

#endif // ROTATEEXTRUDEMODULE_H
