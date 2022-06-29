//
//  Sequence.cpp
//  Project 2
//
//  Created by Izak Bunda on 4/16/22.
//

#include "Sequence.h"
#include <iostream>

using namespace std;

int subsequence(const Sequence& seq1, const Sequence& seq2) {
    
    if ( (seq1.size() < seq2.size() ) || seq2.empty() || seq1.empty() ) { // first checks if seq2 is larger than seq1 or if any are empty
        return -1;
    }
    
    ItemType x;
    seq2.get(0, x);
    
    int p = seq1.find(x); // sets variable p to the first location of the first item in seq2 (starting point)
    
    if ( p == -1 ) { // if it is not in seq1
        return -1; // then immediately return -1
    }
    
    if ( seq2.size() == 1 ) { // this is a special case for when seq2 has only one item
        
        ItemType l, m;
        
        for ( int i = 0; i < seq1.size(); i++ ) { // loops through seq1 to try
            
            seq1.get(i, l); // update the value of l
            seq2.get(0, m); // and m
            
            if ( l == m ) { // if they match, then that is where seq2 can be found
                return i;
            }
        }
        return -1; // if it finishes the entire loop and does not find a match, that means that seq2 could not be found in seq1
    }
    
    // this is the general case for all lengths of seq1 and seq2
    // again, like above, the index starts at the first instance of the first item of seq2 in seq1
    
    for (int i = p; i < seq1.size(); i++ ) { // it loops through the length of seq1
                                             
        for (int j = 0; j < seq2.size(); j++) { // and loops through the length of seq2, but this starts at the first item
            
            ItemType a; ItemType b; // updates the values of a and b based on the current index of i and j
            seq2.get(j, a); seq1.get(i, b);
             
            if ( a != b ) { // if they do not match, then break out of the inner for loop
                p = -1;     // which means that the whole process restarts...
                break;
            }
            i++; // ... but at the next index of seq1
        }
    }
    
    return p; // in the end, return whatever p has become
    
}


void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
    
    Sequence temp; // this takes care of aliasing !
     
    if ( seq1.empty() ) { // if seq1 is empty, then the result is set to seq2
        temp = seq2;
    }
    else if ( seq2.empty() ) { // if seq2 is empty, then the result is set to seq1
        temp = seq1;
    }
    else if ( seq1.empty() && seq2.empty() ) { // if they are both empty, nothing happens
        return;
    }
    else if ( !seq1.empty() && !seq2.empty() ) { // if they are not empty, then we are good to go
        
        // this is the special case for having the same length
        if ( seq1.size() == seq2.size() ) {
            int index = 0;
            
            for ( int i = 0; i < seq1.size() ; i++ ) { // loops through the length of seq1
                ItemType a;
                seq1.get(i, a); // first inserts a value from seq1
                temp.insert(index, a);
                index++;
                
                ItemType b;
                seq2.get(i, b); // then inserts a value from seq2
                temp.insert(index, b);
                index++;
            }
        }
            
        // this is case when seq1 is bigger than seq2
        else if ( seq1.size() > seq2.size() ) {
            
            int index = 0;
            int totalIndex = 0;
            
            for ( int i = 0; i < seq2.size() ; i++ ) { // this does the same pattern as the case above where a value is inserted from seq1 then seq2
                ItemType a;
                seq1.get(i, a);
                temp.insert(index, a);
                index++;
                
                ItemType b;
                seq2.get(i, b);
                temp.insert(index, b);
                index++;
                
                totalIndex++;
            }
            // once it gets through that (which means that seq2 is out of items)
            for ( int j = totalIndex; j < seq1.size(); j++ ) { // this just inserts the rest of seq1 into the result
                ItemType a;
                seq1.get(j, a);
                temp.insert(index, a);
                index++;
            }
            
        }
        
        // this is case when seq2 is bigger than seq1
        else if ( seq2.size() > seq1.size() ) {
            
            int index = 0;
            int totalIndex = 0;
            
            for ( int i = 0; i < seq1.size() ; i++ ) { // this does the same pattern as the case above where a value is inserted from seq1 then seq2
                ItemType a;
                seq1.get(i, a);
                temp.insert(index, a);
                index++;
                
                ItemType b;
                seq2.get(i, b);
                temp.insert(index, b);
                index++;
                
                totalIndex++;
            }
            // once it gets through that (which means that seq1 is out of items)
            for ( int j = totalIndex; j < seq2.size(); j++ ) { // this just inserts the rest of seq2 into the result
                ItemType a;
                seq2.get(j, a);
                temp.insert(index, a);
                index++;
            }
        }
        
    }
    result = temp; // then result is set to whatever temp is
    
}


