#include "Lib.h"
#include "Game.h"

using namespace std;

int main()
{
    int Difficulty = 0;
    Game* SeaFight = NULL;
    do
    {
        system("CLS");
        cout << "Choose difficulty:" << endl << "[1] - Difficulty lvl 1" << endl << "[2] - Difficulty lvl 2" << endl;
        cin >> Difficulty;
        if(cin.fail())
            cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    while(Difficulty != 1 && Difficulty != 2);
    SeaFight = new Game(true, Difficulty);
    while(SeaFight->IsRunning())
    {
        SeaFight->Run();
    }
    return 0;
}
