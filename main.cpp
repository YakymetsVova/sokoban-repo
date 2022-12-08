#include <iostream>

using namespace std;

struct Position {
    int x;
    int y;
};

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
};


void printSignature();

const char PLAYER = 'R';
const char BOX = 'x';
const char BOX_IN_PLACE = '@';
const char PLACE = 'O';
const char WALL = '#';

const char FIRST_MAP[100][200]
        {
                "########################",
                "######            ######",
                "####    R           ####",
                "###     ### #### xx  ###",
                "###            #     ###",
                "###     #      #     ###",
                "###     #    O #     ###",
                "###     ########     ###",
                "####                 ###",
                "###O                 ###",
                "#######         ########",
                "########################",
        };

const char SECOND_MAP[100][200]
        {
                "#######################",
                "########   ############",
                "######## x ############",
                "######## x ###O########",
                "########## ###O########",
                "##########    O########",
                "######### x #  ########",
                "#########R  ###########",
                "#######################"
        };

const char THIRD_MAP[100][200]
        {
                "########################",
                "##########          ####",
                "#########OR    #########",
                "########### x  #########",
                "###   ###O# x  #########",
                "##    ### # O  #########",
                "##    ###x  xxO#########",
                "##    ###   O  #########",
                "########################"
        };




void determinePositions(char MAP[100][200], Position *player, Position *package, Position *place,
                        int *boxCounter, int *placeCounter) {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 200; j++) {
            if (MAP[i][j] == PLAYER) {
                player->x = j;
                player->y = i;

                MAP[i][j] = ' ';
            } else if (MAP[i][j] == BOX || MAP[i][j] == BOX_IN_PLACE) {
                package[*boxCounter].x = j;
                package[*boxCounter].y = i;

                MAP[i][j] = ' ';
                (*boxCounter)++;
            } else if (MAP[i][j] == PLACE) {
                place[*placeCounter].x = j;
                place[*placeCounter].y = i;

                (*placeCounter)++;
            }
        }
    }
}

bool isInPlace(const int *boxCounter, Position *package, Position *place, int k) {
    for (int m = 0; m < *boxCounter; m++) {
        if ((package[k].x == place[m].x) && (package[k].y == place[m].y)) {
            return true;
        }
    }
    return false;
}

void printMap(char MAP[100][200], Position *player, Position *package, Position *place, int *boxCounter, int stepsLeft) {
    system("cls");
    cout << "\n\n\n Steps left: " << stepsLeft << "\n";

    for (int i = 0; i < 100; i++) {
        bool nextLine = false;
        cout << "\t\t\t";
        for (int j = 0; j < 200; j++) {
            char field = ' ';

            if ((player->y == i) && (player->x == j)) {
                cout << PLAYER;
                continue;
            }

            for (int k = 0; k < *boxCounter; k++) {
                if ((package[k].x == j) && (package[k].y == i)) {
                    field = isInPlace(boxCounter, package, place, k) ? BOX_IN_PLACE : BOX;
                    break;
                } else if ((place[k].x == j) && (place[k].y == i)) {
                    field = PLACE;
                }
            }

            if (field != ' ') {
                cout << field;
                continue;
            }

            if (MAP[i][j] != '\0') {
                cout << MAP[i][j];
                nextLine = true;
            }
        }
        if (nextLine) cout << endl;
    }

}


bool isWallNext(char MAP[100][200], Position *package, Position *new_player, Position *box,
                int boxCounter, int packageNumber) {
    if (MAP[new_player->y][new_player->x] == WALL) return true;

    if (packageNumber >= 0) {
        if (MAP[box->y][box->x] == WALL) return true;

        for (int i = 0; i < boxCounter; i++) {
            if ((package[i].x == (box->x)) && (package[i].y == (box->y))) return true;
        }
    }
    return false;
}

Direction getDirectionFromKey(char key) {
    Direction dir;
    switch (key) {
        case 'w':
            dir = UP;
            break;
        case 'a':
            dir = LEFT;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'q':
            exit(0);
        default:
            if ((int) key != 10) {
                cout << " Invalid input: " << key << endl;
                system("pause");
            }
            dir = NONE;
            break;
    }
    return dir;
}

bool isAllBoxesInPlace(const int* boxCounter, const int* onPlace) {
    for (int k = 0; k < *boxCounter; k++) {
        if (!onPlace[k]) {
            return false;
        }
    }
    return true;
}

void updateOnPlaceStats(const Position *box, const Position *place, const int *boxCounter, int *onPlace) {
    for (int k = 0; k < *boxCounter; k++) {
        for (int m = 0; m < *boxCounter; m++) {
            if ((box[k].x == place[m].x) && (box[k].y == place[m].y)) {
                onPlace[k] = 1;
            }
        }
    }
}

