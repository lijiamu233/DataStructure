//empty size Insert clear erase count
#ifndef LEFT_LEANING_RED_BLACK_TREE_H
#define LEFT_LEANING_RED_BLACK_TREE_H

#include <iostream>
#include <cstdio>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <vector>
using namespace std;

const bool RED   = true;
const bool BLACK = false;

template<class T>
class Node
{
    public:
        T Key;
        Node<T>* Left;
        Node<T>* Right;
        bool Color;
        int N;
        Node<T> *prev, *next;
        Node(T key):Key(key),
            Left(nullptr),Right(nullptr),N(1),Color(RED),prev(nullptr),next(nullptr){
            Key    = key;
            Left   = nullptr;
            Right  = nullptr;
            N      = 1;
            Color  = RED;
            prev   = nullptr;
            next   = nullptr;
        }
};

template<class T>
class RBTree
{
    private:
        Node<T>* root;
        int Size;
    public:
        RBTree();
        ~RBTree();
        void insert(T key);
        void erase(T key);
        size_t size();
        bool empty();
        int count(T key);
        void clear();
        vector<T> print() const;
        class Iterator
        {
            private:
                Node<T> *cur_{nullptr};
            public:
                //typedef typename RBTree<T>::Node NodeType;

                Iterator() = default;

                explicit Iterator(Node<T> *cur) : cur_(cur) {}

                Iterator &operator++()
                {
                    cur_ = cur_->next;
                    return *this;
                }

                Iterator &operator--()
                {
                    cur_ = cur_->prev;
                    return *this;
                }

                Iterator operator++(int)
                {
                    Iterator x = *this;
                    operator++();
                    return x;
                }

                Iterator operator--(int)
                {
                    Iterator x = *this;
                    operator--();
                    return x;
                }

                T &operator*()
                {
                    return cur_->Key;
                }

                bool operator==(const Iterator &l) const
                {
                    return cur_ == l.cur_;
                }

                bool operator!=(const Iterator &l) const
                {
                    return !operator==(l);
                }
        };
    Iterator find(const T &key) const;
    Iterator lower_bound(const T &key) const;
    Iterator upper_bound(const T &key) const;

    private:
        bool IsRed(Node<T>* x);
        int ssize(Node<T>* x);
        Node<T>* RotateLeft(Node<T>* x);
        Node<T>* RotateRight(Node<T>* x);
        void FlipColor(Node<T>* x);
        Node<T>* Insert(Node<T>* x,T key);
        Node<T>* Balance(Node<T>* x);
        Node<T>* MoveRedLeft(Node<T>* x);
        Node<T>* MoveRedRight(Node<T>* x);
        Node<T>* DeleteMin(Node<T>* x);
        Node<T>* Min(Node<T>* x);
        Node<T>* Delete(Node<T>* x,T key);
        void clear(Node<T>* x);
        void print(Node<T>* x, vector<T>* res) const;
};

template<class T>
RBTree<T>:: RBTree()
{
    root = nullptr;
    Size = 0;
}

template<class T>
RBTree<T>:: ~RBTree()
{
    
}

template<class T>
int RBTree<T>::ssize(Node<T>* x)
{
    if(x == nullptr) return 0;
    else return x->N;
}

template<class T>
bool RBTree<T>:: IsRed(Node<T>* x)
{
    if(x == nullptr) return false;
    return x->Color == RED;
}

template<class T>
Node<T>* RBTree<T>:: RotateLeft(Node<T>* x)
{
    Node<T>*tmp = x->Right;

	x->Right   = tmp->Left;
	tmp->Left  = x;
	tmp->Color = x->Color;
	x->Color   = RED;
	tmp->N     = x->N;
	x->N       = ssize(x->Left) + ssize(x->Right) + 1;

	return tmp;
}

template<class T>
Node<T>* RBTree<T>:: RotateRight(Node<T>* x)
{
    Node<T>*tmp = x->Left;

	x->Left    = tmp->Right;
	tmp->Right = x;
	tmp->Color = x->Color;
	x->Color   = RED;
	tmp->N     = x->N;
	x->N       = ssize(x->Left) + ssize(x->Right) + 1;

	return tmp;
}

template<class T>
void RBTree<T>:: FlipColor(Node<T>* x)
{
    x->Left->Color  = !x->Left->Color;
    x->Right->Color = !x->Right->Color;
    x->Color        = !x->Color;
}

template<class T>
Node<T>* RBTree<T>::Insert(Node<T>* x,T key)
{
    if(x == nullptr)
    {
        Node<T>* tmp = new Node<T>(key);
        Size++;
        return tmp;
    }
    if(x->Key > key)
    {
        auto ins = x->Left;
        x->Left = Insert(x->Left, key);
        if(ins == nullptr)
        {
            auto Ins = x->Left, pp = x->prev;
            if(pp != nullptr)
                pp->next = Ins;
            Ins->next = x;
            x->prev = Ins;
            Ins->prev = pp;
        }
    }
    else if(x->Key < key)
    {
        auto ins = x->Right;
        x->Right = Insert(x->Right, key);
        if(ins == nullptr)
        {
            auto Ins = x->Right, nn = x->next;
            x->next   = Ins;
            Ins->next = nn;
            if (nn != nullptr)
                nn->prev = Ins;
            Ins->prev = x;
        }
    }
    else
        return x;
    
    if(!IsRed(x->Left) && IsRed(x->Right))
        x = RotateLeft(x);
    if (x->Left != nullptr && IsRed(x->Left) && IsRed(x->Left->Left))
		x = RotateRight(x);
	if (IsRed(x->Left) && IsRed(x->Right))
		FlipColor(x);
	x->N = ssize(x->Left) + ssize(x->Right) + 1;
	return x;
}

