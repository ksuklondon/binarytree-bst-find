#pragma once
#include <cassert>
#include <memory>
#include <string>
#include <ostream>
#include <iomanip>

template<typename KeyType, typename ValueType>
class BinarySearchTree
{
private:
	class Node
	{
	public:
		KeyType key;
		ValueType value;
		std::unique_ptr<Node> left, right;
	public:
		Node(KeyType const& key, ValueType const& value) : key(key), value(value)
		{}
	};

	std::unique_ptr<Node> root;
	size_t _size = 0;

	std::unique_ptr<Node>& findOrClosest(std::unique_ptr<Node>& startNode, KeyType const& key)
	{
		auto node = &startNode;
		auto lastVisited = &startNode;
		while ((*node) != nullptr && (*node)->key != key)
		{
			if (key < (*node)->key)
				node = &((*node)->left);
			else
				node = &((*node)->right);
			if ((*node) != nullptr)
				lastVisited = node;
		}
		return *lastVisited;
	}

	void toString(Node const* node, std::string& s) const
	{
		if (node == nullptr)
			return;
		s += "([" + std::to_string(node->key) + "," + std::to_string(node->value) + "],";
		toString(node->left.get(), s);
		s += ",";
		toString(node->right.get(), s);
		s += ")";
	}

	template<typename StreamType>
	void print(StreamType& stream, Node* node, int indent, std::string prefix) const
	{
		if (node != nullptr)
		{
			if (indent > 0)
				stream << std::setw(indent) << " ";
			stream << prefix;
			stream << "[" << node->key << ", " << node->value << "]" << std::endl;
			print(stream, node->left.get(), indent + 4, "L: ");
			print(stream, node->right.get(), indent + 4, "R: ");
		}
	}

public:

	size_t size() const
	{
		return _size;
	}

	void insert(KeyType const& key, ValueType const& value)
	{
		if (root == nullptr)
		{
			root = std::make_unique<Node>(key, value);
			++_size;
			return;
		}
		auto& node = findOrClosest(root, key);
		if (node->key == key)
			node->value = value;
		else
		{
			if (key < node->key)
				node->left = std::make_unique<Node>(key, value);
			else
				node->right = std::make_unique<Node>(key, value);
			++_size;
		}
	}

	void remove(KeyType const& key)
	{
		if (root == nullptr)
			return;
		auto& node = findOrClosest(root, key);
		if (node->key == key)
		{
			if (node->left.get() != nullptr && node->right.get() != nullptr)
			{
				auto& largestOnTheLeft = findOrClosest(node->left, key);
				auto tmp = std::move(largestOnTheLeft);
				largestOnTheLeft = std::move(tmp->left);

				tmp->left = std::move(node->left);
				tmp->right = std::move(node->right);

				node = std::move(tmp);
			}
			else
				if (node->left.get() != nullptr)
					node = std::move(node->left);
				else
					node = std::move(node->right);
			--_size;
		}
	}

	ValueType* find(KeyType const& key)
	{
		//TODO Implement find methods that looks for a key with the given value
		auto node = &root;
		while (*node != nullptr)
		{
			if (key == (*node)->key)
			{
				return &((*node)->value);
			}
			else if (key < (*node)->key)
			{
				node = &((*node)->left);
			}
			else
			{
				node = &((*node)->right);
			}
		}
		return nullptr; // Key not found
	}

	std::string toString() const
	{
		std::string s;
		toString(root.get(), s);
		return s;
	}

	template<typename StreamType>
	void print(StreamType& stream) const
	{
		print(stream, root.get(), 0, "");
	}
};

template<typename KeyType, typename ValueType>
std::ostream& operator <<(std::ostream& stream, BinarySearchTree<KeyType, ValueType> const& tree)
{
	tree. template print<std::ostream>(stream);
	return stream;
}
