#include "BTreeNode.h"
/*
 * \brief insert a new Element to this node if it is not Full
 * \param x a new Element to be inserted
 */
BTreeNode::BTreeNode(const size_t _t, const bool _isLeaf) : keys_(new char[2 * _t - 1]), t_(_t), child_(new BTreeNode* [2 * t_]), n_(0), isLeaf_(_isLeaf)
{
}
void BTreeNode::insert(BTreeType_t x)
{
	size_t i = n_ - 1;
	if (isLeaf_)
	{
		while (0 <= i && x < keys_[i])
		{
			keys_[i + 1] = keys_[i];
			i--;
		}
		keys_[i + 1] = x;
		n_ = n_ + 1;
	}
	else
	{
		while (0 <= i && x < keys_[i])
			i--;
		if (child_[i + 1]->n_ == 2 * t_ - 1)
		{
			splitChild(i + 1, child_[i + 1]);
			if (keys_[i + 1] < x)
				i++;
		}
		child_[i + 1]->insert(x);
	}
}
/**
 * \brief Split the child 'node' of the Currnet node which must be full
 * \param ind is the index of 'node'
 * \param node is the node to be splitted
 */
void BTreeNode::splitChild(const size_t ind, BTreeNode* node)
{
	BTreeNode* dump = new BTreeNode(node->t_, node->isLeaf_);
	dump->n_ = t_ - 1;
	for (size_t j = 0; j < t_ - 1; j++)
		dump->keys_[j] = node->keys_[j + t_];
	if (!node->isLeaf_)
	{
		for (size_t j = 0; j < t_; j++)
			dump->child_[j] = node->child_[j + t_];
	}
	node->n_ = t_ - 1;
	for (size_t j = n_; 1 + ind <= j; j--)
		child_[j + 1] = child_[j];
	child_[ind + 1] = dump;
	for (size_t j = n_ - 1; j >= ind; j--)
		keys_[j + 1] = keys_[j];
	keys_[ind] = node->keys_[t_ - 1];
	n_ = n_ + 1;
}
/*
 * \brief traverse across the nodes in the current tree
 */
void BTreeNode::traverse() const
{
	size_t i;
	for (i = 0; i < n_; i++)
	{
		if (!isLeaf_)
		{
			child_[i]->traverse();
		}
		std::cout << " '" << keys_[i] << "' ";
	}
	if (!isLeaf_)
		child_[i]->traverse();
}
/*
 * \brief Search for an Element
 * \param x the Element Searched for
 */
BTreeNode* BTreeNode::search(const BTreeType_t x)
{
	size_t ind = 0;
	while (ind < n_ && keys_[ind] < x)
		ind++;
	if (keys_[ind] == x)
		return this;
	if (isLeaf_)
		return nullptr;
	return child_[ind]->search(x);
}
/*
 * A utility function that returns the index of the first key that is
 * greater than or equal to k
 */
