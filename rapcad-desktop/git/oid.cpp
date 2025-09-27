#include "oid.h"

Oid::Oid()
{
}

QString Oid::toHex()
{
	const size_t n=GIT_OID_HEXSZ+1;
	QByteArray buffer(n,0);
	git_oid_tostr(buffer.data(),n,&oid);
	return QString::fromLocal8Bit(buffer.data(),n);
}
