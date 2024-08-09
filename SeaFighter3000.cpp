#include "Lib.h"
#include "SeaFighter3000.h"
#include "Field.h"
#include "Ship.h"

#define MISSED true
#define SHOTSHIP false

using namespace std;

int SeaFighter3000::ShotCondition = FIND_SHIP;
int SeaFighter3000::Difficulty = 1;
vector<int> SeaFighter3000::ToShoot;

void SeaFighter3000::SetRandomly(Field& field)
{
    int i = 0, j, dir, num1, num2, lengths[SHIPS_AMOUNT] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};
    char l1, l2;
    bool try_add;
    Ship s;
    srand(time(0));
    random_shuffle(lengths, lengths + SHIPS_AMOUNT);
    j = lengths[i] - 1;
    while(i < SHIPS_AMOUNT)
    {
        try_add = false;
        l1 = 97 + rand() % 10;
        num1 = rand() % 10 + 1;
        dir = rand() % 4;
        while(!try_add)
        {
            switch(dir)
            {
                case UP:
                    if(num1 <= j)
                        dir = DOWN;
                    else
                    {
                        l2 = l1;
                        num2 = num1 - j;
                        try_add = true;
                    }
                    break;
                case DOWN:
                    if(num1 >= 10 - j + 1)
                        dir = UP;
                    else
                    {
                        l2 = l1;
                        num2 = num1 + j;
                        try_add = true;
                    }
                    break;
                case LEFT:
                    if(l1 <= 'a' + j - 1)
                        dir = RIGHT;
                    else
                    {
                        num2 = num1;
                        l2 = l1 - j;
                        try_add = true;
                    }
                    break;
                case RIGHT:
                    if(l1 >= 'j' - j + 1)
                        dir = LEFT;
                    else
                    {
                        num2 = num1;
                        l2 = l1 + j;
                        try_add = true;
                    }
                    break;
            }
        }
        s.SetCoords(l1, l2, num1, num2);
        if(field.AddToField(s))
        {
            i++;
            j = lengths[i] - 1;
        }
    }
}

bool SeaFighter3000::Shoot(Field& player_field)
{
    static set<int> processed;
    static pair<bool, int>checked_dir[4] = {{false, -10}, {false, 10}, {false, 1}, {false, -1}};
    static vector<int>::iterator it = ToShoot.begin();
    static int direction = -1, try_shoot, offset = 0;
    int i, edge;
    bool need_to_switch = false;
    srand(time(0));
    switch(ShotCondition)
    {
        case FIND_SHIP:
            if(Difficulty == 2 && it != ToShoot.end())
            {
                while(it != ToShoot.end() && processed.find(*it) != processed.end())
                    it++;
                if(it != ToShoot.end())
                {
                    try_shoot = *it;
                    break;
                }
            }
            do
                try_shoot = rand() % 100;
            while(processed.find(try_shoot) != processed.end());
            //cout << "enter the attacking tile" << endl; cin >> try_shoot;
            //cout << "step 1 (scenario 1): Completely random tile: " << (char)(try_shoot % 10 + 97) << try_shoot / 10 + 1 << endl; getchar();
            break;
        case FIND_DIR:
            if(try_shoot >= 0 && try_shoot <= 9 || processed.find(try_shoot - 10) != processed.end())
                checked_dir[UP].first = true;
            if(try_shoot % 10 == 0 || processed.find(try_shoot - 1) != processed.end())
                checked_dir[LEFT].first = true;
            if(try_shoot >= 90 && try_shoot <= 99 || processed.find(try_shoot + 10) != processed.end())
                checked_dir[DOWN].first = true;
            if(try_shoot % 10 == 1 || processed.find(try_shoot + 1) != processed.end())
                checked_dir[RIGHT].first = true;
            do
                direction = rand() % 4;
            while(checked_dir[direction].first);
            //cout << "enter the direction 0up 1down 2right 3left" << endl; cin >> direction;
            checked_dir[direction].first = true;
            break;
        case TRY_FINISH:
            edge = Field::EdgeOfField(try_shoot);
            switch(edge)
            {
                case Field::EdgeTile::TOP:
                    if(direction == UP)
                        need_to_switch = true;
                    break;
                case Field::EdgeTile::LOW:
                    if(direction == DOWN)
                        need_to_switch = true;
                    break;
                case Field::EdgeTile::LEFT:
                    if(direction == LEFT)
                        need_to_switch = true;
                    break;
                case Field::EdgeTile::RIGHT:
                    if(direction == RIGHT)
                        need_to_switch = true;
                    break;
                default:
                    if(edge != Field::EdgeTile::NONE)
                        need_to_switch = true;
                    break;
            }
            if(!need_to_switch && processed.find(try_shoot + checked_dir[direction].second) != processed.end())
                need_to_switch = true;
            if(need_to_switch)
            {
                //cout << "Direction switched" << endl;
                try_shoot -= offset * checked_dir[direction].second;
                direction = Opposite(direction);
            }
            break;
    }
    if(ShotCondition != FIND_SHIP)
    {
        try_shoot += checked_dir[direction].second;
        //cout << "step 1 (scenario 2): Try find the ship's other parts: " << (char)(try_shoot % 10 + 97) << try_shoot / 10 + 1 << endl; getchar();
    }
    processed.insert(try_shoot);
    cout << "SeaFighter: I am shooting " << char(try_shoot % 10 + 97) << try_shoot / 10 + 1 << endl; getchar();
    switch(player_field.Tile(try_shoot).Type)
    {
        case Field::TileType::UNKNOWN:
            player_field.Tile(try_shoot).Type = Field::TileType::EMPTY;
            switch(ShotCondition)
            {
                case FIND_DIR:
                    try_shoot -= checked_dir[direction].second;
                    break;
                case TRY_FINISH:
                    try_shoot -= checked_dir[direction].second * (offset + 1);
                    direction = Opposite(direction);
                    break;
            }
            //cout << "Missed the shot (ShotCon = " << ShotCondition << ")" << endl; getchar();
            //return SHOTSHIP;
            return MISSED;
        case Field::TileType::SHIP:
            player_field.Tile(try_shoot).Type = Field::TileType::SHOT;
            (player_field.Tile(try_shoot).PlacedShip)->Alive -= 1;
            //cout << "Processing ship " << *(player_field.Tile(try_shoot).PlacedShip) << endl;
            switch(ShotCondition)
            {
                case FIND_SHIP:
                    offset = 0;
                    ShotCondition = FIND_DIR;
                    break;
                default:
                    if(ShotCondition == FIND_DIR)
                        ShotCondition = TRY_FINISH;
                    offset++;
                    break;
            }
            if((player_field.Tile(try_shoot).PlacedShip)->Alive == 0)
            {
                ShotCondition = FIND_SHIP;
                const set<int>& nearby_area = (player_field.Tile(try_shoot).PlacedShip)->NearbyArea();
                for(auto el : nearby_area)
                    processed.insert(el);
                for(i = 0; i < 4; i++)
                    checked_dir[i].first = false;
            }
            //cout << "Got the shot (ShotCon = " << ShotCondition << "), offset = " << offset << endl; getchar();
            return SHOTSHIP;
    }
}

