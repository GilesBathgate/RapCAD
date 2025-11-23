#ifndef HEADLESS_H
#define HEADLESS_H

#include "export.h"
#include <QtGlobal>
#include <QCoreApplication>

class Headless
{
	Q_DISABLE_COPY_MOVE(Headless)
public:
	Headless();
	virtual QCoreApplication* headlessOverride() const;
	virtual Export* getExporter(Primitive*,Reporter&) const;
protected:
	void setInstance();
private:
	static const Headless* instance;
};

#endif // HEADLESS_H
