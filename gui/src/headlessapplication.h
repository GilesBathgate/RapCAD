#ifndef HEADLESSAPPLICATION_H
#define HEADLESSAPPLICATION_H

#include "headless.h"

class HeadlessApplication : Headless
{
public:
	static Headless& create();
	QCoreApplication* headlessOverride() const override;
private:
	HeadlessApplication();
};

#endif // HEADLESSAPPLICATION_H
