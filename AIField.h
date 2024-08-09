#pragma once
#ifndef AIFIELD_H_INCLUDED
#define AIFIELD_H_INCLUDED

#include "Field.h"

class AIField : public Field
{
    public:
        AIField(){ShowShips = false;}
        ~AIField(){}

        void Draw() override;
        void SetShips() override;

        void Display(){ShowShips = !ShowShips;}
    private:
        bool ShowShips;
};

#endif // AIFIELD_H_INCLUDED
