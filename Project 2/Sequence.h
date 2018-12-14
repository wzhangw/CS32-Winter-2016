#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED
#include <string>

typedef std::string ItemType;

class Sequence
{
public:
	Sequence();    // Create an empty sequence (i.e., one whose size() is 0).

	~Sequence(); // Desctructor
	// When a Sequence is destroyed, the nodes in the linked list must be deallocated.

	Sequence(const Sequence& other); // Copy constructor
	// When a brand new Sequence is created as a copy of an existing Sequence, 
	// enough new nodes must be allocated to hold a duplicate of the original list.

	Sequence& operator=(const Sequence& other); // Assignment operator
	// When an existing Sequence (the left-hand side) is assigned the value 
	// of another Sequence (the right-hand side), the result must be that the 
	// left-hand side object is a duplicate of the right-hand side object, with 
	// no memory leak of list nodes (i.e. no list node from the old value of 
	// the left-hand side should be still allocated yet inaccessible).

	bool empty() const;  // Return true if the sequence is empty, otherwise false.

	int size() const;    // Return the number of items in the sequence.

	bool insert(int pos, const ItemType& value);
	// Insert value into the sequence so that it becomes the item at
	// position pos.  The original item at position pos and those that
	// follow it end up at positions one higher than they were at before.
	// Return true if 0 <= pos <= size() and the value could be
	// inserted.  (It might not be, if the sequence has a fixed capacity,
	// (e.g., because it's implemented using a fixed-size array) and is
	// full.)  Otherwise, leave the sequence unchanged and return false.
	// Notice that if pos is equal to size(), the value is inserted at the
	// end.

	int insert(const ItemType& value);
	// Let p be the smallest integer such that value <= the item at
	// position p in the sequence; if no such item exists (i.e.,
	// value > all items in the sequence), let p be size().  Insert
	// value into the sequence so that it becomes the item at position
	// p.  The original item at position p and those that follow it end
	// up at positions one higher than before.  Return p if the value
	// was actually inserted.  Return -1 if the value was not inserted
	// (perhaps because the sequence has a fixed capacity and is full).

	bool erase(int pos);
	// If 0 <= pos < size(), remove the item at position pos from
	// the sequence (so that all items that followed this item end up at
	// positions one lower than they were at before), and return true.
	// Otherwise, leave the sequence unchanged and return false.

	int remove(const ItemType& value);
	// Erase all items from the sequence that == value.  Return the
	// number of items removed (which will be 0 if no item == value).

	bool get(int pos, ItemType& value) const;
	// If 0 <= pos < size(), copy into value the item at position pos
	// in the sequence and return true.  Otherwise, leave value unchanged
	// and return false.

	bool set(int pos, const ItemType& value);
	// If 0 <= pos < size(), replace the item at position pos in the
	// sequence with value and return true.  Otherwise, leave the sequence
	// unchanged and return false.

	int find(const ItemType& value) const;
	// Let p be the smallest integer such that value == the item at
	// position p in the sequence; if no such item exists, let p be -1.
	// Return p.

	void swap(Sequence& other);
	// Exchange the contents of this sequence with the other one.
	
	void dump() const;
	// For debugging and testing purposes only

private:
	struct Node
	{
		ItemType m_item;
		Node* m_next; // points to the next node
		Node* m_prev; // points to the previous node
	};
	int m_size;
	Node* m_head;
};

// function declarations
int subsequence(const Sequence& seq1, const Sequence& seq2);
// find out whether seq2 is a subsequence of seq1. If yes, find the first
// position of seq1 at which elements of seq2 appear in order. Otherwise, 
// return -1.

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);
// make result a new sequence which takes elemetns from seq1 and seq2
// alternatively, one-by-one. If one sequence runs out of elements, attach
// the rest of another sequence to the end of result sequence.

bool isSubsequence(int initialPos, const Sequence& seq1, const Sequence& seq2);
// This is an ancillary function for subsequence. It checks whether the elements
// of seq2 is exactly the same as elements in seq1 starting from initialPos.

#endif
