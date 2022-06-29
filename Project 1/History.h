//
//  History.h
//  Project 1
//
//  Created by Izak Bunda on 4/5/22.
//

#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h"

class History
    {
    public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
        
    private:
        int m_rows;
        int m_cols;
        char m_record[MAXROWS][MAXCOLS];
    };


#endif /* History_h */

