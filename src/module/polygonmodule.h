#ifndef POLYGONMODULE_H
#define POLYGONMODULE_H

#include "module.h"

class PolygonModule : public Module
{
	Q_DECLARE_TR_FUNCTIONS(PolygonModule)
public:
	PolygonModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // POLYGONMODULE_H
