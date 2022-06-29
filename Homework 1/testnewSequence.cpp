//
//  main.cpp
//  Homework 1 (2)
//
//  Created by Izak Bunda on 4/12/22.
//

#include "newSequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Sequence s;
    s.insert(0, 10);
    s.insert(0, 20);
    assert(s.size() == 2);
    ItemType x = 999;
    assert(s.get(0, x) && x == 20);
    assert(s.get(1, x) && x == 10);
}

void test2()
{
    Sequence t;
    t.insert(0, 1);
    t.insert(1, 1);
    t.insert(2, 4);
    t.insert(3);
    t.remove(1);
    
    t.dump();
}


int main()
{
    test();
    test2();
    cout << "Passed all tests" << endl;
}
