// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

struct CDynamicArrayStruct
{
	unsigned int pointer;
	CDynamicArrayStruct* next;
};
template <class classType> class CDynamicArray
{
private:
	int elementsCount;
	CDynamicArrayStruct* pool;
	int size;
public:
	CDynamicArray()
	{
		this->pool = 0;
		this->elementsCount = 0;
		this->size = 0;
	}
	int	 GetSize()
	{
		return this->size;
	}
	void PushAtEnd(classType something)
	{
		CDynamicArrayStruct* pointer = pool;
		if (pointer != 0)
		{
			while (pointer != 0)
			{
				pointer = pointer->next;
			}
		}
		
		CDynamicArrayStruct* newOne = new CDynamicArrayStruct();
		newOne->next = 0;
		newOne->pointer = new classType();
		newOne->pointer = something;
		this->size++;
		if (pointer == 0)
			pool = newOne;
		else
			pointer->next = newOne;
	}
	void PushAtStart(classType something)
	{
		CDynamicArrayStruct* newOne = new CDynamicArrayStruct();
		if (pool != NULL)
			newOne->next = pool;
		else
			newOne->next = 0;
		newOne->pointer = new classType();
		newOne->pointer = something;
		this->size++;
		pool = newOne;
	}

	CDynamicArray::operator[](int ID)
	{
		int counter = 0;
		CDynamicArrayStruct* pointer = pool;
		if (pointer != 0)
		{
			if (counter == ID)
				return pointer->pointer;
			while (pointer != 0)
			{
				pointer = pointer->next;
				counter++;
			}
		}
		return 0;
	}

};