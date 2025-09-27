#include "index.h"
#include "repository.h"
#include "gitexception.h"

Index::Index(const Repository& r)
{
	const auto error=git_repository_index(&index, r.repo);
	GitException::checkError(error);
}

Index::~Index()
{
	git_index_free(index);
}

void Index::addByPath(const QString& path)
{
	const auto error=git_index_add_bypath(index, path.toUtf8());
	GitException::checkError(error);
}

void Index::write()
{
	const auto error=git_index_write(index);
	GitException::checkError(error);
}

Oid Index::writeTree()
{
	Oid treeId;
	const auto error=git_index_write_tree(&treeId.oid, index);
	GitException::checkError(error);
	return treeId;
}
