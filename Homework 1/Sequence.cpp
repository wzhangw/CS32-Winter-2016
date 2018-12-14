#include "Sequence.h"

Sequence::Sequence()
{
	m_size = 0; // initialize an empty string
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
	if (pos < 0 || pos > size() || size() == DEFAULT_MAX_ITEMS) // return false if pos not in range, or the array reaches maximal items
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
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
	{
		ItemType temp;
		temp = other.m_items[i];
		other.m_items[i] = m_items[i];
		m_items[i] = temp;
	}
	int temp_size = other.size();
	other.m_size = m_size;
	m_size = temp_size;
}