#include "tree.h"

Tree::Tree()
{
}

Tree::~Tree()
{
	git_tree_free(tree);
}
