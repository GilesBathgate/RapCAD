#ifndef OID_H
#define OID_H

#include <git2.h>
#include <QString>

class Oid
{
public:
	Oid();
	QString toHex();
private:
	friend class Index;
	friend class Repository;
	friend class Commit;
	git_oid oid;
};

#endif // OID_H
