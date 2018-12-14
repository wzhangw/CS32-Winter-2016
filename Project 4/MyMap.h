#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

#include "support.h"

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap(); // create an empty map
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node
	{
		KeyType m_key;
		ValueType m_value;
		Node* m_left;
		Node* m_right;
	};
	Node* m_root;
	int m_size;
	// auxiliary private functions
	const ValueType* findNodeOfTheKey(Node* curr, const KeyType& key) const;
	void insertNode(Node* curr, Node* toBeInserted);
	void clearNodes(Node*& curr);
};

template<typename KeyType, typename ValueType>
inline MyMap<KeyType, ValueType>::MyMap()
	: m_root(nullptr), m_size(0)
{}

template<typename KeyType, typename ValueType>
inline MyMap<KeyType, ValueType>::~MyMap()
{
	while (m_root != nullptr)
		clear();
	// g++ detects memory leak of 56 bytes for 1 allocation. Figure out why later
}

template<typename KeyType, typename ValueType>
inline void MyMap<KeyType, ValueType>::clear()
{
	// clear nodes only if map is not empty
	if (m_root != nullptr)
		clearNodes(m_root);
}

template<typename KeyType, typename ValueType>
inline int MyMap<KeyType, ValueType>::size() const
{
	return m_size;
}

template<typename KeyType, typename ValueType>
inline void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	// create new node/association
	Node* newNode = new Node;
	newNode->m_key = key;
	newNode->m_value = value;
	newNode->m_left = nullptr;
	newNode->m_right = nullptr;

	// if map is empty, root points to newNode. Otherwise, insert the node into correct position.
	if (m_root == nullptr)
	{
		m_root = newNode;
		m_size++;
	}
	else
		insertNode(m_root, newNode);
}

template<typename KeyType, typename ValueType>
inline const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const
{
	return findNodeOfTheKey(m_root, key);
}

template<typename KeyType, typename ValueType>
inline const ValueType* MyMap<KeyType, ValueType>::findNodeOfTheKey(Node* curr, const KeyType& key) const
{
	// return NULL if curr is not a node
	if (curr == nullptr)
		return nullptr;

	// return pointer to current value if current node has the desired key
	const ValueType* resultPtr = &(curr->m_value);
	if (curr->m_key == key)
		return resultPtr;

	// search for the left side, return the result if it is not NULL
	const ValueType* resultLeft = findNodeOfTheKey(curr->m_left, key);
	if (resultLeft != nullptr)
		return resultLeft;

	// search for the right side, return the result if it is not NULL
	const ValueType* resultRight = findNodeOfTheKey(curr->m_right, key);
	if (resultRight != nullptr)
		return resultRight;

	// return NULL if binary search does not find desired key
	return nullptr;
}

template<typename KeyType, typename ValueType>
inline void MyMap<KeyType, ValueType>::insertNode(Node* curr, Node* toBeInserted)
{
	// overload current node value if curr and inserted node share the same key value
	if (curr->m_key == toBeInserted->m_key)
	{
		curr->m_value = toBeInserted->m_value;
		delete toBeInserted;
	}
	else if (curr->m_key < toBeInserted->m_key)
		// go right if inserted node has a greater key value
	{
		if (curr->m_right == nullptr)
		{
			curr->m_right = toBeInserted; // insert new node
			m_size++;
		}
		else
			insertNode(curr->m_right, toBeInserted); // continue to find good place to insert
	}
	else
		// go left if inserted node has a smaller key value
	{
		if (curr->m_left == nullptr)
		{
			curr->m_left = toBeInserted; // insert new node
			m_size++;
		}
		else
			insertNode(curr->m_left, toBeInserted); // continue to find good place to insert
	}
}

template<typename KeyType, typename ValueType>
inline void MyMap<KeyType, ValueType>::clearNodes(Node*& curr)
{
	// assume curr is not nullptr. This is ensured at the time the function is called

	// delte current node if it is a leaf node
	if (curr->m_left == nullptr && curr->m_right == nullptr)
	{
		delete curr;
		m_size--;
		curr = nullptr;
	}
	else
		// follow non-empty child(ren) to clear empty nodes
	{
		if (curr->m_left != nullptr)
			clearNodes(curr->m_left);
		if (curr->m_right != nullptr)
			clearNodes(curr->m_right);
	}
}

#endif // MYMAP_INCLUDED