#pragma once
#ifndef SEAFIGHTER3000_H_INCLUDED
#define SEAFIGHTER3000_H_INCLUDED

class Field;

class SeaFighter3000
{
    public:
        SeaFighter3000(){}
        ~SeaFighter3000(){}

        static void SetRandomly(Field& field);
        static bool Shoot(Field& player_field);

        static void Init(int difficulty);

        enum ShotConditions
        {
            FIND_SHIP, FIND_DIR, TRY_FINISH
        };

        enum Directions
        {
            UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3
        };
        static int Opposite(int direction);
    private:
        static int Difficulty;
        static int ShotCondition;
        static std::vector<int> ToShoot;
};

#endif // SEAFIGHTER3000_H_INCLUDED
