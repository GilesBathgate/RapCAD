#include "headless.h"

const Headless* Headless::instance;

Headless::Headless()
{
}

void Headless::setInstance()
{
	Headless::instance=this;
}

QCoreApplication* Headless::headlessOverride() const
{
	if(!instance) return nullptr;
	return instance->headlessOverride();
}

Export* Headless::getExporter(Primitive* p,Reporter& r) const
{
	if(!instance) return nullptr;
	return instance->getExporter(p,r);
}
