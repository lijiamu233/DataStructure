#include <iostream>
#include <cstdio>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <cassert>

#define OK 1
#define ERROR 0
typedef bool Status;
template <class T>
class StackNode{
    public:
        StackNode<T> *Prev;
        StackNode<T> *Next;
        T data;
        StackNode(T key);
        ~StackNode();
};
template <class T>
StackNode<T>:: StackNode(T key)
{
    data = key;
    Prev = nullptr;
    Next = nullptr;
}

template <class T>
StackNode<T>:: ~StackNode()
{
}

template <class T>
class Stack{
    public:
        Stack();
        ~Stack();
        void ClearStack();
        bool StackEmpty();
        int StackLength() const;
        T GetTop() const;
        Status Push(T e);
        T Pop();
        Status StackTraverse(void (*visit)(T x));
    private:
        int stacksize;
        StackNode<T> *top;
        StackNode<T> *bottom;
};

template <class T>
Stack<T>:: Stack()
{
    top = nullptr;
    bottom = nullptr;
    stacksize = 0;
}

template <class T>
Stack<T>:: ~Stack()
{
}

template <class T>
void Stack<T>:: ClearStack()
{
    top = nullptr;
    stacksize = 0;
}

template <class T>
bool Stack<T>:: StackEmpty()
{
    if (stacksize == 0)
        return true;
    return false;
}

template <class T>
int Stack<T>:: StackLength() const
{
    return stacksize;
}

template <class T>
T Stack<T>:: GetTop() const
{
    if(top == nullptr) assert(ERROR);
    return top->data;
}

template <class T>
Status Stack<T>:: Push(T e)
{
    StackNode<T> *node;
    node = top;
    top = new StackNode<T>(e);
    if (top == nullptr)
        return ERROR;
    if(node != nullptr)
    {
        node -> Next = top;
        top -> Prev = node;
    }
    else
    {
        bottom = top;
    }
    stacksize ++;
    return OK;
}

template <class T>
T Stack<T>:: Pop()
{
    if(top == nullptr) assert(ERROR);
    T data = top -> data;
    top = top -> Prev;
    top -> Next = nullptr;
    delete top -> Next;
    stacksize --;
    if(stacksize == 0)
    {
        bottom = nullptr;
    }
    return data;
}
template <class T>
Status Stack<T>::StackTraverse(void (*visit)(T x))
{
    StackNode<T> *node;
    node = bottom;
    while(node != nullptr)
    {
        visit(node->data);
        node = node ->Next;
    }
    return OK;
}
 