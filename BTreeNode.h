#ifndef BTREENODE_H
#define BTREENODE_H
#pragma once
#include <iostream>
using std::cout;
using BTreeType_t = char;

class BTreeNode
{
protected:
	/**
	 * \brief Array Of Keys to the Elements
	 */
	BTreeType_t* keys;
	/**
	 * \brief the degree of the BTree
	 */
	int t;
	/**
	 * \brief Array of Pointers to the children of the current node
	 */
	BTreeNode** child;
	/**
	 * \brief the current number of keys
	 */
	int n;
	/**
	 * \brief Boolean check if this node is a leaf
	 */
	bool isLeaf;
public:
	/**
	 * \brief Constructor
	 * \param _t  the degree of the BTree
	 * \param _isLeaf  Boolean check if this node is a leaf
	 */
	BTreeNode(const size_t _t, const bool _isLeaf) :keys(new char[2 * _t - 1]), t(_t), child(new BTreeNode* [2 * t]), n(0), isLeaf(_isLeaf) {}
	/**
	 * \brief insert a new Element to this node if it is not Full
	 * \param x a new Element to be inserted
	 */
	void insert(BTreeType_t
		x)
	{
		size_t i = n - 1;
		if (isLeaf)
		{
			while (i >= 0 && x < keys[i])
			{
				keys[i + 1] = keys[i];
				i--;
			}
			keys[i + 1] = x;
			n = n + 1;
		}
		else
		{
			while (i >= 0 && x < keys[i])
				i--;
			if (child[i + 1]->n == 2 * t - 1)
			{
				splitChild(i + 1, child[i + 1]);
				if (keys[i + 1] < x)
					i++;
			}
			child[i + 1]->insert(x);
		}
	}

	/**
	 * \brief Split the child 'node' of the Currnet node which must be full 
	 * \param ind is the index of 'node'  
	 * \param node is the node to be splitted 
	 */
	void splitChild(const size_t ind, BTreeNode* node)
	{
		BTreeNode* z = new BTreeNode(node->t, node->isLeaf);
		z->n = t - 1;
		for (size_t j = 0; j < t - 1; j++)
			z->keys[j] = node->keys[j + t];
		if (!node->isLeaf)
		{
			for (size_t j = 0; j < t; j++)
				z->child[j] = node->child[j + t];
		}
		node->n = t - 1;
		for (size_t j = n; 1 + ind <= j; j--)
			child[j + 1] = child[j];
		child[ind + 1] = z;
		for (size_t j = n - 1; j >= ind; j--)
			keys[j + 1] = keys[j];
		keys[ind] = node->keys[t - 1];
		n = n + 1;
	}
	/**
	 * \brief traverse across the nodes in the current tree
	 */
	void traverse() const
	{
		size_t i;
		for (i = 0; i < n; i++)
		{
			if (!isLeaf) {

				child[i]->traverse();
			}
			std::cout << " '" << keys[i] << "' ";
		}
		if (!isLeaf)
			child[i]->traverse();
	}

	/**
	 * \brief Search for an Element
	 * \param x the Element Searched for
	 * \return
	 */
	BTreeNode* search(const BTreeType_t x)
	{
		size_t ind = 0;
		while (ind < n && keys[ind] < x)
			ind++;
		if (keys[ind] == x)
			return this;
		if (isLeaf)
			return nullptr;
		return child[ind]->search(x);
	}

	/*
	 * A utility function that returns the index of the first key that is
	 * greater than or equal to k
	 */
	size_t findKey(BTreeType_t x)
	{
		size_t res = 0;
		while (res < n && keys[res] < x)
			++res;
		return res;
	}


	// A function to remove the key k from the sub-tree rooted with this node
	void remove(BTreeType_t x)
	{
		size_t ind = findKey(x);

		// The key to be removed is present in this node
		if (ind < n && keys[ind] == x)
		{

			// If the node is a leaf node - removeFromLeaf is called
			// Otherwise, removeFromNonLeaf function is called
			if (isLeaf)
				removeFromLeaf(ind);
			else
				removeFromNonLeaf(ind);
		}
		else
		{

			// If this node is a leaf node, then the key is not present in tree
			if (isLeaf)
			{
				std::cout << "The key " << x << " is does not exist in the tree\n";
				return;
			}

			// The key to be removed is present in the sub-tree rooted with this node
			// The flag indicates whether the key is present in the sub-tree rooted
			// with the last child of this node
			bool flag = (ind == n);

			// If the child where the key is supposed to exist has less that t keys,
			// we fill that child
			if (child[ind]->n < t)
				fill(ind);

			// If the last child has been merged, it must have merged with the previous
			// child and so we recurse on the (ind-1)th child. Else, we recurse on the
			// (ind)th child which now has atleast t keys
			if (flag && ind > n)
				child[ind - 1]->remove(x);
			else
				child[ind]->remove(x); 
		}	
		return;
	}

	void removeFromLeaf(size_t ind)
	{

		for (size_t i = ind + 1; i < n; ++i)
			keys[i - 1] = keys[i];
		n--;
		return;
	}

	void removeFromNonLeaf(size_t ind)
	{

		BTreeType_t k = keys[ind];

		// If the child that precedes k (child[ind]) has atleast t keys,
		// find the predecessor 'pred' of k in the subtree rooted at
		// child[ind]. Replace k by pred. Recursively delete pred
		// in child[ind]
		if (child[ind]->n >= t)
		{
			BTreeType_t dump = getPred(ind);
			keys[ind] = dump;
			child[ind]->remove(dump);
		}

		// If the child child[ind] has less than t keys, examine child[ind+1].
		// If child[ind+1] has atleast t keys, find the successor 'succ' of k in
		// the subtree rooted at child[ind+1]
		// Replace k by succ
		// Recursively delete succ in child[ind+1]
		else if (child[ind + 1]->n >= t)
		{
			BTreeType_t dump = getSucc(ind);
			keys[ind] = dump;
			child[ind + 1]->remove(dump);
		}

		// If both child[ind] and child[ind+1] has less that t keys,merge k and all of child[ind+1]
		// into child[ind]
		// Now child[ind] contains 2t-1 keys
		// Free child[ind+1] and recursively delete k from child[ind]
		else
		{
			merge(ind);
			child[ind]->remove(k);
		}
		return;
	}

