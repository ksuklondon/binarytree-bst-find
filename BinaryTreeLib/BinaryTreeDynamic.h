#pragma once

template<typename T>
class BinaryTreeDynamic
{
private:
	class Node
	{
	private:
		T value;
		Node* left, * right;
		Node* parent;
	public:
		Node(T const& value, Node* left, Node* right) :
			value(value), left(left), right(right)
		{}
	};

	Node* root;
};