void makeMove(char MAP[100][200], Position *player, Position *boxes, Position *place,
              const int *boxCounter, int *movesCounter, bool *gameEnd) {
    char key = cin.get();

    Direction dir = getDirectionFromKey(key);

    Position newPlayer;
    Position newBox;
    newPlayer.x = player->x;
    newPlayer.y = player->y;
    newBox.x = boxes->x;
    newBox.y = boxes->y;

    if (dir == UP) newPlayer.y--;
    else if (dir == DOWN) newPlayer.y++;
    else if (dir == LEFT) newPlayer.x--;
    else if (dir == RIGHT) newPlayer.x++;

    int currentBox = -1;

    for (int i = 0; i < *boxCounter; i++) {
        if ((boxes[i].x == newPlayer.x) && (boxes[i].y == newPlayer.y)) {
            newBox.x = boxes[i].x;
            newBox.y = boxes[i].y;
            currentBox = i;
            break;
        }
    }

    if (currentBox >= 0) {
        if (dir == UP) newBox.y--;
        else if (dir == DOWN) newBox.y++;
        else if (dir == LEFT) newBox.x--;
        else if (dir == RIGHT) newBox.x++;
    }

    if (!isWallNext(MAP, boxes, &newPlayer, &newBox, *boxCounter, currentBox)) {
        player->x = newPlayer.x;
        player->y = newPlayer.y;

        if (currentBox >= 0) {
            boxes[currentBox].x = newBox.x;
            boxes[currentBox].y = newBox.y;
        }
        (*movesCounter)++;
    }

    int onPlace[10] = {};
    updateOnPlaceStats(boxes, place, boxCounter, onPlace);

    *gameEnd = isAllBoxesInPlace(boxCounter, onPlace);
}



void copyMap(const char FROM_MAP[100][200], char TO_MAP[100][200]) {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 200; j++) {
            TO_MAP[i][j] = FROM_MAP[i][j];
        }
    }
}


int main() {
    char name[40];
    char choice;
    bool gameStart = false;
    bool gameEnd = false;
    int movesCounter = 0;
    int movesAllowed = 100;
    Position player;
    player.x = 0;
    player.y = 0;

    Position package[10];
    int boxCounter = 0;

    Position place[10];
    int placeCounter = 0;

    char currentMap[100][200];

    printSignature();


    cout << endl;
    cout << " \n\t Enter your name: ";
    cin >> name;
    cout << "\n\t Hello " << name << "!" << endl;

    while (!gameStart) {
        cout << endl;
        cout << " \t Choose your map: \n" << endl;
        cout << " \t\t1. First Map " << endl;
        cout << " \t\t2. Second Map " << endl;
        cout << " \t\t3. Third Map " << endl;

        cout << "\n\t\t";
        cin >> choice;

        switch (choice) {
            case '1':
                copyMap(FIRST_MAP, currentMap);
                gameStart = true;
                break;
            case '2':
                copyMap(SECOND_MAP, currentMap);
                gameStart = true;
                break;
            case '3':
                copyMap(THIRD_MAP, currentMap);
                gameStart = true;
                break;
            case 'q':
                exit(0);
            default:
                system("cls");
                cout << "\n Invalid option. Choose once again.  " << endl;
        }
    }


    determinePositions(currentMap, &player, package, place, &boxCounter, &placeCounter);

    printMap(currentMap, &player, package, place, &boxCounter, movesAllowed - movesCounter);
    cin.get();
    while (!gameEnd && movesAllowed >= movesCounter) {
        makeMove(currentMap, &player, package, place, &boxCounter, &movesCounter, &gameEnd);
        cin.get();
        printMap(currentMap, &player, package, place, &boxCounter, movesAllowed - movesCounter);
    }

    cout << endl;

    if (!gameEnd && movesAllowed < movesCounter) {
        system("cls");
        cout << "\n\n \t\t\t You have lost, " << name << "! My condolences! \n" << endl;
        cout << endl;
    }

    if (gameEnd) {
        system("cls");
        cout << "\n\n \t\t\t You have won, " << name << "! Congratulations! \n" << endl;
        cout << " \t\t\t Number of moves:  " << movesCounter << endl;
        cout << endl;
    }


    system("pause");
    return 0;
}

void printSignature() {
    cout << R"(
   _____         _           _                                          _
  / ____|       | |         | |                                        | |
 | (___    ___  | | __ ___  | |__    __ _  _ __                        | |__   _   _
  \___ \  / _ \ | |/ // _ \ | '_ \  / _` || '_ \                       | '_ \ | | | |
  ____) || (_) ||   <| (_) || |_) || (_| || | | |                      | |_) || |_| |
 |_____/  \___/ |_|\_\\___/ |_.__/  \__,_||_| |_|                      |_.__/  \__, |
                                                                                __/ |
                                                                               |___/
  _____                     _         _  _                        _  __           _        _
 |  __ \                   (_)       (_)| |                      | |/ /          (_)      | |
 | |  | |  ___   _ __ ___   _  _ __   _ | | __ __ _              | ' /  ___  ____ _   ___ | | __ __ _
 | |  | | / _ \ | '_ ` _ \ | || '_ \ | || |/ // _` |             |  <  / _ \|_  /| | / __|| |/ // _` |
 | |__| || (_) || | | | | || || | | || ||   <| (_| |             | . \| (_) |/ / | || (__ |   <| (_| |
 |_____/  \___/ |_| |_| |_||_||_| |_||_||_|\_\\__,_|             |_|\_\\___//___||_| \___||_|\_\\__,_|


  __   ___  ____  ______  __  _____
 /_ | / _ \|___ \|____  |/_ || ____|
  | || (_) | __) |   / /  | || |__
  | | \__, ||__ <   / /   | ||___ \
  | |   / / ___) | / /    | | ___) |
  |_|  /_/ |____/ /_/     |_||____/


)" << endl;
}
