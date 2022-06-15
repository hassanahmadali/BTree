#include "Btree.h"
/**
 * \brief Constructor
 * \param _t the degree of the BTree
 */
BTree::BTree(const long long _t) : root(nullptr), t(_t) {}
/**
 *  \brief traverse across the nodes in the current tree
 */
void BTree::traverse() const
{
	if (root != nullptr)
		root->traverse();
}
BTreeNode *BTree::search(const BTreeType_t x) const
{
	if ((root == nullptr))
		return nullptr;
	return root->search(x);
}
/**
 * \brief insert a new Element to the current Tree
 * \param x the Element to be inserted
 */
void BTree::insert(char x)
{
	if (root == nullptr)
	{
		root = new BTreeNode(t, true);
		root->keys_[0] = x;
		root->n_ = 1;
	}
	else
	{
		if (root->n_ == 2 * t - 1)
		{
			BTreeNode *s = new BTreeNode(t, false);
			s->child_[0] = root;
			s->splitChild(0, root);
			long long i = 0;
			if (s->keys_[0] < x)
				i++;
			s->child_[i]->insert(x);
			root = s;
		}
		else
			root->insert(x);
	}
}
void BTree::remove(BTreeType_t k)
{
	if (!root)
	{
		std::cout << "The tree is empty\n";
		return;
	}
	root->remove(k);
	if (root->n_ == 0)
	{
		BTreeNode *dump = root;
		if (root->isLeaf_)
			root = nullptr;
		else
			root = root->child_[0];

		delete dump;
	}
	return;
}