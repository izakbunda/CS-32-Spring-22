#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player {
    
  public:
    
    // Constructor
    HumanPlayer( string nm, const Game& g) : Player( nm, g ) {}
    
    // Destructor
    virtual ~HumanPlayer() {}
    
    // Functions
    virtual bool isHuman() const {
        return true;
    }
    
    // Do Something Functions
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    
    // Do Nothing Functions:
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {}
    virtual void recordAttackByOpponent(Point p) {}
    
};

bool HumanPlayer::placeShips(Board& b) {
    
    int r, c;
    char answer;
    Direction dir;
        
    for ( int i = 0; i < game().nShips() ; i++ ) {
        
        // Display the current state of the ship
        b.display( false );
        
        bool properAnswer = false; // bool used to keep track of the validity of answer

        // *** ASK FOR DIRECTION *** //
        
        do {

            // Display directions for entering a ship of specific length and name
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            cin >> answer;
            
            // Answer must be 'h' or 'v' or else the do while loop will repeat and ask for the direction again
            if ( (answer != 'h' ) && (answer != 'v') ) {
                cout << "Direction must be h or v." << endl;
                properAnswer = false;
            }
            else {
                properAnswer = true;
                break;
            }
            
        } while ( properAnswer != true );
        
        if ( answer == 'h' ) {
            dir = HORIZONTAL; }
        else {
            dir = VERTICAL; }
        
        cin.ignore(10000, '\n');
        
        bool canBePlaced = false; // bool used to keep track if ship can be placed or not
    
        
        // *** ASK FOR COORDINATES *** //
        
        do {
            
            bool intOnly = false; // bool used to keep track if the human player has entered valid integers

            if ( dir == HORIZONTAL ) {
                cout << "Enter the row and column of leftmost cell (e.g., 3 5): ";
            }
            else {
                cout << "Enter the row and column of topmost cell (e.g., 3 5): ";
            }
            
            intOnly = getLineWithTwoIntegers(r, c); // bool used to check if the items entered are integers

            // Answer must be int or else the do while loop will repeat and ask for coordinates again
            if ( intOnly ) {
                if ( b.placeShip( Point(r,c), i, dir ) ) {
                    canBePlaced = true;
                    break;
                }
                if ( !b.placeShip( Point(r,c), i, dir) ) {
                    cout << "The ship cannot be placed there." << endl;
                    canBePlaced = false;
                }
            }
            else {
                cout << "You must enter two integers." << endl;
                canBePlaced = false;
            }
            
        } while ( (r < 0) || (r > game().rows()-1) || (c < 0) || (c > game().cols()-1) || (canBePlaced == false) );
    }
    
    // If it passes through all the tests above, then the ship can be placed
    
    return true;
}

Point HumanPlayer::recommendAttack() {
    
    int r, c;
    
    cout << "Enter the row and column to attack (e.g. 3 5): ";
    
    cin >> r >> c;
    Point point(r, c);
    
    cin.ignore(10000, '\n');
    
    return point;
}


//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player {
    
  public:
    // Constructor
    MediocrePlayer( string nm, const Game& g) : Player( nm, g ) {}
    
    // Destructor
    virtual ~MediocrePlayer() {}
    
    // Do Something Functions
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    
    // Do Nothing Function
    virtual void recordAttackByOpponent(Point p) {} // Does nothing
    
  private:
    vector<Point> possibleShots;
    vector<Point> shotsTaken;
    int state = 1;
    Point savePoint;

};


bool placeHelper( int nShips, const Game& g, Board& b ) {
    
    bool placedShip = true;
    
    if ( nShips >= 0 ) {
        
        for ( int r = 0; r < g.rows(); r++ ) {
            for ( int c = 0; c < g.cols(); c++ ) {
                
                // Tries placing the ship horizontally
                placedShip = b.placeShip( Point(r,c), nShips, HORIZONTAL );
                
                // If successful, then move onto the next ship
                if ( placedShip ) {
                    
                    placedShip = placeHelper( nShips-1, g, b );
                    
                    if (placedShip) {
                        return true;
                    }
                    
                }
                // If not successful, then unplace the ship and try placing it vertically
                if ( !placedShip ) {
                    
                    b.unplaceShip( Point(r,c), nShips, HORIZONTAL );
                    
                    placedShip = b.placeShip( Point(r,c), nShips, VERTICAL );
                    
                    // If successful, then move onto the next ship
                    if ( placedShip ) {
                        
                        placedShip = placeHelper( nShips-1, g, b );
                        
                        if (placedShip) {
                            return true;
                        }
                    }
                    
                    // If not successful, then unplace the ship -> move on to next Point
                    if ( !placedShip ) {
                        b.unplaceShip( Point(r,c), nShips, VERTICAL );
                    }

                }
            }
        }
        
    }
    
    return placedShip;
    
}




