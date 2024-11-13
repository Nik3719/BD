#pragma once
#include<iostream>
#include<string>
#include"Tools.h"
#include"DL.h"

using namespace std;


template<typename T>
struct DictNode
{
	string key;
	T value;
	bool isOccupied;
	int NoneDeletePresence;
	DictNode<T>() : value(T()), key(""), isOccupied(false), NoneDeletePresence(1) {};
};

template<typename T>
struct dict
{
	DictNode<T>* table;
	int size = 0;
	int len = 0;
	dict(int=1);
	~dict();
	void HSET(string key, T value);
	int hashFunc(string key);
	T HGET(string key);
	T& operator[](string key);
	T HDEL(string key);
	void resize(string key, T value);
	void PRINT();
	bool ISPRESENCE(string key);
	void copy(dict<T> &other);
	DL<string> getKeys();
	dict(const dict<T>& other);
	dict<T>& operator=(const dict<T>& other);

};

template<typename T>
dict<T>& dict<T>::operator=(const dict<T>& other) 
{
        // Check for self-assignment
        if (this == &other) 
		{
            return *this;
        }
        delete[] table;

        // Copy size
        size = other.size;

        // Allocate new memory for the table
        table = new DictNode<T>[size];

        // Copy each DictNode from the other dict
        for (int i = 0; i < size; ++i) {
            table[i] = other.table[i];
        }

        return *this;
    }

template<typename T>
dict<T>::dict(const dict<T>& other) : size(other.size) 
{
    // Выделяем память для нового массива узлов
    table = new DictNode<T>[other.size];

    // Копируем данные из другого хеш-таблицы
    for (int i = 0; i < other.size; ++i) 
	{
        table[i] = other.table[i]; // Копируем каждый узел
    }
}

template<typename T>
dict<T>::dict(int size)
{
	if (size == 0)return;
	this->size = size;
	this->table = new DictNode<T>[size];
}

template<typename T>
dict<T>::~dict()
{
	delete[]table;
}

template<typename T>
void dict<T>::resize(string key, T value)
{
	int oldSize = size;
	size++;
	//if (size==0) 
	size *= 2;
	DictNode<T>* newTable = new DictNode<T>[size];
	for (int j = 0; j < oldSize; j++)
	{
		if (table[j].isOccupied)
		{
			int index = hashFunc(table[j].key);
			while (newTable[index].isOccupied)
			{
				index = (index + 1) % size;
			}
			newTable[index].key = table[j].key;
			newTable[index].value = table[j].value;
			newTable[index].isOccupied = true;
			newTable[index].NoneDeletePresence = 3;
		}
	}

	table = newTable;
	HSET(key, value);
	//delete []table;
}

template<typename T>
void dict<T>::HSET(string key, T value)
{
	int i = 0;
	int index = hashFunc(key);
	int oldIndex = index;
	if (size == 0)
	{
		resize(key, value);
		return;
	}
	while (table[index].isOccupied)
	{
		if (table[index].key == key)
		{
			table[index].value = value;

			return;
		}
		index = (index + 1) % size;
		if (oldIndex == index)
		{
			resize(key, value);
			return;
		}
	}
	table[index].key = key;
	table[index].value = value;
	table[index].isOccupied = true;
	table[index].NoneDeletePresence = 3;
	len++;
}


template<typename T>
int dict<T>::hashFunc(string key)
{
    int k = 0;
    for (unsigned char b : key)
    {
        k += b;
    }
	return (k % size);//+ i * (1 + (k % (size - 1)))) % size;
	//  такого нет\ n-������� + i* такого нет\ n �������
}

template<typename T>
T dict<T>::HGET(string key)
{
	int i = 0;
	int index = hashFunc(key);
	int oldIndex = index;
	while (table[index].isOccupied)
	{
		if (table[index].NoneDeletePresence == 1)
		{
			return " такого нет\n";
		}
		if (table[index].key == key)
		{	
			return table[index].value;
		}
		
		index = (index + 1) % size;
		if (oldIndex == index)
		{
			break;
		}
	}
	return " такого нет\n";
}

template<typename T>
bool dict<T>::ISPRESENCE(string key)
{
	int i = 0;
	int index = hashFunc(key);
	int oldIndex = index;
	while (table[index].isOccupied)
	{
		if (table[index].NoneDeletePresence == 1)
		{
			return false;
		}
		if (table[index].key == key)
		{	
			return true;
		}
		
		index = (index + 1) % size;
		if (oldIndex == index)
		{
			break;
		}
	}
	return false;
}

template<typename T>
T& dict<T>::operator[](string key)
{
	int i = 0;
	int index = hashFunc(key);
	int oldIndex = index;
	while (table[index].isOccupied)
	{
		if (table[index].key == key)
		{	
			return table[index].value;
		}
		
		index = (index + 1) % size;
		if (oldIndex == index)
		{
			cout << "Error такого ключа нет\n";
			break;
		}
	}
}

template<typename T>
T dict<T>::HDEL(string key)
{
	int i = 0;
	int index = hashFunc(key);
	int oldIndex = index;
	while (table[index].isOccupied)
	{
		if (table[index].key == key)
		{
			T res = table[index].value;
			table[index].isOccupied = false;
			table[index].NoneDeletePresence = 2;
			len--;
			return res;
		}
		index = (index + 1) % size;
		if (oldIndex == index)
		{
			return " такого нет\n";
		}
	}
	return " такого нет\n";
}

template<typename T>
void dict<T>::PRINT()
{
	int index = 0;
	while (index<size)
	{
		if (table[index].isOccupied)
		{
			cout << table[index].key << ":" << table[index].value << "\n";
		}
		index++;
	}

}

template<typename T>
void dict<T>::copy(dict<T> &other)
{
	int index = 0;
	while (index < size)
	{
		if (table[index].isOccupied)
		{
			other.HSET(table[index].key,table[index].value);
		}
		index++;
	}
}


template<typename T>
DL<string> dict<T>:: getKeys()
{
	DL<string> res;
	int index = 0;
	while (index < size)
	{
		if (table[index].isOccupied)
		{
			res.LDPUSHT(table[index].key);
		}
		index++;
	}
	return res;

}