int SeaFighter3000::Opposite(int direction)
{
    switch(direction)
    {
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case RIGHT:
            return LEFT;
        case LEFT:
            return RIGHT;
    }
}

void SeaFighter3000::Init(int difficulty)
{
    if(difficulty < 1 || difficulty > 2)
    {
        cout << "SeaFighter3000::SetDifficulty: invalid difficulty parameter, Difficulty was set to 1 by default" << endl;
        getchar();
        Difficulty = 1;
        return;
    }
    Difficulty = difficulty;
    if(Difficulty == 2)
    {
        srand(time(0));
        int i, j, k, val1 = 0, val2 = 0;
        bool priority = true;
        vector<int> temp;
        vector<pair<int, bool>> starts;
        if(rand() % 2)
            val1 = 1;
        else
            val2 = 1;
        starts.insert(starts.end(), {{val1, false}, {10 + val2,  false}, {20 + val1, true}, {30 + val2, true}, {40 + val1, false}, {50 + val2, false}, {60 + val1, true}, {70 + val2, true}, {80 + val1, false}, {90 + val2, false}});
        for(i = 0; i < 10; i++) //for each line
        {
            priority = starts[i].second; //if line starts with higher priority tile or not
            for(j = starts[i].first; j < starts[i].first + 9; j += 2)
            {
                if(priority)
                {
                    priority = false;
                    ToShoot.push_back(j);
                }
                else
                {
                    priority = true;
                    temp.push_back(j);
                }
            }
        }
        random_shuffle(ToShoot.begin(), ToShoot.end());
        random_shuffle(temp.begin(), temp.end());
        ToShoot.insert(ToShoot.end(), temp.begin(), temp.end());
    }
}



/*void SeaFighter3000::Init(int difficulty)
{
    if(difficulty < 1 || difficulty > 2)
    {
        cout << "SeaFighter3000::SetDifficulty: invalid difficulty parameter, Difficulty was set to 1 by default" << endl;
        getchar();
        Difficulty = 1;
        return;
    }
    Difficulty = difficulty;
    if(Difficulty == 2)
    {
        int i, j, k, n = 0;
        pair<int, int> priority1[4] = {{30, 4}, {70, 8}, {92, 8}, {96, 4}};
        pair<int, int> priority2[5] = {{10, 2}, {50, 6}, {90, 10}, {94, 6}, {98, 2}};
        for(i = 0; i < 4; i++)
        {
            k = priority1[n].first;
            for(j = 0; j < priority1[n].second; j++)
                ToShoot.push_back(k - j * 10 + j);
            n++;
        }
        n = 0;
        random_shuffle(ToShoot.begin(), ToShoot.end());
        vector<int> temp;
        for(i = 0; i < 5; i++)
        {
            k = priority2[n].first;
            for(j = 0; j < priority2[n].second; j++)
                temp.push_back(k - j * 10 + j);
            n++;
        }
        random_shuffle(temp.begin(), temp.end());
        ToShoot.insert(ToShoot.end(), temp.begin(), temp.end());
    }
}*/