bool MediocrePlayer::placeShips(Board& b) {
    
    int counter = 0;
    bool placedAll = false;
    int nShips = game().nShips()-1;
    
    // It will try to place all the ships only 50 times
    while ( (counter < 50) && ( placedAll == false ) ) {
        
        b.block();
        placedAll = placeHelper(  nShips , game() , b );
        b.unblock();

        counter++;
    }
    
    return placedAll;
    
}

bool isUsed( Point p, vector<Point> v ) {
    
    for ( int i = 0; i < v.size(); i++ ) {

        if ( v[i].r == p.r && v[i].c == p.c ) { // if Point is present
            return true; // then it isUsed
        }
    }
    
    return false;
}


Point MediocrePlayer::recommendAttack() {

    bool shotTaken = false;
    Point recPoint;
    
    // this guarantees that recPoint hasn't been chosen before
    
    if ( state == 1 ) {
        
        while ( shotTaken == false ) {
            
            recPoint = game().randomPoint();
             
            if ( !shotsTaken.empty() ) {
                
                // Checks if the recPoint is already in the shotsTaken vector
                for ( int i = 0; i < shotsTaken.size(); i++ ) {
                                    
                    if( (shotsTaken[i].r == recPoint.r) && (shotsTaken[i].c == recPoint.c) ) { // if Point is present in shotsTaken
                        shotTaken = false;
                        break;
                    }
                    else {
                        shotTaken = true;
                    }
                }

            }
            else {
                shotTaken = true;
            }
        }
        
        shotsTaken.push_back(recPoint);
        
        return recPoint;
        
    }
    
    if ( state == 2 ) { // do the cross of 16
                
        if ( possibleShots.empty()) { // will only add the cross if the all possible shots have been used
            
            // *** ADDS THE CROSS OF FOUR *AVAILABLE* SPOTS IN ALL DIRECTIONS *** //
            
            // NORTH
            for ( int i = 1; i <= 4; i++ ) {
                
                if ( !isUsed( Point(savePoint.r-i, savePoint.c) , shotsTaken ) && (savePoint.r-i >= 0) && (savePoint.c >= 0) && (savePoint.r-i < game().rows()) && (savePoint.c < game().cols()) ) { // if the point is not in shotsTaken
                    
                    possibleShots.push_back( Point(savePoint.r-i, savePoint.c) ); // then add it to possibleShots
                }
            }
            
            // EAST
            for ( int i = 1; i <= 4; i++ ) {
                
                if ( !isUsed( Point(savePoint.r, savePoint.c+i) , shotsTaken ) && (savePoint.r >= 0) && (savePoint.c+i >= 0) && (savePoint.r < game().rows()) && (savePoint.c+i < game().cols()) ) { // if the point is not in shotsTaken
                    
                    possibleShots.push_back( Point(savePoint.r, savePoint.c+i) ); // then add it to possibleShots
                }
            }
            
            // SOUTH
            for ( int i = 1; i <= 4; i++ ) {
                
                if ( !isUsed( Point(savePoint.r+i, savePoint.c) , shotsTaken ) && (savePoint.r+i >= 0) && (savePoint.c >= 0) && (savePoint.r+i < game().rows()) && (savePoint.c < game().cols()) ) { // if the point is not in shotsTaken
                    
                    possibleShots.push_back( Point(savePoint.r+i, savePoint.c) ); // then add it to possibleShots
                }
            }
            
            // WEST
            for ( int i = 1; i <= 4; i++ ) {
                
                if ( !isUsed( Point(savePoint.r, savePoint.c-i) , shotsTaken ) && (savePoint.r >= 0) && (savePoint.c-i >= 0) && (savePoint.r < game().rows()) && (savePoint.c-i < game().cols()) ) { // if the point is not in shotsTaken
                    
                    possibleShots.push_back( Point(savePoint.r, savePoint.c-i) ); // then add it to possibleShots
                }
            }
        }
        
        // *** ACTUALLY RECOMMENDS AN ATTACK *** //
        
        if ( !possibleShots.empty() ) {

            int rand = randInt( int(possibleShots.size()) ); // randomizer

            Point tempPoint = possibleShots[ rand ]; // pick a random point

            possibleShots.erase( possibleShots.begin()+rand ); // then erases the item that was recommended from possibleShots since it has been used already

            recPoint = tempPoint;

            shotsTaken.push_back( recPoint ); // considers this spot as used
        }

    }
    
    return recPoint;
    
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {
        
    // *** STATE ONE *** //
    
    if ( state == 1 ) {
                
        if ( !shotHit ) { // shot isn't hit
            state = 1;
        }
        
        else if ( shotHit && shipDestroyed ) { // got lucky
            state = 1;
        }
        
        else if ( shotHit && !shipDestroyed ) { // shot hit but not destroyed
            
            if ( possibleShots.size() == 0 ) { // will only create a new "center" for the cross if all possibleShots of the last time has been used

                savePoint = p;
            }
            
            state = 2;
            return;

        }
        
    }
        
    // *** STATE TWO *** //
    
    else if ( state == 2 ) {
                
        if ( possibleShots.size() == 0 ) { // finished the cross
            state = 1;
        }
        
        else if ( !shotHit ) { // shot isn't hit
            state = 2;
        }
        else if ( shotHit && !shipDestroyed ) { // just shot hit
            state = 2;
        }
        else if ( shotHit && shipDestroyed ) { // shot hit AND destroyed ship
            
            possibleShots.clear();
            state = 1;
            
        }
        
    }
    
}

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.

class GoodPlayer : public Player {

  public:
    // Constructor
    GoodPlayer( string nm, const Game& g) : Player( nm, g ) {}

    // Destructor
    virtual ~GoodPlayer() {}

    // Do Something Functions
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);

    // Do Nothing Function
    virtual void recordAttackByOpponent(Point p) {} // Does nothing, sorry

  private:
    vector<Point> possibleShots;
    vector<Point> everyOtherShots;
    vector<Point> shotsTaken;

    int state = 1;
    Point savePoint;

};

