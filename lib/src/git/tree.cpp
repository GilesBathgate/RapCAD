#include "tree.h"

Tree::Tree()
	: tree(nullptr)
{
}

Tree::~Tree()
{
	git_tree_free(tree);
}
