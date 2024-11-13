#pragma once
#include<string>
#include<iostream>



using namespace std;

template <typename T>
struct DoubleListNode
{
    T data;
    DoubleListNode<T>* next;
    DoubleListNode<T>* prev;
    DoubleListNode<T>(T value) : data(value), next(nullptr), prev(nullptr) {};
    ~DoubleListNode<T>()
    {
        
    }
};

template <typename T>
struct DL
{
    DoubleListNode<T>* head;
    DoubleListNode<T>* tail;
    int len = 0;
    DL(int capacity=0);
    ~DL();

    void LDPUSHH(T elem);
    void LDPUSHT(T elem);
    void LDDELH();
    void LDDELT();
    void PRINT();
    void LDDELV(T elem);
    int LDISINDEX(T elem);
    T LDGET(int index);
    T& operator[](int index);
    DL(const DL& other);
    DL& operator=(const DL& other);
    bool search(T elem);
    void add(DL& other);
      
};

template <typename T>
void DL<T>::add(DL& other)
{
    for (int i=0;i<other.len;i++)
    {
        this->LDPUSHT(other[i]);
    }
}

template <typename T>
bool DL<T>::search(T elem)
{
    DoubleListNode<T>* current = head;
    while (current!=nullptr)
    {
        if (current->data == elem)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename T>
DL<T>& DL<T>::operator=(const DL& other)
{
    if (this != &other)
    {
        // Clear the current list
        while (head)
        {
            LDDELH();
        }

        // Copy the elements from the other list
        DoubleListNode<T>* current = other.head;
        while (current)
        {
            LDPUSHT(current->data);
            current = current->next;
        }
    }
    return *this; // Return the current object
}

template <typename T>
DL<T>::DL(const DL<T>& other) : head(nullptr), tail(nullptr), len(0) 
{
    DoubleListNode<T>* current = other.head;
    while (current != nullptr) 
    {
        LDPUSHT(current->data); // Используем метод добавления в конец
        current = current->next;
    }
}

template <typename T>
DL<T>::~DL()
{
    DoubleListNode<T>* current = head;
    while (current != nullptr)
    {
        DoubleListNode<T>* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

template <typename T>
void DL<T>::LDPUSHT(T elem)
{
    len++;
    DoubleListNode<T>* newNode = new DoubleListNode<T>(elem);
    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode; 
        newNode->prev = tail; 
        tail = newNode;
        newNode->next=nullptr;       
        
    }
}

template <typename T>
void DL<T>::LDPUSHH(T elem)
{
    len++;
    DoubleListNode<T>* newNode = new DoubleListNode<T>(elem);
    if (head == nullptr) 
    {
        head = newNode;
        tail = newNode;
        
    }
    else
    {
        newNode->next = head; // ������������� ��������� ���� ��� ������ ����
        head->prev = newNode; // ������������� ���������� ���� ��� ������� ������
        head = newNode;       // ��������� ������
        
    }
}

template <typename T>
DL<T>::DL(int capacity) : head(nullptr), tail(nullptr){}

template <typename T>
void DL<T>::PRINT()
{
    if (head == nullptr) return;
    DoubleListNode<T>* current = head;
    while (current!=tail)
    {
        cout << current->data << " ";
        current = current->next;
    }
    cout << current->data << " ";
    cout << "\n";
}

template <typename T>
void DL<T>::LDDELH()
{
    if (head == nullptr)
    {
        cout << "list empty\n";
        return;
    }
    len--;
    if (head == tail) 
    {
        delete head;
        head = nullptr;
        tail = nullptr;
        return;
    }
    DoubleListNode<T>* temporary = head;
    head = head->next;
    head->prev = nullptr;
    delete temporary;
}

template <typename T>
void DL<T>::LDDELT()
{
    if (head == nullptr)
    {
        cout << "list empty\n";
        return; 
    }
    len--;
    DoubleListNode<T>* temporary = tail; 

    if (head == tail)
    {
        delete temporary;
        head = nullptr; 
        tail = nullptr;
    }
    else 
    {
        tail = tail->prev; 
        tail->next = nullptr; 
        delete temporary; 
    };
}

template <typename T>
void DL<T>::LDDELV(T elem)
{
    if (head == nullptr)
    {
        cout << "list empty\n";
        return;
    }
 
    DoubleListNode<T>* current = head;
    int index = 0;
    while (current != nullptr)
    {
        if (current->data == elem)
        {
            if (current == head && current == tail)
            {
                delete current;
                head = nullptr;
                tail = nullptr;
            }
            else if (current == head)
            {
                head = head->next;
                head->prev = nullptr;
                delete current;
            }
            else if (current == tail)
            {
                tail = tail->prev;
                tail->next = nullptr;
                delete current;
            }
            else
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                delete current;
            }
            len--;
            break;
        }
        current = current->next;
        index++;
    }

    if (current == nullptr)
    {
        cout << "������ ���\n";
    }
}

template <typename T>
int DL<T>::LDISINDEX(T elem)
{
    DoubleListNode<T>* current = head;
    int counter = 0;
    while (current!=nullptr)
    {
        if (current->data == elem)
        {
            return counter;
        }
        counter++;
        current = current->next;
    }
    return -1;
}

template <typename T>
T DL<T>::LDGET(int index)
{
    if (index >= this->len)
    {
        return "null";
    }
    DoubleListNode<T>* current = head;
    int counter = 0;
    while (current != nullptr)
    {
        if (counter==index)
        {
            return current->data;
        }
        counter++;
        current = current->next;
    }
}

template <typename T>
T& DL<T>::operator[](int index)
{
    if (index >= this->len)
    {
        //cout << "incorrect index\n";
       
        return this->head->data;
    }
    DoubleListNode<T>* current = head;
    int count = 0;

    while (current != nullptr) 
    {
        if (count == index) 
        {
            return current->data; 
        }
        current = current->next;
        count++;
    }
}
