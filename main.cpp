#include "Btree.h"
using namespace std;

int  main()
{

	BTree t(23);
	for (const char i : "Created by Hassan Ahmad Ali in 2022-06-07")

		if (i != '\0')
			t.insert(i);

	cout << "Traversal of the tree is " << endl << "[";
	t.traverse();
	cout << "]" << endl;
	for (const char i : "ac")
		if (i != '\0')
			cout << "the Element '" << i << "' is " << ((t.search(i) != nullptr) ? "" : "Not ") << "found" << endl;
	for (const char i : "H")
		if (i != '\0')
			t.remove(i);

	cout << "Traversal of the tree is \n[";
	t.traverse();

	return 0;
}
