#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <algorithm>


using namespace std;


struct Ship
{
    // CONSTRUCTOR
    Ship( int shipID, int length, char symbol, string name ) : m_shipID(shipID), m_length(length), m_symbol(symbol), m_name(name) {}

    int m_shipID, m_length;
    char m_symbol;
    string m_name;
    
};

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols) : m_rows(nRows), m_cols(nCols) {}
    ~GameImpl();
    
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
    
private:

    int m_rows;
    int m_cols;
    int m_nShips = 0;
    
    vector<Ship*> m_ship; // stores all the ships in the game!

};

// Destructor since m_ship is a vector of pointers
GameImpl::~GameImpl() {
    
    Ship* temp;
    
    while ( !m_ship.empty() ) {
        temp = m_ship.back();
        m_ship.pop_back();
        delete temp;
    }
    
}

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_cols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    Ship* newShip = new Ship( m_nShips++, length, symbol, name ); // creates a pointer to a new ship
    m_ship.push_back( newShip ); // then adds that pointer into the m_ship vector
    return true;
}

int GameImpl::nShips() const
{
    return m_nShips;
}

int GameImpl::shipLength(int shipId) const
{
    return m_ship[shipId]->m_length; // SOMETHING WRONG HERE
}

char GameImpl::shipSymbol(int shipId) const
{
    return m_ship[shipId]->m_symbol;
}

string GameImpl::shipName(int shipId) const
{
    return m_ship[shipId]->m_name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    
    int p1nShips = p1->game().nShips();
    int p2nShips = p2->game().nShips();
    
    // places ships on respective boards
    bool valid1 = p1->placeShips(b1);
    bool valid2 = p2->placeShips(b2);
    
    // if the ships were not all able to be placed, then return the nullptr
    if ( !valid1 || !valid2 ) {
        return nullptr;
    }
    
     // *** THE GAME STARTS *** //
    
    do {

        Player *winner, *loser;

        // *** FIRST PLAYER'S ATTACK *** //

        bool shotHit = false, shipDestroyed = false, validShot = false;
        int shipId = 100;

        cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
        b2.display( p1->isHuman() );

        Point attackPoint = p1->recommendAttack();


        // if it can attack
        if ( b2.attack( attackPoint, shotHit, shipDestroyed, shipId ) ) { // this call will update shotHit, shipDestroyed, and shipID accordingly

            // If it does not hit a spot
            if ( shotHit == false ) {
                cout << p1->name() << " attacked " << "(" << attackPoint.r << "," << attackPoint.c << ") and missed, resulting in:" << endl;
            }
            // If it hits a spot but does not destroy the boat
            else if ( shotHit == true && shipDestroyed != true ) {
                cout << p1->name() << " attacked " << "(" << attackPoint.r << "," << attackPoint.c << ") and hit something, resulting in:" << endl;
            }
            // if it hits a spot and destroys the boat
            else if ( shotHit == true && shipDestroyed == true ) {
                cout << p1->name() << " attacked " << "(" << attackPoint.r << "," <<attackPoint.c << ")" << " and destroyed the " << p2->game().shipName(shipId) << ", resulting in:" << endl;
                p2nShips--;
            }

        }
        else {
            cout << p1->name() << " wasted a shot at " << "(" << attackPoint.r << "," << attackPoint.c << ")" << endl;
        }
        
        // *** recordAttackResult and recordAttackByOpponent *** //
        p1->recordAttackResult( attackPoint, validShot, shotHit, shipDestroyed, shipId );
        p2->recordAttackByOpponent( attackPoint );

        // Displays after the attack
        if ( p1->isHuman() ) {
            b2.display( true );
        }
        else {
            b2.display( false );
        }
        
        // Should Pause?
        if ( shouldPause ) {
            waitForEnter();
        }
        
        // *** //
        
        if ( b1.allShipsDestroyed() ) {
            winner = p2;
            loser = p1;

            cout << winner->name() << " wins!" << endl;

            if ( loser->isHuman() ) {
                cout << "Here is where " << loser->name() << " ships were:" << endl;
                b2.display( false );
            }
            return winner;
        }
        if ( b2.allShipsDestroyed() ) {
            winner = p1;
            loser = p2;

            cout << winner->name() << " wins!" << endl;

            if ( loser->isHuman() ) {
                cout << "Here is where " << loser->name() << " ships were:" << endl;
                b1.display( false );
            }
            return winner;
        }
        
        // *** //

        // *** SECOND PLAYER'S ATTACK *** //

        bool shotHit2 = false, shipDestroyed2 = false, validShot2 = false;
        int shipId2 = 100;


        cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
        b1.display( p2->isHuman() );

        Point attackPoint2 = p2->recommendAttack();

        // if it can attack
        if ( b1.attack( attackPoint2, shotHit2, shipDestroyed2, shipId2 ) ) { // this call will update shotHit, shipDestroyed, and shipID accordingly

            if ( shotHit2 == false ) {
                cout << p2->name() << " attacked " << "(" << attackPoint2.r << "," << attackPoint2.c << ") and missed, resulting in:" << endl;
            }
            else if ( shotHit2 == true && shipDestroyed2 != true  ) {
                cout << p2->name() << " attacked " << "(" << attackPoint2.r << "," << attackPoint2.c << ") and hit something, resulting in:" << endl;
            }
            else if ( shotHit2 == true && shipDestroyed2 == true ) {
                cout << p2->name() << " attacked " << "(" << attackPoint2.r << "," <<attackPoint2.c << ")" << " and destroyed the " << p1->game().shipName(shipId2) << ", resulting in:" << endl;
                p1nShips--;
            }
            
            // *** recordAttackResult and recordAttackByOpponent *** //
            p2->recordAttackResult( attackPoint2, validShot2, shotHit2, shipDestroyed2, shipId2 );
            p1->recordAttackByOpponent( attackPoint2 );

        }
        else {
            cout << p2->name() << " wasted a shot at " << "(" << attackPoint2.r << "," << attackPoint2.c << ")" << endl;
        }

        // Displays after the attack
        if ( p2->isHuman() ) {
            b1.display( true ); // SHOULD BE TRUE
        }
        else {
            b1.display( false ); // SHOULD BE FALSE
        }
        
        // ShouldPause?
        if ( shouldPause ) {
            waitForEnter();
        }

        // check if all the ships are gone

        if ( b1.allShipsDestroyed() ) {
            winner = p2;
            loser = p1;

            cout << winner->name() << " wins!" << endl;

            if ( loser->isHuman() ) {
                cout << "Here is where " << loser->name() << " ships were:" << endl;
                b2.display( false );
            }
            return winner;
        }
        if ( b2.allShipsDestroyed() ) {
            winner = p1;
            loser = p2;

            cout << winner->name() << " wins!" << endl;

            if ( loser->isHuman() ) {
                cout << "Here is where " << loser->name() << " ships were:" << endl;
                b1.display( false );
            }
            return winner;
        }
        
        
    } while ( (!b1.allShipsDestroyed()) && (!b2.allShipsDestroyed()) );
    
    return nullptr;
    
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

