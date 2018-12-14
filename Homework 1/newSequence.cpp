#include "newSequence.h"
#include <iostream>

Sequence::Sequence(int size)
{
	if (size < 0)
	{
		std::cout << "Size must be non-negative." << std::endl;
		exit(1);
	}
	m_size = 0;
	max_size = size;
	m_items = new ItemType[max_size];
}

Sequence::Sequence(const Sequence& copy)
{
	m_size = copy.size();
	max_size = copy.max_size;
	m_items = new ItemType[max_size];
	for (int i = 0; i < m_size; i++)
		m_items[i] = copy.m_items[i];
}

Sequence::~Sequence()
{
	delete [] m_items;
}

Sequence& Sequence::operator=(const Sequence& value)
{
	if (this != &value)
	{
		Sequence temp(value);
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
// what about fixed capacity...
{
	if (pos < 0 || pos > size() || size() == max_size) // return false if pos not in range, or the array reaches maximal items
		return false;
	for (int i = size(); i > pos; i--)
		m_items[i] = m_items[i - 1]; // Starting from the item after the final one, copy the last item until it reaches the one after pos
	m_items[pos] = value;
	m_size++;
	return true;
}

int Sequence::insert(const ItemType& value)
{
	int p = 0;
	for (p; p < size(); p++)
		if (value <= m_items[p])
			break;
	if (!insert(p, value))
		p = -1;
	return p;
}

bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= size()) // return false if pos not in range
		return false;
	for (int i = pos; i < size() - 1; i++)
		m_items[i] = m_items[i + 1];
	m_size--;
	return true;
}

int Sequence::remove(const ItemType& value)
{
	int count = 0;
	for (int i = 0; i < size(); i++)
	{
		if (m_items[i] == value)
		{
			erase(i);
			count++;
			i--;
		}
	}
	return count;
}

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= size()) // return false if pos not in range
		return false;
	value = m_items[pos];
	return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= size()) // return false if pos not in range
		return false;
	m_items[pos] = value;
	return true;
}

int Sequence::find(const ItemType& value) const
{
	int p;
	for (p = 0; p < size(); p++)
		if (m_items[p] == value)
			return p;
	return -1; // Notice if size() == 0, this function returns -1.
}

void Sequence::swap(Sequence& other)
{
	ItemType *temp_ptr = other.m_items;
	other.m_items = m_items;
	m_items = temp_ptr;

	int temp_size = other.size();
	other.m_size = m_size;
	m_size = temp_size;

	int temp_max = other.max_size;
	other.max_size = max_size;
	max_size = temp_max;
}