	// A function to get predecessor of keys[ind]
	BTreeType_t getPred(size_t ind)
	{
		// Keep moving to the right most node until we reach a leaf
		BTreeNode* cur = child[ind];
		while (!cur->isLeaf)
			cur = cur->child[cur->n];

		// Return the last key of the leaf
		return cur->keys[cur->n - 1];
	}

	BTreeType_t getSucc(size_t ind)
	{

		// Keep moving the left most node starting from child[ind+1] until we reach a leaf
		BTreeNode* cur = child[ind + 1];
		while (!cur->isLeaf)
			cur = cur->child[0];

		// Return the first key of the leaf
		return cur->keys[0];
	}

	// A function to fill child child[ind] which has less than t-1 keys
	void fill(size_t ind)
	{

		// If the previous child(child[ind-1]) has more than t-1 keys, borrow a key
		// from that child
		if (ind != 0 && child[ind - 1]->n >= t)
			borrowFromPrev(ind);

		// If the next child(child[ind+1]) has more than t-1 keys, borrow a key
		// from that child
		else if (ind != n && child[ind + 1]->n >= t)
			borrowFromNext(ind);

		// Merge child[ind] with its sibling
		// If child[ind] is the last child, merge it with its previous sibling
		// Otherwise merge it with its next sibling
		else
		{
			if (ind != n)
				merge(ind);
			else
				merge(ind - 1);
		}
		return;
	}

	// A function to borrow a key from child[ind-1] and insert it
	// into child[ind]
	void borrowFromPrev(size_t ind)
	{

		BTreeNode* C = child[ind];
		BTreeNode* sibling = child[ind - 1];

		// The last key from child[ind-1] goes up to the parent and key[ind-1]
		// from parent is inserted as the first key in child[ind]. Thus, the loses
		// sibling one key and child gains one key

		// Moving all key in C[ind] one step ahead
		for (size_t i = C->n - 1; i >= 0; --i)
			C->keys[i + 1] = C->keys[i];

		// If C[ind] is not a leaf, move all its C pointers one step ahead
		if (!C->isLeaf)
		{
			for (size_t i = C->n; i >= 0; --i)
				C->child[i + 1] = C->child[i];
		}

		// Setting C's first key equal to keys[ind-1] from the current node
		C->keys[0] = keys[ind - 1];

		// Moving sibling's last C as C[ind]'s first C
		if (!C->isLeaf)
			C->child[0] = sibling->child[sibling->n];

		// Moving the key from the sibling to the parent
		// This reduces the number of keys in the sibling
		keys[ind - 1] = sibling->keys[sibling->n - 1];

		C->n += 1;
		sibling->n -= 1;

		return;
	}

	// A function to borrow a key from the child[ind+1] and place
	// it in child[ind]
	void borrowFromNext(size_t ind)
	{

		BTreeNode* C = child[ind];
		BTreeNode* sibling = child[ind + 1];

		// keys[ind] is inserted as the last key in C[ind]
		C->keys[(C->n)] = keys[ind];

		// Sibling's first C is inserted as the last C
		// into C[ind]
		if (!(C->isLeaf))
			C->child[(C->n) + 1] = sibling->child[0];

		// The first key from sibling is inserted into keys[ind]
		keys[ind] = sibling->keys[0];

		// Moving all keys in sibling one step behind
		for (size_t i = 1; i < sibling->n; ++i)
			sibling->keys[i - 1] = sibling->keys[i];

		// Moving the child pointers one step behind
		if (!sibling->isLeaf)
		{
			for (size_t i = 1; i <= sibling->n; ++i)
				sibling->child[i - 1] = sibling->child[i];
		}

		// Increasing and decreasing the key count of child[ind] and child[ind+1]
		// respectively
		C->n += 1;
		sibling->n -= 1;

		return;
	}

	// A function to merge child[ind] with child[ind+1]
	// child[ind+1] is freed after merging
	void merge(size_t ind)
	{
		BTreeNode* C = child[ind];
		BTreeNode* sibling = child[ind + 1];

		// Pulling a key from the current node and inserting it into (t-1)th
		// position of child[ind]
		C->keys[t - 1] = keys[ind];

		// Copying the keys from child[ind+1] to child[ind] at the end
		for (size_t i = 0; i < sibling->n; ++i)
			C->keys[i + t] = sibling->keys[i];

		// Copying the child pointers from child[ind+1] to child[ind]
		if (!C->isLeaf)
		{
			for (size_t i = 0; i <= sibling->n; ++i)
				C->child[i + t] = sibling->child[i];
		}

		// Moving all keys after ind in the current node one step before -
		// to fill the gap created by moving keys[ind] to child[ind]
		for (size_t i = ind + 1; i < n; ++i)
			keys[i - 1] = keys[i];

		// Moving the child pointers after (ind+1) in the current node one
		// step before
		for (size_t i = ind + 2; i <= n; ++i)
			child[i - 1] = child[i];

		// Updating the key count of child and the current node
		C->n += sibling->n + 1;
		n--;

		// Freeing the memory occupied by sibling
		delete (sibling);
		return;
	}

	friend class BTree;
};

#endif // BTREENODE_H