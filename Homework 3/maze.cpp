
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

bool pathExists(char maze[][10], int sr, int sc, int er, int ec) {
    
    
    Coord start(sr, sc);
    Coord end(er, ec);
    
    
    if ( (start.r() == end.r()) && (start.c() == end.c()) ) {
        return true;
    }
    
    maze[sr][sc] = '*';
    
    // WEST
    if ( maze[start.r()][start.c()-1] == '.' ) {
        if ( pathExists( maze, sr, sc-1, er, ec) ) {
            return true;
        }
    }
    
    // NORTH
    if ( maze[start.r()-1][start.c()] == '.' ) {
        if ( pathExists( maze, sr-1, sc, er, ec) ) {
            return true;
        }
    }
    // EAST
    if ( maze[start.r()][start.c()+1] == '.' ) {
        if ( pathExists( maze, sr, sc+1, er, ec) ) {
            return true;
        }
    }
    
    // SOUTH
    if ( maze[start.r()+1][start.c()] == '.' ) {
        if ( pathExists( maze, sr+1, sc, er, ec) ) {
            return true;
        }
    }
    
    return false;
    
}
