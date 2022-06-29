#include "Game.h"
#include "Player.h"
#include <iostream>
#include <string>

using namespace std;

bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
           g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat");
}

int main()
{
    const int NTRIALS = 100;

    cout << "Select one of these choices for an example of the game:" << endl;
    cout << "  1.  A mini-game between two mediocre players" << endl;
    cout << "  2.  A mediocre player against a human player" << endl;
    cout << "  3.  A " << NTRIALS
         << "-game match between a mediocre and an awful player, with no pauses"
         << endl;
    cout << "Enter your choice: ";
    string line;
    getline(cin,line);
    if (line.empty())
    {
        cout << "You did not enter a choice" << endl;
    }
    else if (line[0] == '1')
    {
        Game g(2, 3);
        g.addShip(2, 'R', "rowboat");
        Player* p1 = createPlayer("mediocre", "Popeye", g);
        Player* p2 = createPlayer("mediocre", "Bluto", g);
        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '2')
    {
        Game g(10, 10);
        addStandardShips(g);
        Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
        Player* p2 = createPlayer("human", "Shuman the Human", g);
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '3')
    {
        int nMediocreWins = 0;

        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
                 << " =============================" << endl;
            Game g(10, 10);
            addStandardShips(g);
            Player* p1 = createPlayer("awful", "Awful Audrey", g);
            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
            Player* winner = (k % 2 == 1 ?
                                g.play(p1, p2, false) : g.play(p2, p1, false));
            if (winner == p2)
                nMediocreWins++;
            delete p1;
            delete p2;
        }
        cout << "The mediocre player won " << nMediocreWins << " out of "
             << NTRIALS << " games." << endl;
          // We'd expect a mediocre player to win most of the games against
          // an awful player.  Similarly, a good player should outperform
          // a mediocre player.
    }
//    else if (line[0] == '4')
//    {
//        Game g(10, 10);
//        addStandardShips(g);
//        Player* p1 = createPlayer("awful", "Awful Alex", g);
//        cerr << "p1" << endl;
//        Player* p2 = createPlayer("human", "Shuman the Human", g);
//        cerr << "p2" << endl;
//        g.play(p1, p2);
//        cerr << "play" << endl;
//        delete p1;
//        delete p2;
//    }
//    else if (line[0] == '4')
//    {
//        Game g(10, 10);
//        g.addShip(2, 'R', "rowboat");
//        Player* p1 = createPlayer("awful", "Awful Alex", g);
//        Player* p2 = createPlayer("human", "Shuman the Human", g);
//        g.play(p1, p2);
//        delete p1;
//        delete p2;
//    }
//    else if (line[0] == '4')
//    {
//        Game g(10, 10);
//        addStandardShips(g);
////        g.addShip(2, 'R', "rowboat");
////        g.addShip(3, 'W', "warship");
////        g.addShip(4, 'A', "airboat");
//        Player* p1 = createPlayer("mediocre", "maria", g);
//        Player* p2 = createPlayer("human", "Shuman the Human", g);
//        g.play(p1, p2);
//        cerr << "play" << endl;
//        delete p1;
//        delete p2;
//    }
    else if (line[0] == '4')
    {
        Game g(10, 10);
        addStandardShips(g);
        Player* p1 = createPlayer("good", "Gabby the Good Player", g);
        Player* p2 = createPlayer("human", "Shuman the Human", g);
        g.play(p1, p2);
        cerr << "play" << endl;
        delete p1;
        delete p2;
    }
    else if (line[0] == '5')
    {
        int nGoodWins = 0;

        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
                 << " =============================" << endl;
            Game g(10, 10);
            addStandardShips(g);
            Player* p1 = createPlayer("good", "Good Gabby", g);
            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
            Player* winner = (k % 2 == 1 ?
                                g.play(p1, p2, false) : g.play(p2, p1, false));
            if (winner == p1)
                nGoodWins++;
            delete p1;
            delete p2;
        }
        cout << "The good player won " << nGoodWins << " out of "
             << NTRIALS << " games." << endl;
          // We'd expect a mediocre player to win most of the games against
          // an awful player.  Similarly, a good player should outperform
          // a mediocre player.
    }
    else
    {
       cout << "That's not one of the choices." << endl;
    }
}


//int main()
//{
//    Game game(10,10);
//
//    game.addShip(5, 'A', "aircraft carrier");
//    game.addShip(4, 'B', "battleship");
//    game.addShip(3, 'D', "destroyer");
//    game.addShip(3, 'S', "submarine");
//    game.addShip(2, 'P', "patrol boat");
//
//    Board board( game );
//    Player* Mediocre = createPlayer("mediocre", "TEST", game);
//    int count = 0;
//
//    for(int i = 0; i < 100; i++)
//    {
//        if(Mediocre->placeShips(board))
//            count++;
//        board.clear();
//    }
//    cout << "Placed " << count << " times" << endl;
//
//    // board.display(false);
//}
