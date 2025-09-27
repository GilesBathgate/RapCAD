#include "repository.h"
#include "signature.h"
#include "gitexception.h"

Repository::Repository(const QDir& directory)
	: directory(directory)
{

	const auto error=git_repository_open(&repo, directory.absolutePath().toUtf8());
	GitException::checkError(error);
}

Repository::~Repository()
{
	git_repository_free(repo);
}

Tree Repository::lookupTree(const Oid& treeId)
{
	Tree result;
	const auto error=git_tree_lookup(&result.tree, repo, &treeId.oid);
	GitException::checkError(error);
	return result;
}

Commit Repository::getParent(const QString& ref)
{
	Oid parent;
	const auto error=git_reference_name_to_id(&parent.oid, repo, ref.toUtf8());
	if(error==GIT_ENOTFOUND)
		return Commit();

	return Commit(*this, parent);
}

Oid Repository::createCommit(const QString& ref, const QString& message, const Tree& t)
{
	const auto parent=getParent(ref);

	Signature author(*this);

	Oid commitId;
	const auto error=git_commit_create_v(&commitId.oid, repo, ref.toUtf8(), author.signature, author.signature, "utf-8", message.toUtf8(), t.tree, 1, parent.commit);
	GitException::checkError(error);

	return commitId;
}

QString Repository::relativeFilePath(const QString& path)
{
	return directory.relativeFilePath(path);
}
