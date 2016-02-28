#ifndef CHILDMODULE_H
#define CHILDMODULE_H

#include "module.h"

class ChildModule : public Module
{
	Q_DECLARE_TR_FUNCTIONS(ChildModule)
public:
	ChildModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // CHILDMODULE_H
