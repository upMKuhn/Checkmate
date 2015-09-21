#pragma once
#include "DoubleLinkedListNode.h"
#include "Stdafx.h"



namespace DataStructs {

	template<class NodeValue>
	class DoubleLinkedList{

		private:
			DoubleLinkedListNode<NodeValue>* head;
			DoubleLinkedListNode<NodeValue>* tail;
			int m_length;
		public:
			friend class DoubleLinkedListNode<NodeValue>;
			DoubleLinkedList<NodeValue>::DoubleLinkedList()
			{
				m_length = -1;
				head = NULL;
				tail = NULL;
			}
			DoubleLinkedList<NodeValue>::~DoubleLinkedList()
			{
				Clear();
			}

			int DoubleLinkedList<NodeValue>::Length() { return m_length; }

			void DoubleLinkedList<NodeValue>::appendHead(NodeValue val);
			void DoubleLinkedList<NodeValue>::appendTail(NodeValue val);

			DoubleLinkedListNode<NodeValue>* DoubleLinkedList<NodeValue>::getNode(int position);
			void DoubleLinkedList<NodeValue>::insertAt(int position, NodeValue val);
			void Clear();

		};

		template<class NodeValue>
		void DoubleLinkedList<NodeValue>::appendHead(NodeValue val)
		{
			DoubleLinkedListNode<NodeValue>* temp = new DoubleLinkedListNode<NodeValue>(val);

			if (head != NULL)
			{
				temp->insertAfter(head);
			}
			head = temp;
			this->m_length++;
		}

		template<class NodeValue>
		void DoubleLinkedList<NodeValue>::appendTail(NodeValue val)
		{
			DoubleLinkedListNode<NodeValue>* temp = new DoubleLinkedListNode<NodeValue>(val);
			if (tail == NULL)
			{
				head->insertAfter(temp);
			}
			else {

				temp->insertBefore(tail);
			}
			tail = temp;
			this->m_length++;
		}

		template<class NodeValue>
		DoubleLinkedListNode<NodeValue>* DoubleLinkedList<NodeValue>::getNode(int position)
		{
			DoubleLinkedListNode<NodeValue>* node = head;
			while (position > 0)
			{
				assert(!node->isLast());
				node = node->getSuccessor();
				position--;
			}
			return node;
		}
	
		template<class NodeValue>
		void DoubleLinkedList<NodeValue>::insertAt(int position, NodeValue val)
		{
		
			DoubleLinkedListNode<NodeValue>* nodeAt = getNode(position);
			DoubleLinkedListNode<NodeValue>* newNode = new DoubleLinkedListNode<NodeValue>(val);
		
			nodeAt->insertBefore(newNode);
			if (nodeAt == head)
			{
				head = newNode;
			}


			m_length++;
		}

		template<class NodeValue>
		void DoubleLinkedList<NodeValue>::Clear()
		{
			if (head != NULL) {
				DoubleLinkedListNode<NodeValue>* old;
				DoubleLinkedListNode<NodeValue>* temp = head;
				while (!temp->isLast())
				{
					old = temp;
					temp = temp->getSuccessor();
					DELETE(old)
					this->m_length--;
				}

				head = NULL;
				tail = NULL;
			}
		}


}