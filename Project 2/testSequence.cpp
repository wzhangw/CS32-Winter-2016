#include "Sequence.h"
#include <cassert>
#include <iostream>
#include <string>

int main()
{
//=============================================================================================================
	// Create an empty sequence
	Sequence a;
	assert(a.empty());
	assert(a.size() == 0);
	assert(a.find("anything") == -1);
	assert(!a.insert(1, "glucose"));
	assert(!a.insert(-1, "glucose"));
	assert(!a.erase(0));
	assert(a.remove("anything") == 0);

	// Insert things into the empty sequence
	assert(a.insert(0, "glucose"));
	assert(!a.empty());
	assert(a.size() == 1);
	assert(a.insert(0, "galactose"));
	assert(a.insert(2, "pyruvate"));
	assert(a.size() == 3);
	assert(a.insert(2, "glycine"));
	assert(a.size() == 4);
	std::string check;
	assert(a.get(0, check));
	assert(check == "galactose");
	assert(a.get(3, check));
	assert(check == "pyruvate");
	assert(a.find("galactose") == 0);
	assert(a.find("glucose") == 1);
	assert(a.find("glycine") == 2);
	assert(a.find("pyruvate") == 3);
	assert(a.find("phenol") == -1);

	assert(a.insert("alanine") == 0);
	assert(a.find("alanine") == 0);
	assert(a.insert("serine") == 5);
	assert(a.size() == 6);

	Sequence b;

	b.insert("gravity");
	b.insert("potential");
	b.insert("relativity");
	b.insert("gyroscope");

	a.swap(b);

	// a.dump();
	// b.dump();

	Sequence c = a;
	Sequence d(b);

	// c.dump();
	// d.dump();

	d.erase(2);
	d.erase(4);
	d.erase(0);
	assert(!d.erase(3));
	for (int i = 0; i < 3; i++)
		d.insert("glycine");
	assert(d.remove("glycine") == 4);
	assert(d.remove("alanine") == 0);

	d.dump();

	assert(!d.set(2, "idk"));
	assert(d.set(1, "malate"));

	d.dump();
	

	/*
	int lib1[10] = {30, 21, 63, 42, 17, 63, 17, 29, 8, 32};
	int lib2[3] = { 63,17,29 };
	int lib3[3] = { 17,63,29 };
	Sequence a; Sequence b; Sequence c;
	for (int i = 0; i < 10; i++)
		a.insert(i, lib1[i]);
	for (int i = 0; i < 3; i++)
		b.insert(i, lib2[i]);
	for (int i = 0; i < 3; i++)
		c.insert(i, lib3[i]);
	assert(subsequence(a, b) == 5);
	assert(subsequence(a, c) == -1);

	int lib4[6] = { 30, 21, 63, 42, 17, 63 };
	int lib5[4] = { 42, 63, 84, 19 };
	Sequence d; Sequence e;
	for (int i = 0; i < 6; i++)
		d.insert(i, lib4[i]);
	for (int i = 0; i < 4; i++)
		e.insert(i, lib5[i]);
	Sequence disp;
	interleave(d, e, disp);
	ItemType obj;
	for (int i = 0; i < disp.size(); i++)
	{
		disp.get(i, obj);
		std::cerr << obj << std::endl;
	}

	std::cerr << "===============================================" << std::endl;
	std::cerr << "Passed all tests" << std::endl;

	*/

	
}