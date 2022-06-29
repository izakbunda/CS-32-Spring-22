//
//  main.cpp
//  Homework 1
//
//  Created by Izak Bunda on 4/10/22.
//


#include "Sequence.h"
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

int main()
{
    test();
    test2();
    cout << "Passed all tests" << endl;
}
