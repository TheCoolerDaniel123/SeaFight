#include "Lib.h"
#include "Game.h"
#include "SeaFighter3000.h"
#include "PlayerField.h"
#include "AIField.h"
#include "Ship.h"

using namespace std;

Game::Game()
{
    Player = new PlayerField();
    Bot = new AIField();
    Running = true;
    GameState = GameConditions::SETTING;
    srand(time(0));
    if(rand() % 2)
        PlayerTurn = true;
    else
        PlayerTurn = false;
    SeaFighter3000::Init(rand() % 2 + 1);
}

Game::Game(bool player_turn, int difficulty)
{
    Player = new PlayerField();
    Bot = new AIField();
    Running = true;
    GameState = GameConditions::SETTING;
    PlayerTurn = player_turn;
    SeaFighter3000::Init(difficulty);
}

Game::~Game()
{
    delete Player;
    delete Bot;
}

void Game::Run()
{
    switch(GameState)
    {
        case SETTING:
            Player->SetShips();
            Bot->SetShips();
            GameState = PLAYING;
            break;
        case PLAYING:
            system("CLS");
            Player->Draw();
            cout << endl;
            Bot->Draw();
            /*Bot->Draw();
            cout << endl;
            Player->Draw();*/

            if(PlayerTurn)
            {
                HandleInput();
                if(!Bot->AliveShipsLeft())
                {
                    PlayerWins = true;
                    GameState = ENDED;
                }
            }
            else
            {
                PlayerTurn = SeaFighter3000::Shoot(*Player);
                if(!Player->AliveShipsLeft())
                {
                    PlayerWins = false;
                    GameState = ENDED;
                }
            }
            break;
        case ENDED:
            //endgame words you cool blah blah or you re loser and staff;
            Running = false;
            break;
    }
}

void Game::HandleInput()
{
    static string prev_event = "Your first move; type 'help' for more info";
    static vector<int> processed;
    string input;
    int index = -1;
    cout << prev_event << endl << Player->PlayerName() << ">";
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch(input.length())
    {
        case 2:
            if(Player->CorrectLetter(input[0]) && Player->CorrectNumber(&input[1], 1))
                index = input[0] - 97 + (input[1] - '0' - 1) * 10;
            break;
        case 3:
            if(Player->CorrectLetter(input[0]) && Player->CorrectNumber(&input[1], 2))
                index = input[0] - 97 + 90; //this case input is #10 where # is any correct letter, so we can use 90
            break;
        default:
            if(input == "help")
                PlayingGuide();
            if(input == "switch")
            {
                Player->ChangeView();
                Bot->ChangeView();
            }
            if(input == "showme")
                Bot->Display();
            if(input == "ShipVector")
                {for(auto el : Bot->ShipVector()) cout << el << endl; getchar();}
            return;
    }
    if(index == -1)
        return;
    if(find(processed.begin(), processed.end(), index) == processed.end())
    {
        processed.push_back(index);
        switch(Bot->Tile(index).Type)
        {
            case Field::TileType::UNKNOWN:
                Bot->Tile(index).Type = Field::TileType::EMPTY;
                prev_event = "Bot is done after you missed the shot on ";
                PlayerTurn = false;
                break;
            case Field::TileType::SHIP:
                Bot->Tile(index).Type = Field::TileType::SHOT;
                (Bot->Tile(index).PlacedShip)->Alive -= 1;
                if((Bot->Tile(index).PlacedShip)->Alive == 0)
                    prev_event = "You destroyed the ship, shooting its final part ";
                else
                    prev_event = "You shot the ship's part on ";
                break;
        }
        prev_event.push_back(index % 10 + 97);
        prev_event += to_string(index / 10 + 1) + ", now it's your turn again";
    }
    else
        prev_event = "You can't shoot the same tile twice, make another choice";
}

void Game::PlayingGuide()
{
    system("CLS");
    cout << "The format of input is: '[tile_to_shoot]'" << endl << "Example: a1" << endl;
    cout << "Use 'switch' to get rid of borders" << endl << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
