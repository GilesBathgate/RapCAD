#ifndef THREADMODULE_H
#define THREADMODULE_H

#include <module/primitivemodule.h>

class ThreadModule : public PrimitiveModule
{
	Q_DECLARE_TR_FUNCTIONS(ThreadModule)
public:
	explicit ThreadModule(Reporter&);
	Node* evaluate(const Context&) const override;
};

#endif // THREADMODULE_H
