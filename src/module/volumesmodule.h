#ifndef VOLUMESMODULE_H
#define VOLUMESMODULE_H

#include "module.h"

class VolumesModule : public Module
{
	Q_DECLARE_TR_FUNCTIONS(VolumesModule)
public:
	VolumesModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // VOLUMESMODULE_H
