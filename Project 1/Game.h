//
//  Game.h
//  Project 1
//
//  Created by Izak Bunda on 4/3/22.
//

#ifndef GAME_H
#define GAME_H

class Mesa;

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nGarks);
    ~Game();

        // Mutators
    void play();

  private:
    Mesa* m_mesa;
    
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);

#endif /* Game_h */


