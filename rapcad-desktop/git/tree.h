#ifndef TREE_H
#define TREE_H

#include <git2.h>

class Tree
{
public:
	Tree();
	~Tree();
private:
	friend class Repository;
	git_tree* tree;
};

#endif // TREE_H
