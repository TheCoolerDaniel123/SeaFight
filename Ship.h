#pragma once
#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include<iostream>

struct Ship
{
    Ship(){}
    Ship(char l1, char l2, int num1, int num2);
    Ship(const Ship &ref);
    ~Ship(){}

    int CalculateLength() const;
    static int CalculateLength(char l1, char l2, int num1, int num2);
    void SetCoords(char l1, char l2, int num1, int num2);
    const std::set<int>& NearbyArea();

    friend std::ostream& operator<<(std::ostream& stream, const Ship& s);
    Ship operator=(Ship second);

    int Coords[2];
    int Alive;
};

#endif // SHIP_H_INCLUDED
