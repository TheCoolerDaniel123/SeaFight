#include "Lib.h"
#include "PlayerField.h"
#include "SeaFighter3000.h"
#include "Ship.h"

using namespace std;

PlayerField::PlayerField()
{
    Name = "Player";
    ShowOccupied = true;
}

void PlayerField::Draw()
{
    int i, n = 0, info_index = 0, limit;
    char sym1, sym2;
    if(ShowBorders)
    {
        sym1 = '|';
        sym2 = '-';
    }
    else
        sym1 = sym2 = ' ';
    if(Ships.size() < SHIPS_AMOUNT)
        limit = 15;
    else
        limit = 5;
    static string info[15] = {"The upper field is yours and the lower(appears after start) is AI's", "[ ] means unknown area on the map",
                      "[o] means the tile was shot, but ship isn't there", "[#] means your ship is placed there",
                      "[*] means the ship was placed there and now it got shot", "['] means you can't place ship on this tile", "",
                      "Command example | Command purpose", "---------------------------------",
                      "a4c4 (or a4)    | Place ship on the map using '[begin_pos][end_pos]' (or just '[pos]' for single ones)",
                      "del3 (or del)   | Delete last ship of length N you placed using 'delN' (or just 'del' for any last ship)",
                      "clear           | Delete all ships on the map using 'clear'",
                      "name            | Change you name",
                      "switch          | Enable/disable showing ['] on the tiles next to ships (enabled by default)",
                      "random          | Your ships will be placed randomly"};
    cout << "  | A | B | C | D | E | F | G | H | I | J |" << endl;
    cout << "-------------------------------------------";
    for(i = 0; i < QUANTITY; i++)
    {
        if(!(i % 10)) //end of the current line of 10 cells
        {
            if(i)
            {
                if(info_index < limit)
                    cout << Symbol(' ', 5) << info[info_index++];
                cout << endl << "---" << Symbol(sym2, 40);
            }
            if(info_index < limit)
                cout << Symbol(' ', 5) << info[info_index++];
            cout << endl << ++n;
            if(n < 10)
                cout << " ";
            cout << "|";
        }
        if(Tiles[i].Occupied && Tiles[i].Type == TileType::UNKNOWN && Ships.size() < SHIPS_AMOUNT && ShowOccupied)
            cout << " ' " << sym1;
            //cout << " " << Tiles[i].Occupied << " " << sym1; //showing the exact number or ships that occupy this tile
        else
            cout << " " << TileImages[Tiles[i].Type] << " " << sym1;/*
        else
        {
            static int N = 0;
            if(i < 10) cout << " ";
            cout << N++ << " " << sym1;
        }*/
    }
    cout << endl;
}

