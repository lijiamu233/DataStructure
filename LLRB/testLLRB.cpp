#define CATCH_CONFIG_MAIN

#include <vector>
#include <algorithm>
#include <catch2/catch.hpp>
#include "llrb.h"

constexpr int n = 1000000;

TEST_CASE("test insert","[insert]")
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> insert_number(1, 1000000);

    int a[10] = {2,12,37,21,4,7,15,19,33,1};
    SECTION("insert once")
    {
        RBTree<int> *rb = new RBTree<int>();
        for(int i = 0;i < 10;i++)
        rb->insert(a[i]);
        REQUIRE(rb->print() == vector<int>{1,2,4,7,12,15,19,21,33,37});
        rb->clear();
    }

    SECTION("insert twice")
    {
        RBTree<int> *rb = new RBTree<int>();
        for(int i = 0;i < 10;i++)
        rb->insert(a[i]);
        for(int i = 0;i < 10;i++)
        rb->insert(a[i]);
        REQUIRE(rb->print() == vector<int>{1,2,4,7,12,15,19,21,33,37});
        rb->clear();
    }

    SECTION("insert 1~n")
    {
        RBTree<int> *rb = new RBTree<int>();
        for(int i = 1;i <= n; i++)
            rb->insert(i);
        vector<int> v = rb->print();

        REQUIRE(v.size() == n);
        
        for(size_t i = 0; i < n; i++)
            REQUIRE(v[i] == i+1);
        v.clear();
        rb->clear();
    }

    SECTION("insert random numbers")
    {
        RBTree<int> *rb = new RBTree<int>();
        vector<int> v;
        for(int i = 0;i < n; i++)
            v.push_back(insert_number(gen));
        sort(v.begin(), v.end());
        auto END = unique(v.begin(), v.end());
        v.erase(END, v.end());
        shuffle(v.begin(), v.end(), gen);
        for(auto &i : v) 
            rb->insert(i);
        sort(v.begin(), v.end());
        vector<int> vv = rb->print();
        REQUIRE(v.size() == vv.size());
        for(size_t i = 0;i < v.size(); i++)
            REQUIRE(v[i] == vv[i]);
        v.clear();
        vv.clear();
        rb->clear();
    }
}
TEST_CASE("test erase","[erase]")
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> ran(1, 1000000);
    
    SECTION("regularly erase odd numbers")
    {
        RBTree<int> *rb = new RBTree<int>();
        for(int i = 0;i < n; i++)
            rb->insert(i);
        for(int i = 1;i < n;i+=2)
            rb->erase(i);
        vector<int> v = rb->print();
        REQUIRE(v.size() == n/2);
        for(size_t i = 0;i < v.size(); i++)
            REQUIRE(v[i] == i * 2);
        
        rb->clear();
        v.clear();
    }

    SECTION("random insert and random erase")
    {
        RBTree<int> *rb = new RBTree<int>();
        set<int> s;
        for(int i = 0;i < n; i++)
            s.insert(ran(gen));
        
        vector<int> v(n);
        copy(s.begin(), s.end(), v.begin());
        for(auto &i : v)
            rb->insert(i);
        
        shuffle(v.begin(), v.end(), gen);
        for(auto &i : v)
            rb->erase(i);
        REQUIRE(rb->empty());

        rb->clear();
        s.clear();
        v.clear();
    }
}
TEST_CASE("test count","[count]")
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> insert_number(1, 1000000);

    SECTION("count all")
    {
        RBTree<int> *rb = new RBTree<int>();
        vector<int> v;
        for(int i = 0;i < n; i++)
            v.push_back(insert_number(gen));
        sort(v.begin(), v.end());
        auto END = unique(v.begin(), v.end());
        v.erase(END, v.end());
        shuffle(v.begin(), v.end(), gen);
        for(auto &i : v) 
            rb->insert(i);
        for(auto &i : v)
            REQUIRE(rb->count(i));
        rb->clear();
        v.clear();
    }
}
TEST_CASE("Compare with STL")
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> op(0, 2);
    uniform_int_distribution<> insert_number(0, 1000000);
    enum Op
    {
        Insert,Count,Erase
    };
    vector<Op> operation;
    RBTree<int> *rb=new RBTree<int>();
    set<int> s;
    for(int i = 0;i < n; i++)
        operation.push_back(Op(op(gen)));
    for(auto &i : operation)
    {
        switch (i)
        {
            case Insert:
            {
                int num = insert_number(gen);
                s.insert(num);
                rb->insert(num);
                break;
            }
            case Count:
            {
                uniform_int_distribution<> count_rand(0, s.size());
                int num = count_rand(gen);
                REQUIRE(s.count(num) == rb->count(num));
                break;
            }
            case Erase:
            {
                uniform_int_distribution<> count_rand(0, s.size());
                auto it = s.begin();
                advance(it, static_cast<int>(count_rand(gen)));
                int num = *it;
                s.erase(num);
                rb->erase(num);
                break;
            }
        }
    }
    REQUIRE(s.size() == rb->size());
    vector<int> v(s.size()), vv = rb->print();
    copy(s.begin(), s.end(), v.begin());
    REQUIRE(v == vv);

    rb->clear();
    v.clear();vv.clear();
    s.clear();
    operation.clear();
}

TEST_CASE("Iterator test")
{
    SECTION("test find")
    {
        RBTree<int> *rb = new RBTree<int>();
        for(int i = 1;i <= n; i++)
        rb->insert(i);
        for(int i = 1;i <= n; i+=2)
        rb->erase(i);
        for(int i = 2;i <= n; i+=2)
        REQUIRE(*rb->find(i) == i);
        rb->clear();
    }
    SECTION("test binary_search")
    {
        RBTree<int> *rb = new RBTree<int>();
        for(int i = 1;i <= n; i++)
        rb->insert(i);
        for(int i = 2;i <= n; i+=2)
        rb->erase(i);
        for (int i = 1; i <= n - 2; i++)
        {
            int pos_lower,pos_upper;
            pos_lower = (i&1) ? i : i+1;
            pos_upper = (i&1) ? i+2 : i+1;
            REQUIRE(*rb->lower_bound(i) == pos_lower);
            REQUIRE(*rb->upper_bound(i) == pos_upper);
        }
        rb->clear();
    }
}