size_t BTreeNode::findKey(BTreeType_t x)
{
	size_t res = 0;
	while (res < n_ && keys_[res] < x)
		++res;
	return res;
}
void BTreeNode::remove(BTreeType_t x)
{
	size_t ind = findKey(x);
	if (ind < n_ && keys_[ind] == x)
	{
		if (isLeaf_)
			removeLeafNode(ind);
		else
			removeNonLeafNode(ind);
	}
	else
	{
		if (isLeaf_)
		{
			std::cout << "The key " << x << " is not existed in this Btree\n";
			return;
		}
		bool temp = (ind == n_);
		if (child_[ind]->n_ < t_)
			fill(ind);
		if (temp && ind > n_)
			child_[ind - 1]->remove(x);
		else
			child_[ind]->remove(x);
	}
	return;
}
void BTreeNode::removeLeafNode(size_t ind)
{
	for (size_t i = ind + 1; i < n_; ++i)
		keys_[i - 1] = keys_[i];
	n_--;
	return;
}
void BTreeNode::removeNonLeafNode(size_t ind)
{
	BTreeType_t k = keys_[ind];
	if (child_[ind]->n_ >= t_)
	{
		BTreeType_t dump = getPredecessor(ind);
		keys_[ind] = dump;
		child_[ind]->remove(dump);
	}
	else if (child_[ind + 1]->n_ >= t_)
	{
		BTreeType_t dump = getSuccessor(ind);
		keys_[ind] = dump;
		child_[ind + 1]->remove(dump);
	}
	else
	{
		merge(ind);
		child_[ind]->remove(k);
	}
	return;
}
BTreeType_t BTreeNode::getPredecessor(size_t ind)
{
	BTreeNode* cur = child_[ind];
	while (!cur->isLeaf_)
		cur = cur->child_[cur->n_];
	return cur->keys_[cur->n_ - 1];
}
BTreeType_t BTreeNode::getSuccessor(size_t ind)
{
	BTreeNode* cur = child_[ind + 1]; /*c => Current Node*/
	while (!cur->isLeaf_)
		cur = cur->child_[0];
	return cur->keys_[0];
}
void BTreeNode::fill(size_t ind)
{
	if (ind != 0 && child_[ind - 1]->n_ >= t_)
		borrowFromPrev(ind);
	else if (ind != n_ && child_[ind + 1]->n_ >= t_)
		borrowFromNext(ind);
	else
	{
		if (ind != n_)
			merge(ind);
		else
			merge(ind - 1);
	}
	return;
}
void BTreeNode::borrowFromPrev(size_t ind)
{
	BTreeNode* cur = child_[ind];
	BTreeNode* prev = child_[ind - 1];
	for (size_t i = cur->n_ - 1; 0 <= i; --i)
		cur->keys_[i + 1] = cur->keys_[i];
	if (cur->isLeaf_ == false)
	{
		for (size_t i = cur->n_; 0 <= i ; --i)
			cur->child_[i + 1] = cur->child_[i];
	}
	cur->keys_[0] = keys_[ind - 1];
	if (!cur->isLeaf_)
		cur->child_[0] = prev->child_[prev->n_];
	keys_[ind - 1] = prev->keys_[prev->n_ - 1];
	cur->n_ += 1;
	prev->n_ -= 1;
	return;
}
void BTreeNode::borrowFromNext(size_t ind)
{
	BTreeNode* cur = child_[ind];
	BTreeNode* prev = child_[ind + 1];
	cur->keys_[(cur->n_)] = keys_[ind];
	if (!(cur->isLeaf_))
		cur->child_[(cur->n_) + 1] = prev->child_[0];
	keys_[ind] = prev->keys_[0];
	for (size_t i = 1; i < prev->n_; ++i)
		prev->keys_[i - 1] = prev->keys_[i];
	if (!prev->isLeaf_)
	{
		for (size_t i = 1; i <= prev->n_; ++i)
			prev->child_[i - 1] = prev->child_[i];
	}
	cur->n_ += 1;
	prev->n_ -= 1;
	return;
}
void BTreeNode::merge(const size_t ind)
{
	BTreeNode* cur = child_[ind];
	BTreeNode* prev = child_[ind + 1];
	cur->keys_[t_ - 1] = keys_[ind];
	for (size_t i = 0; i < prev->n_; ++i)
		cur->keys_[i + t_] = prev->keys_[i];
	if (!cur->isLeaf_)
	{
		for (size_t i = 0; i <= prev->n_; ++i)
			cur->child_[i + t_] = prev->child_[i];
	}
	for (size_t i = ind + 1; i < n_; ++i)
		keys_[i - 1] = keys_[i];
	for (size_t i = ind + 2; i <= n_; ++i)
		child_[i - 1] = child_[i];
	cur->n_ += prev->n_ + 1;
	n_--;
	delete (prev);
	return;
}