bool GoodPlayer::placeShips(Board& b) {

    // this is a default layout that according to many studies online is the "best" placement of ships for a standard Battleship game
    
    if ( game().cols() == 10 && game().rows() == 10 ) {
        
        if ( game().shipLength(0) == 5 && game().shipSymbol(0) == 'A' ) { // aircraft carrier
            b.placeShip( Point(0,4), 0, HORIZONTAL );
        }
        if ( game().shipLength(1) == 4 && game().shipSymbol(1) == 'B') { // battleship
            b.placeShip( Point(6,0), 1, VERTICAL );
        }
        if ( game().shipLength(2) == 3 && game().shipSymbol(2) == 'D' ) { // destroyer
            b.placeShip( Point(8,3), 2, HORIZONTAL );
        }
        if ( game().shipLength(3) == 3 && game().shipSymbol(3) == 'S' ) { // submarine
            b.placeShip( Point(7,6), 3, HORIZONTAL );
        }
        if ( game().shipLength(4) == 2 && game().shipSymbol(4) == 'P' ) { // patrol boat
            b.placeShip( Point(4,5), 4, VERTICAL );
        }
        
        return true;
        
    }
    
    // If the tester suddenly tries to play a game with GoodPlayer that is not standard, then it places ships the same way as a mediocre ship (recursively)
    
    else {
        
        int counter = 0;
        bool placedAll = false;
        int nShips = game().nShips()-1;

        // It will try to place all the ships only 50 times
        while ( (counter < 50) && ( placedAll == false ) ) {

            b.block();
            placedAll = placeHelper(  nShips , game() , b );
            b.unblock();

            counter++;
        }

        return placedAll;
    }
}

