#include <QStringList>
#include "versionfunction.h"
#include "context.h"
#include "vectorvalue.h"
#include "numbervalue.h"
#include "textvalue.h"
#include "stringify.h"

VersionFunction::VersionFunction() : Function("version")
{
}

Value* VersionFunction::evaluate(Context*)
{
	QList<Value*> version;
	QString v=STRINGIFY(RAPCAD_VERSION);
	QStringList parts=v.split(".");
	int major=parts.at(0).toInt();
	int minor=parts.at(1).toInt();
	QString build=parts.at(2);
	version.append(new NumberValue(major));
	version.append(new NumberValue(minor));
	if(build=="git") {
		version.append(new TextValue(build));
		QString revision=parts.at(3);
		version.append(new TextValue(revision));
	} else {
		version.append(new NumberValue(build.toInt()));
	}

	return new VectorValue(version);
}
