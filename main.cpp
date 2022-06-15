#include "Btree.h"
using namespace std;
/*
 * the variable type BTreeType_t is the type of the Data Contained in the BTree
 * you can edit the type of the BTree by editing the line number 4 in the file "BTreeNode.h"
*/

int  main()
{

	BTree t(23);
	BTreeType_t temp[] = "Created by Hassan Ahmad Ali in 2022-06-15";
	for (const char i : temp)
		if (i != '\0')
			t.insert(i);

	cout << "Traversal of the tree is \n[";
	t.traverse();
	cout << "]\n";
	for (const char i : "ac")
		if (i != '\0')
			cout << "the Element '" << i << "' is " << ((t.search(i) != nullptr) ? "" : "Not ") << "found\n" ;
	for (const char i : "HAC")
		if (i != '\0')
			t.remove(i);

	cout << "Traversal of the tree is \n[";
	t.traverse();
	cout << "]\n";

	return 0;
}
