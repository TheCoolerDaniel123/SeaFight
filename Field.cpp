#include "Lib.h"
#include "Field.h"
#include "Ship.h"

using namespace std;

char* Field::TileImages = NULL;

Field::Field()
{
    int i;
    Tiles = new FieldTile[QUANTITY];
    if(!TileImages)
        TileImages = new char[4] {' ', 'o', '#', '*'};
    for(i = 0; i < QUANTITY; i++)
    {
        Tiles[i].Type = TileType::UNKNOWN;
        Tiles[i].PlacedShip = NULL;
        Tiles[i].Occupied = 0;
    }
    ShowBorders = true;
}

Field::~Field()
{
    delete Tiles;
    if(TileImages)
        delete TileImages;
}

bool Field::AddToField(Ship& s)
{
    int step = 10, i; //step is 10 for vertically placed ships
    vector<int> processed;
    if(s.Coords[1] - s.Coords[0] < 5) //step 1 is for horizontally placed ships
        step = 1;
    for(i = s.Coords[0]; i <= s.Coords[1]; i += step)
        if(Tiles[i].Occupied)
            return false;
    s.Alive = s.CalculateLength();
    Ships.push_back(s);
    for(i = s.Coords[0]; i <= s.Coords[1]; i += step)
        Tiles[i].Type = TileType::SHIP;
    OccupyArea(s, true);
    return true;
}

bool Field::DeleteFromField(int length)
{
    vector<Ship>::reverse_iterator i = Ships.rbegin();
    while(i != Ships.rend())
    {
        if(i->CalculateLength() == length)
        {
            vector<Ship>::iterator j = (i + 1).base(); //converting reverse iterator to common iterator
            int k, step = 10;
            if(j->Coords[1] - j->Coords[0] < 5)
                step = 1;
            for(k = j->Coords[0]; k <= j->Coords[1]; k += step)
            {
                Tiles[k].Type = TileType::UNKNOWN;
                Tiles[k].PlacedShip = NULL;
            }
            OccupyArea(*j, false);
            Ships.erase(j);
            return true;
        }
        advance(i, 1);
    }
    return false;
}

void Field::OccupyArea(Ship& s, bool occupy)
{
    int value = 1, i, step = 10;
    if(!occupy)
        value = -1;
    if(s.Coords[1] - s.Coords[0] < 5)
        step = 1;
    const set<int>& nearby_area = s.NearbyArea(); //returns vector of tiles around ship
    for(auto el : nearby_area) //occupy tiles around ship
        Tiles[el].Occupied += value;
    for(i = s.Coords[0]; i <= s.Coords[1]; i += step) //occupy ship's tiles
        Tiles[i].Occupied += value;
}

void Field::SetTilePointers()
{
    if(Ships.size() < 10)
    {
        cout << "Field::SetPointers: invalid Ships.size() < 10" << endl;
        getchar();
        return;
    }
    int step, j;
    vector<Ship>::iterator i;
    for(i = Ships.begin(); i != Ships.end(); i++)
    {
        if(i->Coords[1] - i->Coords[0] < 5)
            step = 1;
        else
            step = 10;
        for(j = i->Coords[0]; j <= i->Coords[1]; j += step)
            Tiles[j].PlacedShip = &(*i); //each tile that has ship on it, pointing onto the full info about the ship
    }
}

bool Field::AliveShipsLeft()
{
    vector<Ship>::iterator i;
    for(i = Ships.begin(); i != Ships.end(); i++)
        if(i->Alive)
            return true;
    return false;
}

int Field::EdgeOfField(int tile)
{
    if(tile == 0)
        return TOPLEFT;
    if(tile == 9)
        return TOPRIGHT;
    if(tile == 90)
        return LOWLEFT;
    if(tile == 99)
        return LOWRIGHT;
    if(tile > 0 && tile < 9)
        return TOP;
    if(tile > 90 && tile < 99)
        return LOW;
    if(tile % 10 == 0)
        return LEFT;
    if(tile % 10 == 9)
        return RIGHT;
    return NONE;
}

string Field::Symbol(char symbol, int amount)
{
    int i;
    string res = "";
    for(i = 0; i < amount; i++)
        res.push_back(symbol);
    return res;
}