Point GoodPlayer::recommendAttack() {
    
    Point recPoint;
    
    if ( state == 1 ) {
        
        if ( everyOtherShots.empty() ) {

            for ( int r = 0; r < game().rows(); r++ ) {

                if ( r%2 == 0 ) { // if row is an even number

                    for ( int c = 0; c < 10; c++ ) {

                        if ( c%2 == 0 ) {
                            everyOtherShots.push_back( Point(r, c) );
                        }
                    }
                }
                else { // if row is not an even number

                    for ( int c = 0; c < 10; c++ ) {

                        if ( c%2 != 0 ) {
                            everyOtherShots.push_back( Point(r, c) );
                        }
                    }
                }
            }
            
        }
        
        Point tempPoint;

        tempPoint = everyOtherShots.front();

        shotsTaken.push_back( tempPoint );

        recPoint = tempPoint;

        everyOtherShots.erase( everyOtherShots.begin() );
        
        return recPoint;
    }
    
    if ( state == 2 ) { // do the cross of 4
                
        if ( possibleShots.empty()) { // will only add the cross if the all possible shots have been used
            
            // *** ADDS THE CROSS OF 1 *AVAILABLE* SPOT IN ALL FOUR DIRECTIONS *** //
            
            // NORTH
            for ( int i = 1; i <= 1; i++ ) {
                
                if ( !isUsed( Point(savePoint.r-i, savePoint.c) , shotsTaken ) && (savePoint.r-i >= 0) && (savePoint.c >= 0) && (savePoint.r-i < game().rows()) && (savePoint.c < game().cols()) ) { // if the point is not in shotsTaken
                    
                    possibleShots.push_back( Point(savePoint.r-i, savePoint.c) ); // then add it to possibleShots
                }
            }
            
            // EAST
            for ( int i = 1; i <= 1; i++ ) {
                
                if ( !isUsed( Point(savePoint.r, savePoint.c+i) , shotsTaken ) && (savePoint.r >= 0) && (savePoint.c+i >= 0) && (savePoint.r < game().rows()) && (savePoint.c+i < game().cols()) ) { // if the point is not in shotsTaken
                    
                    possibleShots.push_back( Point(savePoint.r, savePoint.c+i) ); // then add it to possibleShots
                }
            }
            
            // SOUTH
            for ( int i = 1; i <= 1; i++ ) {
                
                if ( !isUsed( Point(savePoint.r+i, savePoint.c) , shotsTaken ) && (savePoint.r+i >= 0) && (savePoint.c >= 0) && (savePoint.r+i < game().rows()) && (savePoint.c < game().cols()) ) { // if the point is not in shotsTaken
                    
                    possibleShots.push_back( Point(savePoint.r+i, savePoint.c) ); // then add it to possibleShots
                }
            }
            
            // WEST
            for ( int i = 1; i <= 1; i++ ) {
                
                if ( !isUsed( Point(savePoint.r, savePoint.c-i) , shotsTaken ) && (savePoint.r >= 0) && (savePoint.c-i >= 0) && (savePoint.r < game().rows()) && (savePoint.c-i < game().cols()) ) { // if the point is not in shotsTaken
                    
                    possibleShots.push_back( Point(savePoint.r, savePoint.c-i) ); // then add it to possibleShots
                }
            }
            
        }
        
        // *** ACTUALLY RECOMMENDS AN ATTACK *** //
        
        if ( !possibleShots.empty() ) {

            int rand = randInt( int(possibleShots.size()) ); // randomizer

            Point tempPoint = possibleShots[ rand ]; // pick a random point

            possibleShots.erase( possibleShots.begin()+rand ); // then erases the item that was recommended from possibleShots since it has been used already

            recPoint = tempPoint;

            shotsTaken.push_back( recPoint ); // considers this spot as used

        }

    } // end of state 2
    
    return recPoint;
    
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {
    
    
    // *** STATE ONE *** //
    
    if ( state == 1 ) {
                
        if ( !shotHit ) { // shot isn't hit
            state = 1;
        }
        
        else if ( shotHit && shipDestroyed ) { // got lucky
            state = 1;
        }
        
        else if ( shotHit && !shipDestroyed ) { // shot hit but not destroyed
            
            if ( possibleShots.size() == 0 ) { // will only create a new "center" for the cross if all possibleShots of the last time has been used

                savePoint = p;
            }
            
            state = 2;
            return;

        }
        
    }
        
    // *** STATE TWO *** //
    
    else if ( state == 2 ) {
                
        if ( possibleShots.size() == 0 ) { // finished the cross            
            state = 1;
        }
        
        else if ( !shotHit ) { // shot isn't hit
            state = 2;
        }
        
        else if ( shotHit && !shipDestroyed ) { // just shot hit
            
//            savePoint2 = p;
            
            state = 2; // enter state 2
        }
        
        else if ( shotHit && shipDestroyed ) { // shot hit AND destroyed ship
            
            possibleShots.clear();

            state = 1;
        }
        
    }

}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0]) && type != types[pos]; pos++);
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
