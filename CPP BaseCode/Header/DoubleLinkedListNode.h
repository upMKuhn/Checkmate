#pragma once
#include "Stdafx.h"

namespace DataStructs { 
	

	template<class NodeValue>
	class DoubleLinkedListNode {
		private:
			NodeValue value;
			DoubleLinkedListNode* successor;
			DoubleLinkedListNode* predecessor;

		public:
			DoubleLinkedListNode(NodeValue val);
			DoubleLinkedListNode<NodeValue>::~DoubleLinkedListNode();

			DoubleLinkedListNode<NodeValue>* DoubleLinkedListNode<NodeValue>::getSuccessor();
			DoubleLinkedListNode<NodeValue>* DoubleLinkedListNode<NodeValue>::getPredecessor();


			bool DoubleLinkedListNode<NodeValue>::isFirst();
			bool DoubleLinkedListNode<NodeValue>::isLast();
		
			void DoubleLinkedListNode<NodeValue>::remove(NodeValue val);
			void DoubleLinkedListNode<NodeValue>::insertBefore(DoubleLinkedListNode<NodeValue>* newNode);
			void DoubleLinkedListNode<NodeValue>::insertAfter(DoubleLinkedListNode<NodeValue>* newNode);

			NodeValue DoubleLinkedListNode<NodeValue>::getValue()
			{
				return this->value;
			}
			

	};


			template<class NodeValue>
			DoubleLinkedListNode<NodeValue>::DoubleLinkedListNode(NodeValue val)
			{
				this->value = val;
				//if (successor != NULL) { successor->predecessor = NULL; }
				//if (predecessor != NULL) { predecessor->successor = NULL; }
				successor = NULL;
				predecessor = NULL;
			}

			template<class NodeValue>
			DoubleLinkedListNode<NodeValue>::~DoubleLinkedListNode()
			{
				successor = NULL;
				predecessor = NULL;
				delete this->value;
			}

			template<class NodeValue>
			DoubleLinkedListNode<NodeValue>* DoubleLinkedListNode<NodeValue>::getSuccessor()
			{
				return successor;
			}
			template<class NodeValue>
			DoubleLinkedListNode<NodeValue>* DoubleLinkedListNode<NodeValue>::getPredecessor()
			{
				return predecessor;
			}


			template<class NodeValue>
			bool DoubleLinkedListNode<NodeValue>::isFirst()
			{
				return predecessor == NULL;

			}
			template<class NodeValue>
			bool DoubleLinkedListNode<NodeValue>::isLast()
			{
				return successor == NULL;
			}


			template<class NodeValue>
			void DoubleLinkedListNode<NodeValue>::insertAfter(DoubleLinkedListNode<NodeValue>* newNode)
			{
				
				if (this->isFirst())
				{
					successor = newNode;
					newNode->predecessor = this;
				}
				else {
					predecessor-> successor = newNode;
					newNode->predecessor = this;
					newNode->successor = successor;
					successor = newNode;
				}
			}

			template<class NodeValue>//insert before this
			void DoubleLinkedListNode<NodeValue>::insertBefore(DoubleLinkedListNode<NodeValue>* newNode)
			{
				if (this->isFirst())
				{
					predecessor = newNode;
					newNode->successor = this;
				}
				else {
					predecessor->successor = newNode;
					newNode->predecessor = predecessor;
					newNode->successor = this;
					predecessor = newNode;
				}
			}


			template<class NodeValue>
			void DoubleLinkedListNode<NodeValue>::remove(NodeValue val)
			{

				if (!this->isLast())
				{
					successor->predecessor = predecessor;
				}

				if (!this->isFirst())
				{
					predecessor->successor = successor;
				}
			}

}