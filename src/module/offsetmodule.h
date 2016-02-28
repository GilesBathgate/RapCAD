#ifndef OFFSETMODULE_H
#define OFFSETMODULE_H

#include "module.h"

class OffsetModule : public Module
{
	Q_DECLARE_TR_FUNCTIONS(OffsetModule)
public:
	OffsetModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // OFFSETMODULE_H
