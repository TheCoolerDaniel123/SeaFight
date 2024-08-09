#pragma once
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

class AIField;
class PlayerField;

class Game
{
    public:
        Game();
        Game(bool player_turn, int difficulty);
        ~Game();

        bool IsRunning(){return Running;}

        void Run();
        void HandleInput();

        enum GameConditions
        {
            SETTING, PLAYING, ENDED
        };

        void PlayingGuide();
    private:
        PlayerField* Player;
        AIField* Bot;
        bool PlayerTurn;
        bool PlayerWins;
        int GameState;
        bool Running;
};

#endif // GAME_H_INCLUDED
