#ifndef _FIBONACCI_TREE_HEAP_
#define _FIBONACCI_TREE_HEAP_

#include <iomanip>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <assert.h>
using namespace std;

template <class T>
class FibNode {
    public:
    T key;
    int degree;
    FibNode<T> *left;
    FibNode<T> *right;
    FibNode<T> *child;
    FibNode<T> *parent;

    FibNode(T value):key(value),degree(0),
            left(NULL),right(NULL),child(NULL),parent(NULL) {
            key    = value;
            degree = 0;
            left   = this;
            right  = this;
            child  = NULL;
            parent = NULL;
        }
};

template <class T>
class FibHeap {
    private:
    int Size;
    int MaxDegree;
    FibNode<T> *min;
    FibNode<T> **cons;
    
    public:
        FibHeap();
        ~FibHeap();
        void push(T key);
        void pop();
        void destroy();
        bool empty() const;
        int size() const;
        T top() const;

    private:
        void removeNode(FibNode<T> *node);
        void addNode(FibNode<T> *node,FibNode<T> *root);
        void insert(FibNode<T> *node);
        FibNode<T>* extractMin();
        void link(FibNode<T> *node,FibNode<T> *root);
        void makeCons();
        void consolidate();
        void destroyNode(FibNode<T>* node);
};

template<class T>
FibHeap<T>::FibHeap()
{
    Size = 0;
    MaxDegree = 0;
    min = NULL;
    cons = NULL;
}

template <class T>
FibHeap<T>::~FibHeap()
{
}

template <class T>
void FibHeap<T>::removeNode(FibNode<T> *node)
{
    node->left->right = node->right;
    node->right->left = node->left;
}

template <class T>
void FibHeap<T>::addNode(FibNode<T> *node,FibNode<T> *root)
{
    node->left        = root->left;
    root->left->right = node;
    node->right       = root;
    root->left        = node;
}

template <class T>
void FibHeap<T>::insert(FibNode<T> *node)
{
    if(Size == 0)
        min = node;
    else
    {
        addNode(node,min);
        if(node->key < min->key)
        min = node;
    }
    Size++;
}

template <class T>
void FibHeap<T>::push(T key)
{
    FibNode<T> *node;
    node =new FibNode<T>(key);
    if(node==NULL) return;
    insert(node);
}

template <class T>
FibNode<T> *FibHeap<T>::extractMin()
{
    FibNode<T> *p = min;
    
    if(p == p->right)
        min = NULL;
    else
    {
        removeNode(p);
        min = p->right;
    }
    p->left = p->right = p;
    
    return p;
}

template<class T>
void FibHeap<T>::link(FibNode<T> *node,FibNode<T> *root)
{
    removeNode(node);

    if(root->child == NULL)
        root->child = node;
    else
        addNode(node,root->child);
    
    node->parent = root;
    root->degree++;
}

template <class T>
void FibHeap<T>::makeCons()
{
    int old = MaxDegree;
    MaxDegree = (log((Size))/log(2.0)) + 1;
    if(old >= MaxDegree)
        return;
    cons= (FibNode<T> **) realloc(cons,sizeof(FibHeap<T> *) * (MaxDegree + 1));
}

template <class T>
void FibHeap<T>::consolidate()
{
    int d, D;
    FibNode<T> *x, *y, *tmp;

    makeCons();
    D = MaxDegree + 1;

    for(int i = 0; i < D; i++) cons[i]=NULL;

    while (min != NULL)
    {
        x = extractMin();
        d = x->degree;
        while (cons[d] != NULL)
        {
            y = cons[d];

            if(x->key > y->key)
                swap(x, y);
            link(y, x);
            cons[d] = NULL;
            d++;
        }
        cons[d] = x;
    }
    min = NULL;

    for(int i = 0; i < D; i++)
    {
        if(cons[i] != NULL) 
        {
            if(min == NULL)
                min = cons[i];
            else
            {
                addNode(cons[i], min);
                if((cons[i])->key < min->key)
                    min = cons[i];
            }
            
        }
    }
}

template <class T>
void FibHeap<T>:: pop()
{
    if(min == NULL) return;

    FibNode<T> *child = NULL;
    FibNode<T> *m = min;

    while(m->child != NULL)
    {
        child = m->child;
        removeNode(child);

        if(child->right == child)
            m->child = NULL;
        else
            m->child = child->right;

        addNode(child, min);
        child->parent = NULL;
    }

    removeNode(m);
    if(m->right == m)
        min = NULL;
    else
    {
        min = m->right;
        consolidate();
    }

    Size--;

    delete m;
}

template <class T>
T FibHeap<T>::top() const
{
    if(min == NULL) assert(false);
    return min->key;
}

template <class T>
bool FibHeap<T>::empty() const
{
    if (Size == 0) return true;
    else return false;
}

template <class T>
int FibHeap<T>:: size() const
{
    return Size;
}

template <class T>
void FibHeap<T>::destroyNode(FibNode<T> *node)
{
    FibNode<T> *start = node;

    if(node == NULL)
        return;

    do {
        destroyNode(node->child);
        node = node->right;
        delete node->left;
    } while(node != start);
}

template <class T>
void FibHeap<T>::destroy()
{
    destroyNode(min);
    free(cons);
}
#endif 