Sequence::Sequence() : mSize(0) {
    node *dummyHead = new node; // creates the dummyHead

    head = dummyHead;
    
    head->prev = head; // points the prev to itself
    head->next = head; // points the next to itself
}

Sequence::~Sequence() {

    node *p = head->prev; // p starts from the back
    node *q = p->prev; // q startd from the back of p

    while ( p->prev != head ) { // loops through the linked list until the back is again the head
        p = p->prev; // this scoots p backward
        q = p->prev; // and q one step more backwards
        delete p->next; // then this deletes p's previous spot
    }
    
    if ( p == q ) { // this takes care of the situation in the end when they both point at the same spot
        delete p;
        return;
    }
    
    delete q; // otherwise, this deletes the head
    delete p; // and this deletes the second to last item
    
}

// destructor

Sequence::Sequence(const Sequence& other) : mSize(other.mSize) {
    
    for ( int index = 0; index < other.mSize; index++ ) { // loops for the length of other
        ItemType value;
        other.get(index, value); // inserts the value into other
        insert(index, value);
    }
}

// Copy constructor

Sequence& Sequence::operator=(const Sequence& rhs) {
    
    if ( this != &rhs ) { // this step is crucial because it prevents memory leaks!
        for (int i = mSize - 1; i >= 0; i--) { // this loop starts from the back
            erase(i); // and erases everything
        }
                
        mSize = 0;
        
        for (int index = 0; index < rhs.mSize; index++) { // then similar to the copy constructor
            ItemType value;
            rhs.get(index, value); // it just inserts the values into rhs
            insert(index, value);
        }
    }
    
    return *this; // if they are the same thing, then this saves a lot of time
}

// Overloaded operator

void Sequence::dump() {
    
    node* p = head->next;
    
    do {
        cerr << p->val << ",";
        p = p->next;
    } while ( p != head );
}

bool Sequence::empty() const {
    if ( mSize <= 0 ) {
        return true;
    }
    return false;
}

int Sequence::size() const {
    return mSize;
}

int Sequence::insert( int pos, const ItemType& value ) {
    if ( (pos < 0) || (pos > size() ) ) {
        return -1;
    }
    
    if ( (pos == 0) && (size() == 0) ) { // very first call of insert
        node* newNode = new node; // creates a new node
        newNode->val = value; // stores value into new node
        
        head->next = newNode; // next four lines points the prev and next to the appropriate locations
        head->prev = newNode;
        newNode->next = head;
        newNode->prev = head;
        
        mSize++; // and then increases the size
        
        return pos;
    }

    // this is the code for insert for every other call after the first
    
    int counter = 0;
    node *p = head; // start at head
    
    while ( p->next != head ) {
        if ( counter == pos ) {
            break;
        }
        counter++; // this counts how many nodes are in the linked list
        p = p->next;
    }
    
    node* newNode = new node; // this creates a new node
    newNode->val = value; // copies the value into the node
        
    newNode->prev = p;
    newNode->next = p->next;
    (p->next)->prev = newNode;
    p->next = newNode;
    
    mSize++; // increases the size
    
    return pos;
}

// Insert value into the sequence so that it becomes the item at
// position pos.  The original item at position pos and those that
// follow it end up at positions one greater than they were at before.
// Return pos if 0 <= pos <= size() and the value could be
// inserted. Otherwise, leave the sequence unchanged and return -1.
// Notice that if pos is equal to size(), the value is inserted at the end.



