#ifndef VOLUMESMODULE_H
#define VOLUMESMODULE_H

#include "module.h"

class VolumesModule : public Module
{
public:
	VolumesModule();
	Node* evaluate(Context*);
};

#endif // VOLUMESMODULE_H
