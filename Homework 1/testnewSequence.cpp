#include "newSequence.h"
#include "ScoreList.h"
#include <cassert>
#include <iostream>
#include <string>
using namespace std;


void test()
{
	/*
	Sequence a;
	assert(a.empty());
	assert(!a.erase(0));
	assert(a.insert("A") == 0);
	assert(a.insert("B") == 1);
	assert(a.insert(1, "C"));
	assert(a.find("B") == 2);
	assert(!a.insert(10, "A"));
	a.insert(1, "B");
	assert(a.find("B") == 1);
	assert(a.erase(1));
	assert(a.size() == 3);
	string temp;
	assert(a.get(1, temp));
	assert(temp == "C");
	assert(a.set(1, "D"));
	assert(a.get(1, temp));
	assert(temp == "D");

	Sequence b, c;
	b.insert(0, "A");
	b.insert(1, "B");
	b.insert(2, "C");
	for (int i = 0; i < 5; i++)
		c.insert(i, "agea");
	b.swap(c);

	assert(b.size() == 5);
	assert(c.size() == 3);

	for (int i = 0; i < 5; i++)
	{
		assert(b.get(i, temp));
		assert(temp == "agea");
	}

	assert(c.get(0, temp));
	assert(temp == "A");
	assert(c.get(1, temp));
	assert(temp == "B");
	assert(c.get(2, temp));
	assert(temp == "C");

	assert(c.find("D") == -1);
	assert(!c.get(3, temp));
	assert(!c.set(3, "D"));
	assert(c.remove("D") == 0);
	assert(b.remove("agea") == 5);

	Sequence d, e;
	d.insert("A");
	d.insert("B");
	d.insert("C");
	e = d;

	assert(e.find("A") == 0);
	assert(e.size() == 3);
	d.swap(e);
	assert(d.find("A") == 0);
	assert(d.find("B") == 1);
	assert(d.find("C") == 2);

	Sequence f(e);
	assert(f.size() == 3);
	assert(f.find("A") == 0);
	assert(f.find("B") == 1);
	assert(f.find("C") == 2); */

	Sequence a(1000);   // a can hold at most 1000 items
	Sequence b(5);      // b can hold at most 5 items
	Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
	ItemType v = 1;

	// No failures inserting 5 items into b
	for (int k = 0; k < 5; k++)
		assert(b.insert(k, v));

	// Failure if we try to insert a sixth item into b
	assert(!b.insert(5, v));

	// When two Sequences' contents are swapped, their capacities are
	// swapped as well:
	a.swap(b);
	assert(!a.insert(5, v));
	for (int k = 0; k < 1000; k++)
		assert(b.insert(k, v));

	// Operator test
	c = a;
	assert(c.size() == 5);
	assert(!c.insert(5, v));

	// Copy constructor test
	Sequence d(a);
	assert(d.size() == 5);
	assert(!d.insert(5, v)); 
}

int main()
{
	test();
	std::cerr << "Passed all tests for newSequence" << std::endl;
}