int Sequence::insert(const ItemType& value) {
    
    int counter = 0;
    node *p = head;
    
    while ( p->next != head ) { // similar to insert above, this keeps track of how many nodes are in the linked list
        if ( p->val == value ) { // the only difference is that we insert where the next item after it is larger
            break;
        }
        counter++;
        p = p->next;
    }
    
    node* newNode = new node;
    newNode->val = value;
        
    newNode->prev = p;
    newNode->next = p->next;
    (p->next)->prev = newNode;
    p->next = newNode;
    
    mSize++;
    
    return counter;
    
}

// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item in position
// p.  The original item at position p and those that follow it end
// up at positions one greater than before.  Return p if the value
// was actually inserted.

bool Sequence::erase(int pos) {
    if ( (pos >= 0) && (pos < size()) ) { // validates the parameters
        
        int counter = 0;
        node *p = head; // starts at head
        
        while ( p->next != head ) { // loops through the sequence until it gets to the head
            if ( counter == pos+1 ) { // if it reaches taht pos, then the loop is broken
                break;
            }
            counter++;
            p = p->next;
        }
        
        node *TBD = p; // and a temp "to be deleted" node is created
        
        (p->prev)->next = p->next;
        (p->next)->prev = p->prev;
        
        delete TBD; // saves us from a memory leak issue!
        
        mSize--; // decreases the size of the sequence
        return true;
    }
    
    return false;
}

// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed that item end up at
// positions one lower than they were at before), and return true.
// Otherwise, leave the sequence unchanged and return false.

int Sequence::remove(const ItemType& value) {
    
    int index = mSize;
    int removed = 0;
    node *p = head->prev; // starts behind the head

    while ( p != head ) { // loops through the whole linked list backwards
        if ( p->val == value ) { // if it finds a match, then...
            p = p->prev;
            erase(index-1); // it calls erase on that item

            removed++; // increases the removed variable
            index--;
            continue; // and conitnues down the line
        }
        index--; // if it does not find a match, then it just keeps traversing
        p = p->prev;
    }

    return removed;
}
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).

bool Sequence::get(int pos, ItemType& value) const {
    
    int counter = 0;
    node *p = head;
    
    if ( (pos >= 0) && (pos < size()) ) {
        
        while ( p->next != head ) { // this while loop is like an indexer
            if ( counter == pos+1 ) { // counter is the index
                break; // when it reaches the necessary position
            }
            counter++;
            p = p->next;
        }
        
        value = p->val; // it copies the value at that node into the parameter
        
        return true; // then returns true
    }
    
    return false;
    
}


// If 0 <= pos < size(), copy into value the item at position pos
// of the sequence and return true.  Otherwise, leave value unchanged
// and return false.

bool Sequence::set(int pos, const ItemType& value) {
    
    int counter = 0;
    node *p = head;
    
    if ( (pos >= 0) && (pos < size()) ) {
        
        while ( p->next != head ) {  // this while loop is like an indexer
            if ( counter == pos+1 ) { // counter is the index
                break; // when it reaches the necessary position
            }
            counter++;
            p = p->next;
        }
        
        p->val = value; // it sets the value of that node into the parameter
         
        return true; // then returns true
    }
    
    return false;
    
}
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.

int Sequence::find(const ItemType& value) const {
    
    int index = 0;
    node *p = head->next; // starts at the front
    
        
    while ( p != head ) { // traverses the sequence until it reaches head
        if ( p->val == value ) { // as soon as it sees a match
            return index; // it immediately returns
        }
    index++;
    p = p->next;
    }

    return -1;
    
}

// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.

void Sequence::swap(Sequence& other) {
    
    int tempSize;
    tempSize = size();
    mSize = other.size();
    other.mSize = tempSize;

    node *temp = head;
    head = other.head;
    other.head = temp;
    
    
}

// Exchange the contents of this sequence with the other one.
// the number of statement executions when swapping two sequences
// must be the same no matter how many items are in the sequences.
