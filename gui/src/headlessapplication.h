#ifndef HEADLESSAPPLICATION_H
#define HEADLESSAPPLICATION_H

#include "headless.h"
#include "renderexport.h"

class HeadlessApplication : Headless
{
public:
	static Headless& create();
	QCoreApplication* headlessOverride() const override;
	Export* getExporter(Primitive*,Reporter&) const override;
private:
	HeadlessApplication();
	mutable RenderExport* exporter;
};

#endif // HEADLESSAPPLICATION_H
