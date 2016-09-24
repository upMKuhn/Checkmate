#pragma once
#include "Stdafx.h"

namespace DataStructs {

	template<class NodeValue>
	class SinglyLL
	{
	public:
		SinglyLL(NodeValue val) : myvalue(val) {};
		~SinglyLL() {};

		SinglyLL* node;

		SinglyLL<NodeValue>* makeNext(NodeValue val)
		{
			SinglyLL* nnode = new SinglyLL(val);
			nnode->node = &*this;
			nnode->index = index + 1;
			return nnode;
		}

		SinglyLL<NodeValue>* clear()
		{
			SinglyLL* ml = node;
			SinglyLL* temp;
			while (index > 0 && ml->index)
			{
				temp = ml->node;
				delete ml;
				ml = temp;
			}
			index = 0;
			return this;
		}


		inline int getIndex()
		{
			return index;
		}
		template<class NodeValue>
		inline NodeValue value()
		{
			return this->myvalue;
		}

	private:
		int index = 0;
		NodeValue myvalue;
	};


}