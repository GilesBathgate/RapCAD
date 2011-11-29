#ifndef CHILDMODULE_H
#define CHILDMODULE_H

#include "module.h"
#include "context.h"

class ChildModule : public Module
{
public:
	ChildModule();
	Node* evaluate(Context*);
};

#endif // CHILDMODULE_H
