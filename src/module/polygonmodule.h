#ifndef POLYGONMODULE_H
#define POLYGONMODULE_H

#include "module.h"

class PolygonModule : public Module
{
public:
	PolygonModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // POLYGONMODULE_H
