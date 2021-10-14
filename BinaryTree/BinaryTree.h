#include <iostream>
#include <cstdio>
#include <string>
#include "../Stack/Stack.h"
#include <stack>
using namespace std;

template <class T>
class BiNode{
    public:
        T data;
        BiNode<T>* lchild;
        BiNode<T>* rchild;
        BiNode<T>* father;
        BiNode(T d)
        {
            data = d;
            father = nullptr;
            lchild = nullptr;
            rchild = nullptr;
        }
        ~BiNode();
};

template <class T>
class BinaryTree{
    public:
        BinaryTree(int n);
        ~BinaryTree();
        void InOrderTraverse();
        void Insert(int fa, int ins, int LR, T d);
        void SetToot(int r, T d);
    private:
        BiNode<T>* root;
        BiNode<T>** pos;
        int size;
};

template <class T>
BinaryTree<T>::BinaryTree(int n)
{
    root = nullptr;
    pos = new BiNode<T>*[n+1];
    size = 0;
}

template <class T>
BinaryTree<T>::~BinaryTree()
{
}

template <class T>
void BinaryTree<T>::Insert(int fa, int ins, int LR, T d)
{
    BiNode<T>* p = pos[fa];
    if(p == nullptr)
    {
        throw "insert error";
    }
    size ++;
    BiNode<T>* NewNode = new BiNode<T>(d);
    NewNode->father = p;
    if(LR == 0)
    {
        NewNode->lchild = p->lchild;
        p->lchild = NewNode;
    }
    else if(LR == 1)
    {
        NewNode->rchild = p->rchild;
        p->rchild = NewNode;
    }
    pos[ins] = NewNode;
}
template <class T>
void BinaryTree<T>::SetToot(int r, T d)
{
    BiNode<T>* Node = new BiNode<T>(d);
    root = Node;
    pos[r] = Node;
}
template <class T>
void BinaryTree<T>::InOrderTraverse()
{
    Stack< BiNode<T>* > s;
    BiNode<T>* p = root;
    int KKK=0;
    while(p != nullptr || !s.StackEmpty())
    {
        KKK++;
        while(p != nullptr)
        {
            s.Push(p);
            p = p -> lchild;
        }
        if(!s.StackEmpty())
        {
            p = s.GetTop();
            cout << p->data << ' ';
            s.Pop();
            p = p -> rchild;
        }
    }
    cout << endl;
}