/*bool SeaFighter3000::Shoot(Field& player_field)
{
    static set<int> processed; //got to change everywhere use unique vector (for example Ship::NearbyArea)
    static bool try_finish = false;
    static pair<bool, int>checked_dir[4] = {{false, -10}, {false, 10}, {false, 1}, {false, -1}};
    static int direction = -1, try_shoot, offset = 0;
    int i, edge;
    bool need_to_switch = false;
    if(!try_finish){
        do
            try_shoot = rand() % 100;
        while(processed.find(try_shoot) != processed.end());
        cout << "enter the attacking tile" << endl; cin >> try_shoot;
        cout << "step 1 (scenario 1): Completely random tile: " << (char)(try_shoot % 10 + 97) << try_shoot / 10 + 1 << endl; getchar();}
    else
    {
        if(direction == -1)
        {
            if(try_shoot >= 0 && try_shoot <= 9 || processed.find(try_shoot - 10) != processed.end())
                checked_dir[UP].first = true;
            if(try_shoot % 10 == 0 || processed.find(try_shoot - 1) != processed.end())
                checked_dir[LEFT].first = true;
            if(try_shoot >= 90 && try_shoot <= 99 || processed.find(try_shoot + 10) != processed.end())
                checked_dir[DOWN].first = true;
            if(try_shoot % 10 == 1 || processed.find(try_shoot + 1) != processed.end())
                checked_dir[RIGHT].first = true;
            do
                direction = rand() % 4;
            while(checked_dir[direction].first);
            checked_dir[direction].first = true;
            cout << "enter the direction 0up 1down 2right 3left" << endl; cin >> direction;
        }
        else
        {
            edge = Field::EdgeOfField(try_shoot);
            switch(edge)
            {
                case Field::EdgeTile::TOP:
                    if(direction == UP)
                        need_to_switch = true;
                    break;
                case Field::EdgeTile::LOW:
                    if(direction == DOWN)
                        need_to_switch = true;
                    break;
                case Field::EdgeTile::LEFT:
                    if(direction == LEFT)
                        need_to_switch = true;
                    break;
                case Field::EdgeTile::RIGHT:
                    if(direction == RIGHT)
                        need_to_switch = true;
                    break;
                default:
                    if(edge != Field::EdgeTile::NONE)
                        need_to_switch = true;
                    break;
            }
            if(!need_to_switch && processed.find(try_shoot + checked_dir[direction].second) != processed.end())
                need_to_switch = true;
            if(need_to_switch)
            {
                cout << "Direction switched" << endl;
                try_shoot -= offset * checked_dir[direction].second;
                direction = Opposite(direction);
            }
        }
        try_shoot += checked_dir[direction].second;
        cout << "step 1 (scenario 2): Try find the ship's other parts: " << (char)(try_shoot % 10 + 97) << try_shoot / 10 + 1 << endl; getchar();
    }
    processed.insert(try_shoot);

    //cout << "[" << processed.size() << "]"<< endl;for (auto e:processed)cout << e << endl; getchar();

    switch(player_field.Tile(try_shoot).Type)
    {
        case Field::TileType::UNKNOWN:
            player_field.Tile(try_shoot).Type = Field::TileType::EMPTY;
            if(try_finish)
            {
                if(offset == 0)
                {
                    try_shoot -= checked_dir[direction].second;
                    direction = -1;
                }
                else
                {
                    try_shoot -= checked_dir[direction].second * (offset + 1);
                    direction = Opposite(direction);
                }
            }
            cout << "Missed the shot (try_finish = " << try_finish << ")" << endl; getchar();
            return SHOTSHIP;//return MISSED;
        case Field::TileType::SHIP:
            player_field.Tile(try_shoot).Type = Field::TileType::SHOT;
            (player_field.Tile(try_shoot).PlacedShip)->Alive -= 1;
            cout << "Processing ship " << *(player_field.Tile(try_shoot).PlacedShip) << endl;
            if(!try_finish)
            {
                offset = 0;
                try_finish = true;
            }
            else
                offset++;
            if((player_field.Tile(try_shoot).PlacedShip)->Alive == 0)
            {
                try_finish = false;
                const set<int>& nearby_area = (player_field.Tile(try_shoot).PlacedShip)->NearbyArea();
                for(auto el : nearby_area)
                    processed.insert(el);
                direction = -1;
                for(i = 0; i < 4; i++)
                    checked_dir[i].first = false;
            }
            cout << "Got the shot (try_finish = " << try_finish << "), offset = " << offset << endl; getchar();
            return SHOTSHIP;
    }
}*/
