#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    
    char m_board[MAXROWS][MAXCOLS];
    
    const Game& m_game;
    
};

BoardImpl::BoardImpl(const Game& g) : m_game(g)
{
    for ( int r = 0; r < g.rows(); r++ ) {
        for ( int c = 0; c < g.cols(); c++ ) {
            m_board[r][c] = '.';
        }
    }
    
}

void BoardImpl::clear()
{
    for ( int r = 0; r < m_game.rows(); r++ ) {
        for ( int c = 0; c < m_game.cols(); c++ ) {
            m_board[r][c] = '.';
        }
    }
}

void BoardImpl::block()
{
    // Block cells with 50% probability - as stated in update - "[N/2]"
    int n = ((m_game.rows() * m_game.cols()) / 2);
    
    while ( n > 0 ) {
        Point randPoint = m_game.randomPoint();
        
        if ( m_board[randPoint.r][randPoint.c] == '.' ) {
            m_board[randPoint.r][randPoint.c] = '#';
            n--;
        }
    }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if( m_board[r][c] == '#' ) {
                m_board[r][c] = '.';
            }
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    
    // the shipId is invalid
    if ( (shipId < 0) || (shipId > m_game.nShips()) ) {
        return false;
    }
    
    // *** HORIZONTAL *** //

    if ( dir == HORIZONTAL ) {
        
        if ( (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols()) ) { // checks if it will go outside the borders
            return false;
        }
        
        if ( m_game.isValid(topOrLeft) ) { // initial checks
            
            for ( int i = 0; i < m_game.shipLength(shipId) ; i++  ) {
                if( m_board[topOrLeft.r][topOrLeft.c+i] != '.' ) { // the ship would overlap an already-placed ship or blocked spot
                    return false;
                }
            }
        }
    }
    
    // *** VERTICALS *** //
    
    if ( dir == VERTICAL ) {
        
        if ( (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows()) ) { // checks if it will go outside the borders
            return false;
        }
        
        if ( m_game.isValid(topOrLeft) ) { // initial checks
            
            for ( int i = 0; i < m_game.shipLength(shipId) ; i++  ) {
                if( m_board[topOrLeft.r+i][topOrLeft.c] != '.' ) { // the ship would overlap an already-placed ship or blocked spot
                    return false;
                }
            }
        }
    }
    
    // the ship with that ship ID has previously been placed on this Board and not yet been unplaced since its most recent placement
    for ( int r = 0; r < m_game.rows(); r++ ) {
        for ( int c = 0; c < m_game.cols(); c++ ) {
            if ( m_board[r][c] == m_game.shipSymbol(shipId) ) {
                return false;
            }
        }
    }
    
    // actually places it on the board
    if ( dir == HORIZONTAL ) {
        for ( int i = 0; i < m_game.shipLength(shipId); i++ ) {
            m_board[topOrLeft.r][topOrLeft.c+i] = m_game.shipSymbol(shipId);
        }
    }
    if ( dir == VERTICAL ) {
        for ( int i = 0; i < m_game.shipLength(shipId); i++ ) {
            m_board[topOrLeft.r+i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
    }
    
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    
    // checks if the shipId is invalid
    if ( (shipId < 0) || (shipId > m_game.nShips()) ) {
        return false;
    }
    
    // checks if the entirety of the ship is actually there
    if ( dir == HORIZONTAL ) {
        for ( int i = 0; i < m_game.shipLength(shipId); i++ ) {
            if ( m_board[topOrLeft.r][topOrLeft.c+i] != m_game.shipSymbol(shipId) ) {
                return false;
            }
        }
    }
    if ( dir == VERTICAL ) {
        for ( int i = 0; i < m_game.shipLength(shipId); i++ ) {
            if ( m_board[topOrLeft.r+i][topOrLeft.c] != m_game.shipSymbol(shipId) ) {
                return false;
            }
        }
    }
    
    // unplaces the entirety of the ship
    if ( dir == HORIZONTAL ) {
        for ( int i = 0; i < m_game.shipLength(shipId); i++ ) {
            m_board[topOrLeft.r][topOrLeft.c+i] = '.';
        }
    }
    if ( dir == VERTICAL ) {
        for ( int i = 0; i < m_game.shipLength(shipId); i++ ) {
            m_board[topOrLeft.r+i][topOrLeft.c] = '.';
        }
    }
    
    return true;
}

void BoardImpl::display(bool shotsOnly) const
{
    
    // *** FIRST LINE *** //
    
    cout << "  ";
    for ( int i = 0; i < m_game.cols(); i++ ) {
        cout << i;
    }
    cout << endl;
    
    if ( !shotsOnly ) { // FALSE
        for ( int r = 0; r < m_game.rows(); r++ ) {
            
            cout << r << " "; // Number of row and then a space
            
            for ( int c = 0; c < m_game.cols(); c++ ) {
                cout << m_board[r][c]; // the rest of the row
            }
            cout << endl; // then a space
        }
    }
    
    if ( shotsOnly ) { // TRUE
        for ( int r = 0; r < m_game.rows(); r++ ) {
            
            cout << r << " "; // Number of row and then a space
            
            for ( int c = 0; c < m_game.cols(); c++ ) {
                if ( (m_board[r][c] != 'X') && (m_board[r][c] != 'o') ) { // Checks if a spot is a shot/hit segment
                    cout << '.';
                }
                else {
                    cout << m_board[r][c]; // otherwise display the rest of the row
                }
            }
            cout << endl; // then a space
        }
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    int tempShipId = 0; // initialized to 0 to make Xcode shut up
    char tempSymbol = m_board[p.r][p.c]; // looks for this later
    
    // checks if the attack is on the board
    if ( !m_game.isValid(p) ) {
        shotHit = false;
        shipDestroyed = false;
        return false;
    }
    
    // checks if the position has already been attacked
    if ( m_board[p.r][p.c] == 'o' ) {
        shotHit = false;
        shipDestroyed = false;
        return false;
    }
    
    // checks if the point is even a ship/ship segment
    if ( (m_board[p.r][p.c] == 'X') || (m_board[p.r][p.c] == 'o') ) {
        shotHit = false;
        shipDestroyed = false;
        return false;
    }
        
    // Goes through the board to determine the shipId at this Point
    for ( int r = 0; r < m_game.rows(); r++ ) {
        for ( int c = 0; c < m_game.cols(); c++ ) {
            
            // loops through all the shipIds
            for ( int i = 0; i < m_game.nShips() ; i++ ) {
                
                // checks if the symbol at this location matches with the symbol of an already delcared ship
                if ( tempSymbol == m_game.shipSymbol(i) ) {
                    tempShipId = i; // then stores that shipId
                }
            }
        }
    }
    
    // *** THE BOARD WILL BE ALTERED FROM HERE ON *** //

    
    // Checks if there is an undamaged part of the ship there
    if ( (m_board[p.r][p.c] != 'X') && (m_board[p.r][p.c] != 'o') ) {
        
        if ( m_board[p.r][p.c] == '.' ) {
            m_board[p.r][p.c] = 'o'; // marks that an empty spot has been hit
            shotHit = false; // updates the parameter shotHit
        }
        else {
            m_board[p.r][p.c] = 'X'; // marks that a ship segment has been hit
            shotHit = true; // updates the parameter shotHit
        }
    }
    else {
        shotHit = false; // updates the parameter shotHit
    }
    
    int found = 0;
    
    // checks if the whole ship has been destroyed
    if ( shotHit ) {
        for ( int r = 0; r < m_game.rows() && !found ; r++ ) {
            for ( int c = 0; c < m_game.cols() && !found ; c++ ) {
                if ( m_board[r][c] == tempSymbol ) { // if it can't find tempSymbol, this means the whole ship is gone
//                    destroyed = false; // so set shipDestroyed param to true
                    found++;
                }
            }
        }
    }
    
    // If the a ship was destroyed, then we must change the shipId to the match the one that was destroyed
    if ( found == 0 ) {
        shipDestroyed = true;
        shipId = tempShipId;
    }
    else {
        shipDestroyed = false;
    }
    
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    
    for ( int r = 0; r < m_game.rows(); r++ ) {
        for ( int c = 0; c < m_game.cols(); c++ ) {
            
            // Checks for one ship at a time
            for ( int i = 0; i < m_game.nShips(); i++ ) {
               
                if ( m_board[r][c] == m_game.shipSymbol(i) ) {
                    return false;
                }
            }
        }
    }
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
