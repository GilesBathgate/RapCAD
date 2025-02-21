#ifndef INDEX_H
#define INDEX_H

#include "repository.h"
#include "oid.h"

#include <QString>
#include <git2.h>

class Index
{
public:
	Index(const Repository&);
	~Index();
	void addByPath(const QString& path);
	void write();
	Oid writeTree();
private:
	git_index* index;
};

#endif // INDEX_H
