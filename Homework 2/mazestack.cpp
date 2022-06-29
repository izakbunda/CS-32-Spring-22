//
//  mazestack.cpp
//
//  Created by Izak Bunda on 4/24/22.
//

#include <iostream>
#include <stack>

using namespace std;

class Coord {
    public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc){}
    int r() const {
        return m_row; }
    int c() const {
        return m_col; }
            
    private:
    int m_row;
    int m_col;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec);
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise

bool pathExists(char maze[][10], int sr, int sc, int er, int ec) {
    
    stack<Coord> coordStack;
    
    Coord start(sr, sc);
    Coord end(er, ec);
    
    Coord current(sr, sc);
    
    coordStack.push(start); //Push the starting coordinate (sr,sc) onto the coordinate stack and
    maze[sr][sc] = '*'; // update maze[sr][sc] to indicate that the algorithm has encountered it
    
    while ( !coordStack.empty() ) { // While the stack is not empty,
        
        current = coordStack.top();
        coordStack.pop(); // Pop the top coordinate off the stack. This gives you the current (r,c) location that your algorithm is exploring.
        
        if ( (current.r() == end.r()) && (current.c() == current.c()) ) { // If the current (r,c) coordinate is equal to the ending coordinate, then we've solved the maze
            return true; // so return to true!
        }
        
        if ( maze[current.r()][current.c()-1] == '.' ) {  // If you can move WEST and haven't encountered that cell yet,
            coordStack.push( Coord( current.r(), current.c()-1 ) ); // then push the coordinate (r,c-1) onto the stack
            maze[current.r()][current.c()-1] = '*'; // and update maze[r][c-1] to indicate the algorithm has encountered it.
        }
        if (maze[current.r()-1][current.c()] == '.' ) { // If you can move NORTH and haven't encountered that cell yet,
            coordStack.push( Coord( current.r()-1, current.c() ) ); // then push the coordinate (r-1,c) onto the stack
            maze[current.r()-1][current.c()] = '*'; // and update maze[r-1][c] to indicate the algorithm has encountered it.
        }
        if (maze[current.r()][current.c()+1] == '.' ) { // If you can move EAST and haven't encountered that cell yet,
            coordStack.push( Coord( current.r(), current.c()+1 ) ); // then push the coordinate (r,c+1) onto the stack
            maze[current.r()][current.c()+1] = '*'; // and update maze[r][c+1] to indicate the algorithm has encountered it.
        }
        if (maze[current.r()+1][current.c()] == '.' ) { // If you can move SOUTH and haven't encountered that cell yet,
            coordStack.push( Coord( current.r()+1, current.c() ) ); // then push the coordinate (r+1,c) onto the stack
            maze[current.r()+1][current.c()] = '*'; // and update maze[r+1][c] to indicate the algorithm has encountered it.
        }
    }
    
    return false; // There was no solution, so return false
    
}
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise

// REMINDER TO CHANGE THE ARRAY BEFORE SUBMITTING!!!


int main()
        {
            char maze[10][10] = {
                { 'X','X','X','X','X','X','X','X','X','X' },
                { 'X','.','X','.','.','.','X','.','.','X' },
                { 'X','.','X','.','X','X','X','X','.','X' },
                { 'X','.','X','.','.','X','.','X','.','X' },
                { 'X','.','X','X','.','X','.','.','.','X' },
                { 'X','.','.','.','.','.','.','X','X','X' },
                { 'X','.','X','X','X','X','.','X','.','X' },
                { 'X','.','.','.','.','X','X','X','.','X' },
                { 'X','.','X','.','.','X','.','.','.','X' },
                { 'X','X','X','X','X','X','X','X','X','X' }
            };
        
            if (pathExists(maze, 5,6, 8,1))
                cout << "Solvable!" << endl;
            else
                cout << "Out of luck!" << endl;
        }
