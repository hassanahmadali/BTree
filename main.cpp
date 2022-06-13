#include "Btree.h"
using namespace std;

int main()
{
	BTree t(3);
	for (const char i : /*"Created by Hassan Ahmad Ali in 2022-06-07"*/ "12")
		if (i != '\0')
			t.insert(i);
	cout << "Traversal of the tree is " << endl << "[";
	t.traverse();
	cout << "]" << endl;
	for (const char i : "ac")
		if (i != '\0')
			cout << "the Element '" << i << "' is " << ((t.search(i) != nullptr) ? "found" : "Not found") << endl;

	return 0;
}