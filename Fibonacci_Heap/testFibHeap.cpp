
#define CATCH_CONFIG_MAIN 
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include "Fibonacci_Heap.h"
#include <catch2/catch.hpp>
using namespace std;

constexpr int n = 1000000;

TEST_CASE("test push,top and pop of integers")
{
    FibHeap<int>* hb=new FibHeap<int>();

    REQUIRE(hb->empty());

    int a[10] = {2,12,37,21,4,7,15,19,33,1};
    for(size_t i = 0;i < 10; i++)
        hb->push(a[i]);

    REQUIRE(hb->size() == 10);
    REQUIRE(hb->top() == 1);

    sort(a,a+10);
    for (size_t i = 0; i < 10; i++)
    {
        REQUIRE(hb->top() == a[i]);
        hb->pop();
        REQUIRE(hb->size() == 9 - i);
    }
    
    REQUIRE(hb->empty());
    hb->destroy();
}
TEST_CASE("compare with STL")
{
    enum Op {
        Push,Pop,Top
    };

    priority_queue<int,vector<int>,greater<int> > q;
    FibHeap<int>* hb = new FibHeap<int>();

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> op(0, 2);
    uniform_int_distribution<> insert_number(0, n*100);
    vector<Op> operations;
    for(int i = 0;i < n; i++)
        operations.push_back(Op(op(gen)));

    for(auto &i : operations)
    {
        switch (i)
        {
            case Push:
            {
                int data = insert_number(gen);
                q.push(data);
                hb->push(data);
                break;
            }
            case Pop:
            {
                REQUIRE(q.size() == hb->size());
                if (!hb->empty())
                {
                    q.pop();
                    hb->pop();
                }
                break;
            }
            case Top:
            {
                REQUIRE(q.size() == hb->size());
                if (!hb->empty())
                REQUIRE(q.top() == hb->top());
                break;
            }
        }
    }
    REQUIRE(q.size() == hb->size());
    while (!hb->empty())
    {
        REQUIRE(q.top() == hb->top());
        hb->pop();
        q.pop();
    }
    REQUIRE(hb->empty());
    hb->destroy();
}
