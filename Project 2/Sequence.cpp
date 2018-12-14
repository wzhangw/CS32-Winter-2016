#include "Sequence.h"
#include <iostream>

Sequence::Sequence()
	:m_size(0)
{
	m_head = nullptr;
}

Sequence::~Sequence()
{
	if (m_head != nullptr) // if sequence is not empty
	{
		Node* ptr;
		for (ptr = m_head; ptr->m_next != nullptr; ptr = ptr->m_next)
			delete ptr->m_prev; // delete the previous node until ptr reaches the last node
		delete ptr->m_prev; // delete the node ahead of the last node (safe if prev is nullptr)
		delete ptr; // delete the last node
	}
}

Sequence::Sequence(const Sequence& other)
{
	m_size = other.m_size;
	if (m_size == 0)
		m_head = nullptr; // construct an empty sequence if other is empty
	else
	{
		m_head = new Node;
		*m_head = *other.m_head; // assign the first node of other to this sequence
		m_head->m_prev = nullptr; // assign null to previous pointer of head node
		Node* ptr = m_head;
		Node* ptr_other = other.m_head;
		for (int i = 1; i < m_size; i++)
		{
			ptr->m_next = new Node; // create a new node
			*ptr->m_next = *ptr_other->m_next; // copy the next node
			ptr->m_next->m_prev = ptr;
			ptr = ptr->m_next; // ptr points to the new node
			ptr_other = ptr_other->m_next; // ptr points to the next node in other
		}
	}
}

Sequence& Sequence::operator=(const Sequence& other)
{
	if (this != &other)
	{
		Sequence temp = other;
		swap(temp);
	}
	return *this;
}

bool Sequence::empty() const
{
	return m_size == 0;
}

int Sequence::size() const
{
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
	// check whether pos is valid
	if (pos < 0 || pos > size())
		return false;

	// create a new node
	Node* newNode = new Node;
	newNode->m_item = value;
	if (m_head == nullptr)
	{
		// if sequence is empty, insert the first node into an empty list...
		newNode->m_next = nullptr;
		newNode->m_prev = nullptr;
	}
	else
	{
		Node* temp = m_head;
		if (pos == size())
		{
			// insert a new node to the end of a list
			for (int i = 0; i < size() - 1; i++)
				temp = temp->m_next;
			newNode->m_prev = temp;
			temp->m_next = newNode;
			newNode->m_next = nullptr;
		}
		else 
		{
			// insert a new node to anywhere else of a list
			for (int i = 0; i < pos; i++)
				temp = temp->m_next;
			newNode->m_prev = temp->m_prev;
			newNode->m_next = temp;
			if (temp->m_prev != nullptr)
				temp->m_prev->m_next = newNode;
			temp->m_prev = newNode;
		}
	}

	// re-assign head pointer if new node is inserted at pos 0
	if (pos == 0)
		m_head = newNode;
	
	// increment size
	m_size++;

	return true;
}

int Sequence::insert(const ItemType& value)
{
	Node* ptr = m_head;
	int p = 0;

	// find the first position p where value is smaller than or equal to the item
	for (; ptr != nullptr && value > ptr->m_item; p++)
		ptr = ptr->m_next;

	// insert value to position p
	insert(p, value);
	return p;
}

bool Sequence::erase(int pos)
{
	// return false if pos out of range, or size is 0
	if (pos < 0 || pos >= size()) 
		return false;
	
	// find the node at pos
	Node* temp = m_head;
	for (int i = 0; i < pos; i++)
		temp = temp->m_next;

	// re-assign pointers
	if (pos != 0)
		temp->m_prev->m_next = temp->m_next;
	if (pos != size() - 1)
		temp->m_next->m_prev = temp->m_prev;

	// re-assign head pointer if necessary
	if (pos == 0)
		m_head = temp->m_next;

	delete temp;
	m_size--;
	return true;
}

int Sequence::remove(const ItemType& value)
{
	// return 0 for empty sequence
	if (size() == 0)
		return 0;

	// find all items equal to value, and erase every one of them
	int p = 0; // counts how many items are removed
	int pos = 0;
	for (Node* ptr = m_head; ptr != nullptr; ptr = ptr->m_next, pos++)
		if (ptr->m_item == value)
		{
			ptr = ptr->m_prev;
			erase(pos);
			p++;
			pos--; // restore pos back to make sure every node is checked
		}
	return p;
}

