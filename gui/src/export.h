#ifndef EXPORT_H
#define EXPORT_H

#include "primitive.h"
#include "reporter.h"

#include <QFileInfo>
#include <QString>

class Export
{
public:
	Export(Primitive*,Reporter&);
	void exportResult(const QFileInfo&) const;
protected:
	Primitive* primitive;
	Reporter& reporter;
};

#endif // EXPORT_H
