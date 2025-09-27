#include "commit.h"
#include "repository.h"
#include "gitexception.h"

Commit::Commit()
	: commit(nullptr)
{
}

Commit::Commit(const Repository& r, const Oid& o)
{
	const auto error=git_commit_lookup(&commit, r.repo, &o.oid);
	GitException::checkError(error);
}

Commit::~Commit()
{
	git_commit_free(commit);
}