bool Sequence::get(int pos, ItemType& value) const
{
	// return false if pos is out of range
	if (pos < 0 || pos >= size())
		return false;

	// find the item at pos, and set value as the item
	Node* ptr = m_head;
	for (int i = 0; i < pos; i++)
		ptr = ptr->m_next;
	value = ptr->m_item;
	return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
	// return false if pos is out of range
	if (pos < 0 || pos >= size())
		return false;

	// find the item at pos, and set the item as value
	Node* ptr = m_head;
	for (int i = 0; i < pos; i++)
		ptr = ptr->m_next;
	ptr->m_item = value;
	return true;
}

int Sequence::find(const ItemType& value) const
{
	// return -1 if sequence is empty
	if (m_head == nullptr)
		return -1;

	// loop through every node to find all items that are equal to value
	Node* temp = m_head;
	int p;
	for (p = 0; temp->m_next != nullptr; p++, temp = temp->m_next) // loop until reaches the last node
		if (temp->m_item == value)
			return p;
	if (temp->m_item == value) // check the last node
		return p;

	// if compiler reaches here, it means no item equal to value is found, so return -1
	return -1;
}

void Sequence::swap(Sequence& other)
{
	// swap the pointer
	Node* temp_ptr = other.m_head;
	other.m_head = m_head;
	m_head = temp_ptr;

	// swap the size
	int temp_size = other.m_size;
	other.m_size = m_size;
	m_size = temp_size;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	// return -1 for empty seq2 or if size of seq1 is smaller
	if (seq2.empty() || seq1.size() < seq2.size()) 
		return -1;

	// get the first item of seq2
	ItemType value; 
	ItemType firstItemOfSeq2;
	seq2.get(0, firstItemOfSeq2);

	// find the items of seq1 that are equal to the first item of seq2,
	// and check whether seq1 has a subsequence starting from that position
	// that is the same as seq2
	for (int i = 0; i < seq1.size() - seq2.size() + 1; i++)
	{
		seq1.get(i, value);
		if (value == firstItemOfSeq2 && isSubsequence(i, seq1, seq2))
			return i;
	}
	
	// if compiler reaches here, it means no subsequence is found, so return -1
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	Sequence empty;
	result = empty;
	int smallerSize;

	// Determine which one is the smaller sequence (equal size doesn't matter...)
	Sequence largerSeq;
	if (seq1.size() < seq2.size())
	{
		smallerSize = seq1.size();
		largerSeq = seq2;
	}
	else
	{
		smallerSize = seq2.size();
		largerSeq = seq1;
	}

	// Interweave until one sequence runs out
	ItemType inserted1;
	ItemType inserted2;
	for (int i = 0; i < smallerSize; i++)
	{
		seq1.get(i, inserted1);
		seq2.get(i, inserted2);
		result.insert(2 * i, inserted1);
		result.insert(2 * i + 1, inserted2);
	}

	// Attach the rest of another sequence, if any
	for (int i = smallerSize; i < largerSeq.size(); i++)
	{
		largerSeq.get(i, inserted1);
		result.insert(smallerSize + i, inserted1);
	}
}

// ancillary function for subsequence
bool isSubsequence(int initialPos, const Sequence& seq1, const Sequence& seq2)
{
	ItemType value1;
	ItemType value2;

	// Starting from initialPos of seq1, assume size of seq2 is n,
	// check whether the next n consecutive items of seq1 are the
	// same as all items of seq2. If yes, return true. Otherwise, 
	// return false.
	for (int i = 0; i < seq2.size(); i++)
	{
		seq1.get(i + initialPos, value1);
		seq2.get(i, value2);
		if (value1 != value2)
			return false;
	}
	return true;
}

// For debugging and testing purposes only
// dump prints out all items of sequence in order
void Sequence::dump() const
{
	for (Node* ptr = m_head; ptr != nullptr; ptr = ptr->m_next)
		std::cerr << ptr->m_item << std::endl;
}