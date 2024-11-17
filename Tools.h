#pragma once
#include"DL.h"

using namespace std;


DL<string> splitString(const string& str);
string input();
string PlusOne(string& num);
void* clear(void* ptr, int value, size_t num);
template<typename T, typename T1>
struct Pair
{
	T first;
	T1 second;
	friend istream& operator>>(istream& in, Pair<T,T1>& pair)
	{
		in >> pair.first >> pair.second;
		return in;
	}
	friend ostream& operator<<(ostream& os, Pair<T, T1>& pair)
	{
		cout << pair.first << " " << pair.second;
		return os;
	}
	friend bool operator==(Pair<T, T1>& pair1, Pair<T, T1>& pair2)
	{
		if (pair1.first != pair2.first)
		{
			return false;
		}
		if (pair1.second != pair2.second)
		{
			return false;
		}
		return true;
	}
};