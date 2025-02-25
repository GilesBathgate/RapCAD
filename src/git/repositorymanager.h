#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H

#include "repository.h"

#include <git2.h>
#include <QString>

class RepositoryManager
{
public:
	RepositoryManager();
	~RepositoryManager();
	const Repository& getRepository(const QDir&);
private:
	const Repository& openRepository(const QDir&);
	QHash<QString,Repository*> repositories;
	int state;
};

#endif // REPOSITORYMANAGER_H
