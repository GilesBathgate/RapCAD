#include "repositorymanager.h"

RepositoryManager::RepositoryManager()
{
	state=git_libgit2_init();
}

RepositoryManager::~RepositoryManager()
{
	qDeleteAll(repositories);

	state=git_libgit2_shutdown();
}

const Repository& RepositoryManager::getRepository(const QDir& path)
{
	const auto found=repositories.find(path.absolutePath());
	if (found==repositories.end())
		return openRepository(path);

	return *found.value();
}

const Repository& RepositoryManager::openRepository(const QDir& path)
{
	auto repository=new Repository(path);
	const auto added=repositories.insert(path.absolutePath(),repository);
	return *added.value();
}
