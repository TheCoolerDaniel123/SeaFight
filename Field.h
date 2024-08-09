#pragma once
#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#define QUANTITY 100 //10x10 field = 100 cells
#define SHIPS_AMOUNT 10

struct Ship;

class Field
{
    public:
        Field();
        ~Field();

        struct FieldTile
        {
            int Type;
            Ship* PlacedShip;
            int Occupied;
        };

        FieldTile& Tile(int index){return Tiles[index];}
        std::vector<Ship>& ShipVector(){return Ships;}
        bool AliveShipsLeft();
        void ChangeView(){ShowBorders = !ShowBorders;}

        virtual void Draw() = 0;
        virtual void SetShips() = 0;

        bool AddToField(Ship& s);
        bool DeleteFromField(int length);
        void OccupyArea(Ship& s, bool occupy);

        void SetTilePointers();

        enum TileType
        {
            UNKNOWN = 0, EMPTY = 1, SHIP = 2, SHOT = 3
        };
        enum EdgeTile
        {
            TOP = 0, LOW = 1, RIGHT = 2, LEFT = 3, TOPRIGHT = 4, TOPLEFT = 5, LOWRIGHT = 6, LOWLEFT = 7, NONE = -1
        };
        static int EdgeOfField(int tile);

        std::string Symbol(char symbol, int amount);
    protected:
        std::vector<Ship> Ships;
        FieldTile* Tiles;
        static char* TileImages;
        bool ShowBorders;
};

#endif // FIELD_H_INCLUDED
