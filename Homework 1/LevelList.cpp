//
//  LevelList.cpp
//  Homework 1
//
//  Created by Izak Bunda on 4/11/22.


#include "LevelList.h"

LevelList::LevelList() {
}

// create an empty level list

bool LevelList::add(unsigned long level) {
    
    if ( (level >= 30) && (level <= 400) ) {
        
        levelSequence.insert(level);
        return true;
    }
    
    return false;
    
}

    // If the level is valid (a value from 30 to 400) and the level list
    // has room for it, add it to the level list and return true.
    // Otherwise, leave the level list unchanged and return false.


bool LevelList::remove(unsigned long level) {
    
    if ( levelSequence.find(level) != -1 ) {
        levelSequence.erase( levelSequence.find(level) );
        return true;
    }
    
    return false;
    
}

    // Remove one instance of the specified level from the level list.
    // Return true if a level was removed; otherwise false.

int LevelList::size() const {
    return levelSequence.size();
}

// Return the number of levels in the list.

unsigned long LevelList::minimum() const {
    
    if ( levelSequence.size() != 0 ) {
        ItemType min;
        levelSequence.get(0, min);
        return min;
    }
    return NO_LEVEL;
}

    // Return the lowest-valued level in the level list.  If the list is
    // empty, return NO_LEVEL.

unsigned long LevelList::maximum() const {
    
    if ( levelSequence.size() != 0 ) {
        ItemType max;
        levelSequence.get( size()-1 , max);
        return max;
    }
    return NO_LEVEL;
}
    // Return the highest-valued level in the level list.  If the list is
    // empty, return NO_LEVEL.