void PlayerField::SetShips()
{
    bool placing_done = false;
    char desision;
    string input;
    int ships[4] = {4, 3, 2, 1};
    while(!placing_done)
    {
        system("CLS");
        Draw();
        cout << endl << "To start, you have to place:" << endl;
        cout << ships[0] << " ship(s) of length 1" << endl;
        cout << ships[1] << " ship(s) of length 2" << endl;
        cout << ships[2] << " ship(s) of length 3" << endl;
        cout << ships[3] << " ship(s) of length 4" << endl << Name + ">";
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        HandleInput(input, ships);
        if(!ships[0] && !ships[1] && !ships[2] && !ships[3])
        {
            do
            {
                system("CLS");
                Draw();
                cout << endl << "Do you want to start the game with this field?" << endl;
                cout << "y - Yes, let's go" << endl << "n - No, let me change something" << endl;
                cin >> desision;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            while(desision != 'y' && desision != 'n');
            if(desision == 'y')
            {
                placing_done = true;
                SetTilePointers();
            }
            else
                HandleInput("del", ships); //deleting last placed ship
        }
    }
}

void PlayerField::HandleInput(string input, int* amounts)
{
    int length;
    bool flag1, flag2, try_add = false;
    Ship s;
    switch(input.length()) //trying to get info about string accuracy
    {
        case 2:
            if(CorrectLetter(input[0]) && CorrectNumber(&input[1], 1))
            {
                s.SetCoords(input[0], input[0], input[1] - '0', input[1] - '0');
                try_add = true;
            }
            break;
        case 3:
            if(input == "del" && !Ships.empty()) //deleting any last ship that was placed
            {
                length = (Ships.end() - 1)->CalculateLength();
                DeleteFromField(length);
                amounts[length - 1]++;
                return;
            }
            if(CorrectLetter(input[0]) && CorrectNumber(&input[1], 2))
            {
                s.SetCoords(input[0], input[0], 10, 10);
                try_add = true;
            }
            break;
        case 4:
            if(input == "name")
            {
                system("CLS");
                Draw();
                cout << endl << "Enter your name: ";
                cin >> Name;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if(Name.length() > 20)
                    Name = Name.substr(0, 20);
                return;
            }
            if(input.substr(0, 3) == "del" && input[3] - '0' >= 1 && input[3] - '0' <= 4) //deleting ships
            {
                if(DeleteFromField(input[3] - '0'))
                    amounts[input[3] - '0' - 1]++;
                return;
            }
            if(CorrectLetter(input[0]) && CorrectNumber(&input[1], 1) && CorrectLetter(input[2]) && CorrectNumber(&input[3], 1))
            {
                length = Ship::CalculateLength(input[0], input[2], input[1] - '0', input[3] - '0');
                if(length <= 4 && length >= 1)
                {
                    s.SetCoords(input[0], input[2], input[1] - '0', input[3] - '0');
                    try_add = true;
                }
            }
            break;
        case 5:
            if(input == "clear")
            {
                while(!Ships.empty())
                {
                    length = (Ships.end() - 1)->CalculateLength();
                    DeleteFromField(length);
                    amounts[length - 1]++;
                }
                return;
            }
            flag1 = (CorrectLetter(input[0]) && CorrectNumber(&input[1], 1) && CorrectLetter(input[2]) && CorrectNumber(&input[3], 2));
            flag2 = (CorrectLetter(input[0]) && CorrectNumber(&input[1], 2) && CorrectLetter(input[3]) && CorrectNumber(&input[4], 1));
            if(flag1)
            {
                length = Ship::CalculateLength(input[0], input[2], input[1] - '0', 10);
                if(length <= 4 && length >= 1)
                {
                    s.SetCoords(input[0], input[2], input[1] - '0', 10);
                    try_add = true;
                }
            }
            if(flag2)
            {
                length = Ship::CalculateLength(input[0], input[3], 10, input[4] - '0');
                if(length <= 4 && length >= 1)
                {
                    s.SetCoords(input[0], input[3], 10, input[4]  - '0');
                    try_add = true;
                }
            }
            break;
        case 6:
            if(input == "switch")
            {
                ShowOccupied = !ShowOccupied;
                return;
            }
            if(input == "random")
            {
                int i;
                HandleInput("clear", amounts);
                for(i = 0; i < 4; i++)
                    amounts[i] = 0;
                SeaFighter3000::SetRandomly(*this);
                return;
            }
            if(CorrectLetter(input[0]) && CorrectNumber(&input[1], 2) && CorrectLetter(input[3]) && CorrectNumber(&input[4], 2))
            {
                length = Ship::CalculateLength(input[0], input[3], 10, 10);
                if(length <= 4 && length >= 1)
                {
                    s.SetCoords(input[0], input[3], 10, 10);
                    try_add = true;
                }
            }
            break;
        default: //debug
            if(input == "ShipVector") {for(auto el : Ships) cout << el << endl; getchar();}
            if(input == "VichmatTop") Name = "Mr.Mardanov";
            return;
    }
    if(try_add)
    {
        length = s.CalculateLength();
        if(amounts[length - 1] && AddToField(s))
            amounts[length - 1]--;
    }
}

bool PlayerField::CorrectLetter(char letter)
{
    if(letter >= 97 && letter <= 106)
        return true;
    return false;
}

bool PlayerField::CorrectNumber(char* number, int length)
{
    switch(length)
    {
        case 1:
            if(number[0] - '0' >= 1 && number[0] - '0' <= 9)
                return true;
            return false;
        case 2:
            if(number[0] == '1' && number[1] == '0')
                return true;
            return false;
    }
}

