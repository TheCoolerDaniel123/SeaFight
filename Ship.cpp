#include "Lib.h"
#include "Ship.h"
#include "Field.h"

using namespace std;

Ship::Ship(char l1, char l2, int num1, int num2)
{
    SetCoords(l1, l2, num1, num2); //calling to avoid replicating code
    Alive = CalculateLength();
}

Ship::Ship(const Ship &ref)
{
    Alive = ref.Alive;
    Coords[0] = ref.Coords[0];
    Coords[1] = ref.Coords[1];
}

Ship Ship::operator=(Ship second) //the types aren't refs to be able to use vector.erase() (it uses this version of operator)
{
    Alive = second.Alive;
    Coords[0] = second.Coords[0];
    Coords[1] = second.Coords[1];
    return *this;
}

ostream& operator<<(ostream& stream, const Ship& s)
{
    int l = s.CalculateLength();
    stream << endl << "Coords are [" << s.Coords[0] << ", " << s.Coords[1] << "]; on the field: ";
    stream << (char)(s.Coords[0] % 10 + 97) << s.Coords[0] / 10 + 1 << (char)(s.Coords[1] % 10 + 97) << s.Coords[1] / 10 + 1 << endl;
    stream << "Alive: " << s.Alive << endl << "Length: " << l << endl;
    return stream;
}

int Ship::CalculateLength() const
{
    int len;
    if(abs(Coords[0] - Coords[1]) > 4)
        len = abs(Coords[0] - Coords[1]) / 10 + 1;
    else
        len = abs(Coords[0] - Coords[1]) + 1;
    return len;
}

int Ship::CalculateLength(char l1, char l2, int num1, int num2)
{
    if(l1 == l2)
        return abs(num1 - num2) + 1;
    else
        if(num1 == num2)
            return abs(l1 - l2) + 1;
    return -1;
}

void Ship::SetCoords(char l1, char l2, int num1, int num2)
{
    Coords[0] = (num1 - 1) * 10 + l1 - 97;
    Coords[1] = (num2 - 1) * 10 + l2 - 97;
    if(Coords[0] > Coords[1])
        swap(Coords[0], Coords[1]);
}

const set<int>& Ship::NearbyArea()
{
    static set<int> TilesAround;
    TilesAround.clear();
    const int LEFT = -1, RIGHT = 1, DOWN = 10, UP = - 10, DOWNLEFT = 9, DOWNRIGHT = 11, UPLEFT = - 11, UPRIGHT = -9;
    int i, step = 10;
    vector<int> directions;
    if(Coords[1] - Coords[0] < 5)
        step = 1;
    for(i = Coords[0]; i <= Coords[1]; i += step)
    {
        switch(Field::EdgeOfField(i))
        {
            case Field::EdgeTile::TOP:
                directions.insert(directions.end(), {LEFT, RIGHT, DOWNLEFT, DOWNRIGHT, DOWN});
                break;
            case Field::EdgeTile::LOW:
                directions.insert(directions.end(), {LEFT, UPLEFT, UP, UPRIGHT, RIGHT});
                break;
            case Field::EdgeTile::TOPLEFT:
                directions.insert(directions.end(), {RIGHT, DOWNRIGHT, DOWN});
                break;
            case Field::EdgeTile::LOWLEFT:
                directions.insert(directions.end(), {RIGHT, UPRIGHT, UP});
                break;
            case Field::EdgeTile::LEFT:
                directions.insert(directions.end(), {UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN});
                break;
            case Field::EdgeTile::TOPRIGHT:
                directions.insert(directions.end(), {LEFT, DOWNLEFT, DOWN});
                break;
            case Field::EdgeTile::LOWRIGHT:
                directions.insert(directions.end(), {LEFT, UPLEFT, UP});
                break;
            case Field::EdgeTile::RIGHT:
                directions.insert(directions.end(), {UP, UPLEFT, LEFT, DOWNLEFT, DOWN});
                break;
            case Field::EdgeTile::NONE:
                directions.insert(directions.end(), {UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN, DOWNLEFT, LEFT, UPLEFT});
                break;
        }
        for(auto el : directions)
            TilesAround.insert(i + el);
        directions.clear();
    }
    for(i = Coords[0]; i <= Coords[1]; i += step)
        TilesAround.erase(i);
    return TilesAround;
}

