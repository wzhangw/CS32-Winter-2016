#include "ScoreList.h"

ScoreList::ScoreList()
{
}

bool ScoreList::add(unsigned long score)
{
	if (score >= 0 && score <= 100)
	{
		int position = m_sequence.insert(score);
		return (position != -1);
	}
	return false;
}

bool ScoreList::remove(unsigned long score)
{
	if (m_sequence.find(score) == -1)
		return false;
	bool success = m_sequence.erase(m_sequence.find(score));
	return success;
}

int ScoreList::size() const
{
	return m_sequence.size();
}

unsigned long ScoreList::minimum() const
{
	if (size() == 0)
		return NO_SCORE;
	unsigned long min;
	unsigned long temp;
	m_sequence.get(0, min);
	for (int i = 1; i < m_sequence.size(); i++)
	{
		m_sequence.get(i, temp);
		if (min > temp)
			min = temp;
	}
	return min;
}

unsigned long ScoreList::maximum() const
{
	if (size() == 0)
		return NO_SCORE;
	unsigned long max;
	unsigned long temp;
	m_sequence.get(0, max);
	for (int i = 1; i < m_sequence.size(); i++)
	{
		m_sequence.get(i, temp);
		if (max < temp)
			max = temp;
	}
	return max;
}