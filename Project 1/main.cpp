//
//  main.cpp
//  Project 1
//
//  Created by Izak Bunda on 4/3/22.
//

#include <iostream>
#include <string> // what about including this in Game.h
#include <random>
#include <utility>
#include <cstdlib>

using namespace std;

#include "Gark.h"
#include "Player.h"
#include "Mesa.h"
#include "Game.h"
#include "globals.h"

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

      // Play the game
    g.play();
}

