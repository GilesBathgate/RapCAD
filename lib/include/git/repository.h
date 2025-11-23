#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "tree.h"
#include "oid.h"
#include "commit.h"

#include <QString>
#include <QDir>
#include <git2.h>

class Repository
{
public:
	explicit Repository(const QDir& directory);
	~Repository();
	Tree lookupTree(const Oid& id);
	Oid createCommit(const QString& ref,const QString& message,const Tree& tree);
	QString relativeFilePath(const QString& path);
private:
	Commit getParent(const QString& ref);

	friend class Index;
	friend class Commit;
	friend class Signature;
	QDir directory;
	git_repository* repo;

};

#endif // REPOSITORY_H
