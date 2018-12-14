///*
#include "ScoreList.h"
#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	ScoreList a;
	assert(a.size() == 0);
	assert(a.minimum() == NO_SCORE);
	assert(a.maximum() == NO_SCORE);
	assert(!a.remove(1));
	assert(!a.add(-1));
	assert(!a.add(101));
	assert(a.add(90));
	assert(a.size() == 1);
	assert(a.maximum() == 90);
	assert(a.maximum() == 90);
	assert(!a.remove(85));
	assert(a.remove(90));
	assert(a.size() == 0);
	assert(a.minimum() == NO_SCORE);
	assert(a.maximum() == NO_SCORE);

	a.add(30); a.add(50); a.add(70); a.add(90);
	assert(a.maximum() == 90);
	assert(a.minimum() == 30);
	assert(a.size() == 4);
	
	ScoreList b;
	for (unsigned long i = 0; i < DEFAULT_MAX_ITEMS; i++)
		b.add(i);
	assert(b.size() == 101);

	ScoreList c;
	for (unsigned long i = 0; i < DEFAULT_MAX_ITEMS; i++)
		c.add(1);
	assert(c.size() == DEFAULT_MAX_ITEMS);
	assert(!c.add(50));

	ScoreList d(c);
	assert(d.size() == DEFAULT_MAX_ITEMS);
	assert(!d.add(50));

	ScoreList e = c;
	assert(e.size() == DEFAULT_MAX_ITEMS);
	assert(!e.add(50));
}

int main()
{
	test();
	cerr << "Passed all tests for ScoreList" << endl;
}

//*/