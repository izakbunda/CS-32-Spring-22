//
//  newSequence.cpp
//  Homework 1 (2)
//
//  Created by Izak Bunda on 4/12/22.
//

#include "newSequence.h"
#include <iostream>

using namespace std;

Sequence::Sequence() : m_length(0), maxLength(DEFAULT_MAX_ITEMS) { // default constructor
    array = new ItemType[DEFAULT_MAX_ITEMS];
}

Sequence::Sequence(int amount) : m_length(0) { // overloaded constructor
    maxLength = amount;
    array = new ItemType[maxLength];
}

Sequence& Sequence::operator=(const Sequence& amount) { // overloaded operator
    if ( this != &amount ) {
        
        delete [] array;
        m_length = amount.m_length;
        array = new ItemType[size()];
        
        // the copying:
        for (int i = 0; i < size(); i++) {
            array[i] = amount.array[i];
        }
    }
    return *this;
}

Sequence::Sequence(const Sequence& other) { // copy constructor
    m_length = other.size();
    array = new ItemType[size()];
    
    for (int i = 0; i < size(); i++) {
        array[i] = other.array[i];
    }
}

Sequence::~Sequence() { // destructor
    delete [] array;
}


bool Sequence::empty() const { // same as original in Sequence
    if (m_length == 0 ) {
        return false;
    }
    return true;
}

int Sequence::size() const { // same as original in Sequence
    return m_length;
}

void Sequence::dump() const {
    
    for (int i = 0; i < size(); i++) {
        cerr << array[i] << ",";
    }
}

int Sequence::insert(int pos, const ItemType& value) {
    
    if ( (pos < 0) || (pos > size()) || size() == maxLength ) {
        return -1;
    }
    

    for (int i = m_length; i > pos ; i--) { // starts from the back
        array[i] = array[i-1];                // if i = 1 that means that this is the first
    }                                         // call of this function
    
    m_length++;
    array[pos] = value;
    
    return pos;
    
}

// Insert value into the sequence so that it becomes the item at
// position pos.  The original item at position pos and those that
// follow it end up at positions one greater than they were at before.

// Return pos if 0 <= pos <= size() and the value could be
// inserted.  (It might not be, if the sequence has a fixed capacity,
// e.g., because it's implemented using a fixed-size array.)  Otherwise,
// leave the sequence unchanged and return -1.  Notice that
// if pos is equal to size(), the value is inserted at the end.

int Sequence::insert(const ItemType& value){
    
    if (size() != maxLength) { // Makes sure that this is not full!
        
        if (size() == 0) { // First term
            array[0] = value;
            m_length++;
            return 0;
        }

        for (int i = 0; i < size(); i++) {

            if (value <= array[i]) {

                for (int j = size(); j > i ; j--) { // starts from the back
                    array[j] = array[j-1];                // if j = 1 that means that this is the first
                }                                         // call of this function

                array[i] = value;
                m_length++;
                return i;
            }
        }
        
        m_length++;
        array[size()-1] = value;
        
        return -1;
    }

    return -1;
}

  // Let p be the smallest integer such that value <= the item at
  // position p in the sequence; if no such item exists (i.e.,
  // value > all items in the sequence), let p be size().  Insert
  // value into the sequence so that it becomes the item in position
  // p.  The original item at position p and those that follow it end
  // up at positions one greater than before.  Return p if the value
  // was actually inserted.  Return -1 if the value was not inserted
  // (perhaps because the sequence has a fixed capacity and is full).

bool Sequence::erase(int pos) { // keep the same
    
    if ( (pos >= 0) && (pos < size()) ) {
        
        for (int i = pos; i < size() ; i++) {
            array[i] = array[i+1];
        }
        m_length--;
        return true;
    }
    
    return false;
}

  // If 0 <= pos < size(), remove the item at position pos from
  // the sequence (so that all items that followed that item end up at
  // positions one lower than they were at before), and return true.
  // Otherwise, leave the sequence unchanged and return false.

int Sequence::remove(const ItemType& value){ // keep the same
    
    int rmCounter = 0;
    
    for (int i = 0; i < size() ; i++ ) {
        if (array[i] == value) {
            erase(i);
            i--;
            rmCounter++;
        }
    }
    
    return rmCounter;
    
}

  // Erase all items from the sequence that == value.  Return the
  // number of items removed (which will be 0 if no item == value).

bool Sequence::get(int pos, ItemType& value) const { // keep the same
    
    if ( (pos >= 0) && (pos < size()) ) {
        value = array[pos];
        return true;
    }
    
    return false;
    
}

  // If 0 <= pos < size(), copy into value the item at position pos
  // of the sequence and return true.  Otherwise, leave value unchanged
  // and return false.

bool Sequence::set(int pos, const ItemType& value) { // keep the same
    
    if ( (pos >= 0) && (pos < size()) ) {
        array[pos] = value;
        return true;
    }
    return false;
}
  // If 0 <= pos < size(), replace the item at position pos in the
  // sequence with value and return true.  Otherwise, leave the sequence
  // unchanged and return false.

int Sequence::find(const ItemType& value) const { // keep the same
    
    for ( int i = 0; i < size() ; i++ ) {
        if ( array[i] == value ) {
            return i;
        }
    }
    
    return -1;
    
}
  // Let p be the smallest integer such that value == the item at
  // position p in the sequence; if no such item exists, let p be -1.
  // Return p.

void Sequence::swap(Sequence& other) {
    
    int tempLength = 0;
    tempLength = m_length;
    m_length = other.m_length;
    other.m_length = tempLength;
    
    ItemType *p;
    p = array;
    array = other.array;
    other.array = p;
    
}
  // Exchange the contents of this sequence with the other one.


