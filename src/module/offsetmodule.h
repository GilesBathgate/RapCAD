#ifndef OFFSETMODULE_H
#define OFFSETMODULE_H

#include "module.h"

class OffsetModule : public Module
{
public:
	OffsetModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // OFFSETMODULE_H