template<class T>
void RBTree<T>:: insert(T key)
{
    root = Insert(root, key);
}

template<class T>
Node<T>* RBTree<T>:: Balance(Node<T>* x)
{
    if(IsRed(x->Right)) x = RotateLeft(x);

    if(x->Left != nullptr && IsRed(x->Left) && IsRed(x->Left->Left))
        x = RotateRight(x);
    
    if(IsRed(x->Left) && IsRed(x->Right))
        FlipColor(x);
    return x;
}

template<class T>
Node<T>* RBTree<T>:: MoveRedLeft(Node<T>* x)
{
    FlipColor(x);
	if (x->Right && IsRed(x->Right->Left))
	{
		x->Right = RotateRight(x->Right);
		x = RotateLeft(x);
		FlipColor(x);
	}
	return x;
}

template<class T>
Node<T>* RBTree<T>::DeleteMin(Node<T>* x)
{
	if (x->Left == nullptr)
	{
        auto xp = x->prev,xn = x->next;
        if (xp) xp->next = xn;
        if (xn) xn->prev = xp;
		delete x;
		return nullptr;
	}
	if (!IsRed(x->Left) && !IsRed(x->Left->Left))
		x = MoveRedLeft(x);
	x->Left = DeleteMin(x->Left);
	return Balance(x);
}

template<class T>
Node<T>* RBTree<T>::MoveRedRight(Node<T>* x)
{
	FlipColor(x);
	if (IsRed(x->Left->Left))
	{
		x = RotateRight(x);
		FlipColor(x);
	}
	return x;
}

template<class T>
Node<T>* RBTree<T>:: Min(Node<T>* x)
{
    if(x == nullptr) return nullptr;
    if(x->Left == nullptr) return x;
    return Min(x->Left);
}

template<class T>
size_t RBTree<T>:: size()
{
    return Size;
}

template<class T>
bool RBTree<T>:: empty()
{
    return !Size;
}

template<class T>
int RBTree<T>:: count(T key)
{
    Node<T>* x = root;
    while(x != nullptr)
    {
        if(x->Key == key) return 1;
        if(x->Key < key) x = x->Right;
        else x = x-> Left;
    }
    return false;
}

template<class T>
Node<T>* RBTree<T>:: Delete(Node<T>* x, T key)
{
    if(x->Key > key)
    {
        if(!IsRed(x->Left) && !IsRed(x->Left->Left))
            x = MoveRedLeft(x);
        x->Left = Delete(x->Left, key);
    }
    else
    {
        if(IsRed(x->Left)) x = RotateRight(x);
        if(x->Key == key && x->Right == nullptr)
        {
            auto xp = x->prev,xn = x->next;
            if (xp) xp->next = xn;
            if (xn) xn->prev = xp;
            delete x;
            return nullptr;
        }
        if(!IsRed(x->Right) && !IsRed(x->Right->Left))
            x = MoveRedRight(x);
        if(x->Key == key)
        {
            Node<T>* tmp=Min(x->Right);
            x->Key = tmp->Key;
            x->Right = DeleteMin(x->Right);
        }
        else x->Right = Delete(x->Right, key);
    }
    return Balance(x);
}

template<class T>
void RBTree<T>:: erase(T key)
{
    if(!count(key)) return;
    root = Delete(root, key);
    Size--;
}

template<class T>
void RBTree<T>:: clear(Node<T>* x)
{
    if(x != nullptr)
    {
        clear(x->Left);
        clear(x->Right);
        x->Left = x->Right = nullptr;
        delete x;
    }
}
template<class T>
void RBTree<T>:: clear()
{
    clear(root);
    root = nullptr;
    Size = 0;
}

template<class T>
void RBTree<T>:: print(Node<T>* x,vector<T> *res) const
{
    if(x == nullptr) return;
    print(x->Left, res);
    res->push_back(x->Key);
    print(x->Right, res);
}

template<class T>
vector<T> RBTree<T>:: print() const
{
    vector<T> v;
    print(root, &v);
    return v;
}

template<class T>
typename RBTree<T>::Iterator RBTree<T>::lower_bound(const T &key) const
{
    Node<T> *x = root, *ans = nullptr;
    while (x)
    {
        if(x->Key >= key && (ans == nullptr || x->Key < ans->Key))
            ans = x;
        if (key < x->Key)
            x = x->Left;
        else if (key == x->Key) break;
        else x = x->Right;
  }
  return Iterator(ans);
}

template<class T>
typename RBTree<T>::Iterator RBTree<T>::upper_bound(const T &key) const
{
    Iterator x = lower_bound(key);
    if (*x == key) return ++x;
    else return x;
}

template<class T>
typename RBTree<T>::Iterator RBTree<T>::find(const T &key) const
{
    auto x = root;
    while(x)
    {
        if(x->Key == key)
            return Iterator(x);
        if(x->Key < key)
            x = x->Right;
        else x = x->Left;
    }
    return Iterator(nullptr);
}
#endif