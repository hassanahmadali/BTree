#ifndef BTREE_H
#define BTREE_H
#pragma once
#include "BTreeNode.h"

	class BTree
	{
		BTreeNode* root;
		/**
		 * \brief the degree of the BTree
		 */
		long long  t;
	public:
		/**
		 * \brief Constructor
		 * \param _t the degree of the BTree
		 */
		explicit BTree(const long long  _t);

		/**
		 *  \brief traverse across the nodes in the current tree
		 */
		void traverse() const;


		BTreeNode* search(const BTreeType_t x) const;

		/**
		 * \brief insert a new Element to the current Tree
		 * \param x the Element to be inserted
		 */
		void insert(char x);

		
		void remove(BTreeType_t k);
	};

#endif // BTREE_H
