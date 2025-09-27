#ifndef COMMIT_H
#define COMMIT_H

#include <git2.h>
#include "oid.h"

class Commit
{
public:
	explicit Commit();
	Commit(const class Repository& repo, const Oid& oid);
	~Commit();
private:
	friend class Repository;
	git_commit* commit;
};

#endif // COMMIT_H
