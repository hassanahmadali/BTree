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
		int t;
	public:
		/**
		 * \brief Constructor
		 * \param _t the degree of the BTree
		 */
		explicit BTree(const int _t) : root(nullptr), t(_t) {}

		/**
		 *  \brief traverse across the nodes in the current tree
		 */
		void traverse() const
		{
			if (root != nullptr) root->traverse();
		}


		BTreeNode* search(const BType_t x) const
		{
			if ((root == nullptr))
				return nullptr;
			return root->search(x);
		}

		/**
		 * \brief insert a new Element to the current Tree
		 * \param x the Element to be inserted
		 */
		void insert(char x)
		{
			if (root == nullptr)
			{
				root = new BTreeNode(t, true);
				root->keys[0] = x;
				root->n = 1;
			}
			else
			{
				if (root->n == 2 * t - 1)
				{
					BTreeNode* s = new BTreeNode(t, false);
					s->child[0] = root;
					s->splitChild(0, root);
					int i = 0;
					if (s->keys[0] < x)
						i++;
					s->child[i]->insert(x);
					root = s;
				}
				else
					root->insert(x);
			}
		}


		void remove(BType_t k)
		{
			if (!root)
			{
				std::cout << "The tree is empty\n";
				return;
			}

			// Call the remove function for root
			root->remove(k);

			// If the root node has 0 keys, make its first child as the new root
			// if it has a child, otherwise set root as NULL
			if (root->n == 0)
			{
				BTreeNode* tmp = root;
				if (root->isLeaf)
					root = NULL;
				else
					root = root->child[0];

				// Free the old root
				delete tmp;
			}
			return;
		}
	};

#endif // BTREE_H
