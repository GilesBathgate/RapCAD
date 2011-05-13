#ifndef INSETMODULE_H
#define INSETMODULE_H

#include "module.h"
#include "context.h"

class InsetModule : public Module
{
public:
	InsetModule();
	Node* evaluate(Context*,QList<Node*>);
};

#endif // INSETMODULE_H
