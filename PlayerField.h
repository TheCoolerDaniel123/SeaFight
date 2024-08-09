#pragma once
#ifndef PLAYERFIELD_H_INCLUDED
#define PLAYERFIELD_H_INCLUDED

#include "Field.h"

class PlayerField : public Field
{
    public:
        PlayerField();
        ~PlayerField(){}

        std::string& PlayerName(){return Name;}

        void Draw() override;
        void HandleInput(std::string input, int* amounts);
        void SetShips() override;

        bool CorrectLetter(char letter);
        bool CorrectNumber(char* number, int length);
    private:
        std::string Name;
        bool ShowOccupied;
};

#endif // PLAYERFIELD_H_INCLUDED
