//
//  History.cpp
//  Project 1
//
//  Created by Izak Bunda on 4/5/22.
//

#include <iostream>
#include "History.h"

using namespace std;

History::History(int nRows, int nCols) {
    
    m_rows = nRows;
    m_cols = nCols;
    
    for (int r = 0; r < nRows; r++) {
        for (int c = 0; c < nCols; c++) {
            m_record[r][c] = '.';
        }
    }
}

bool History::record( int r, int c ) {

    if (m_record[r-1][c-1] == '.') {
        m_record[r-1][c-1] = 'A';
        return true;
    }
    else if (m_record[r-1][c-1] >= 'Z') {
        m_record[r-1][c-1] = 'Z';
        return true;
    }
    else {
        m_record[r-1][c-1]++;
        return true;
    }
    
    return false;
}

void History::display() const {
    clearScreen();
    
    for (int r = 0; r < m_rows; r++) {
        for (int c = 0; c < m_cols; c++) {
            if ((m_record[r][c] < 65) || (m_record[r][c] > 90))  {
                cout << ".";
            }
            else {
                cout << m_record[r][c];
            }
        }
        cout << endl;
    }
    cout << endl;
}

