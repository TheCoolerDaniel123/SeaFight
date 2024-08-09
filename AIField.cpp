#include "Lib.h"
#include "AIField.h"
#include "SeaFighter3000.h"

using namespace std;

void AIField::Draw()
{
    int i, n = 0;
    cout << "  | A | B | C | D | E | F | G | H | I | J |" << endl;
    cout << "-------------------------------------------";
    char sym1, sym2;
    if(ShowBorders)
    {
        sym1 = '|';
        sym2 = '-';
    }
    else
        sym1 = sym2 = ' ';
    for(i = 0; i < QUANTITY; i++)
    {
        if(!(i % 10)) //end of the current line of 10 cells
        {
            if(i)
                cout << endl << "---" << Symbol(sym2, 40);
            cout << endl << ++n;
            if(n < 10)
                cout << " ";
            cout << "|";
        }
        if(!ShowShips && Tiles[i].Type == TileType::SHIP) //hides ships from the player
            cout << " " << TileImages[TileType::UNKNOWN] << " " << sym1;
        else
            cout << " " << TileImages[Tiles[i].Type] << " " << sym1;
    }
    cout << endl;
}

void AIField::SetShips()
{
    SeaFighter3000::SetRandomly(*this);
    SetTilePointers();
}
