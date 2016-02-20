#ifndef CHILDMODULE_H
#define CHILDMODULE_H

#include "module.h"

class ChildModule : public Module
{
public:
	ChildModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // CHILDMODULE_H
