#ifndef OFFSETMODULE_H
#define OFFSETMODULE_H

#include "module.h"
#include "context.h"

class OffsetModule : public Module
{
public:
	OffsetModule();
	Node* evaluate(Context*);
};

#endif // OFFSETMODULE_H
