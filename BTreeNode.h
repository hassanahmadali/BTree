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
	BTreeType_t* keys_;
	/**
	 * \brief the degree of the BTree
	 */
	long long  t_;
	/**
	 * \brief Array of Pointers to the children of the current node
	 */
	BTreeNode** child_;
	/**
	 * \brief the current number of keys
	 */
	long long  n_;
	/**
	 * \brief Boolean check if this node is a leaf
	 */
	bool isLeaf_;
public:
	/**
	 * \brief Constructor
	 * \param _t  the degree of the BTree
	 * \param _isLeaf  Boolean check if this node is a leaf
	 */
	BTreeNode(const size_t _t, const bool _isLeaf);
	/**
	 * \brief insert a new Element to this node if it is not Full
	 * \param x a new Element to be inserted
	 */
	void insert(BTreeType_t x);

	/**
	 * \brief Split the child 'node' of the Currnet node which must be full
	 * \param ind is the index of 'node'
	 * \param node is the node to be splitted
	 */
	void splitChild(const size_t ind, BTreeNode* node);
	/**
	 * \brief traverse across the nodes in the current tree
	 */
	void traverse() const;

	/**
	 * \brief Search for an Element
	 * \param x the Element Searched for
	 */
	BTreeNode* search(const BTreeType_t x);

	/**
	 * A utility function that returns the index of the first key that is
	 * greater than or equal to k
	 */
	size_t findKey(BTreeType_t x);

	/**
	 * \brief remove an Element x from the tree
	 * \param x the Element to be removed from the tree
	 */
	void remove(BTreeType_t x);
	void removeLeafNode(size_t ind);
	void removeNonLeafNode(size_t ind);

	/**
	 * \brief get the Predecessor of the keys[ind]
	 * \param ind the index of the key to get the predecessor to
	 */
	BTreeType_t getPredecessor(size_t ind);

	/**
	 * \brief get the Successor of the keys[ind]
	 * \param ind the index of the key to get the Successor to
	 */
	BTreeType_t getSuccessor(size_t ind);

	// A function to fill child child[ind] which has less than t-1 keys
	/**
	 * \brief fill the child number ind
	 */
	void fill(size_t ind);

	/**
	 * \brief A function to replace the element child[ind] with child[ind-1]
	 */
	void borrowFromPrev(size_t ind);
	/**
	 * \brief A function to replace the element child[ind] with child[ind-1]
	 */
	void borrowFromNext(size_t ind);
	
	/**
	 * \brief A function to merge child[ind] with child[ind+1] and remove child[ind+1]
	 */
	void merge(const size_t ind);

	friend class BTree;
};

#endif // BTREENODE_H
