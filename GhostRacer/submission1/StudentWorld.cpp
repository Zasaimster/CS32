#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    ghostRacerStatus = 0;
    level = 0;
}

int StudentWorld::init()
{
    ghostRacer = new GhostRacer(this);


    int countOfYellowBorders = VIEW_HEIGHT / SPRITE_HEIGHT;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;

    for (int i = 0; i < countOfYellowBorders; i++) {
        listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, i * SPRITE_HEIGHT, ghostRacer));
        listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, i * SPRITE_HEIGHT, ghostRacer));
    }
    
    int countOfWhiteBorders = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
     
    for (int i = 0; i < countOfWhiteBorders; i++) {
        listOfActors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH / 3, i * (4 * SPRITE_HEIGHT), ghostRacer));
        listOfActors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH / 3, i * (4 * SPRITE_HEIGHT), ghostRacer));
    }

    yCoordOfLastWhiteBorder = (countOfWhiteBorders - 1) * (4 * SPRITE_HEIGHT);

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();

    list<Actor*>::iterator actor;

    if (ghostRacer->getIsAlive())
        ghostRacer->doSomething();
    
    for (actor = listOfActors.begin(); actor != listOfActors.end(); actor++) {
        if ((*actor)->getIsAlive()) {
            (*actor)->doSomething();

            if (ghostRacerStatus == 1) {
                //add bonus points
                return GWSTATUS_FINISHED_LEVEL;
            }
            if (ghostRacerStatus == 2)
                return GWSTATUS_PLAYER_DIED;
        }
    }

    for (actor = listOfActors.begin(); actor != listOfActors.end(); ) {
        //cout << "here1" << endl;
        if (!(*actor)->getIsAlive()) {
            Actor* toDelete = *actor;
            listOfActors.erase(actor++);
            delete toDelete;
        }
        else {
            actor++;
        }
    }

    //call logic to add new actors
    addNewBorders();
    addNewHumanPeds();
    addNewZombiePeds();
    addNewOilSlicks();

    //update display text
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addNewBorders() {
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int dy = new_border_y - yCoordOfLastWhiteBorder;

    yCoordOfLastWhiteBorder += -4 - ghostRacer->getVertSpeed();

    if (dy >= SPRITE_HEIGHT) {
        //cout << "need new yellow" << endl;
        listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, 
            ROAD_CENTER - ROAD_WIDTH / 2, new_border_y, ghostRacer));
        listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, 
            ROAD_CENTER + ROAD_WIDTH / 2, new_border_y, ghostRacer));
    }

    if (dy >= 4 * SPRITE_HEIGHT) {
        cout << "need new white" << endl;
        listOfActors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 -
            ROAD_WIDTH / 3, new_border_y, ghostRacer));
        listOfActors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 +
            ROAD_WIDTH / 3, new_border_y, ghostRacer));
        yCoordOfLastWhiteBorder = new_border_y;
    }

}

void StudentWorld::addNewHumanPeds() {
    int chance = max(200 - level * 10, 30);
    int rand = randInt(0, chance - 1);

    if (rand == 0) {
        int randX = randInt(0, VIEW_WIDTH - 1);
        listOfActors.push_back(new HumanPed(randX));
    }
}

void StudentWorld::addNewZombiePeds() {
    int chance = max(100 - level * 10, 20);
    int rand = randInt(0, chance - 1);

    if (rand == 0) {
        int randX = randInt(0, VIEW_WIDTH - 1);
        listOfActors.push_back(new ZombiePed(randX));
    }
}

void StudentWorld::addNewZombieCabs() {
    int chance = max(100 - level * 10, 20);
    int rand = randInt(0, chance - 1);

    if (rand == 0) {
        //IMPLEMENT ZOMBIE CAB THING
        //listOfActors.push_back(new OilSlick(randX));
    }
}

void StudentWorld::addNewOilSlicks() {
    int chance = max(150 - level * 10, 40);
    int rand = randInt(0, chance - 1);

    if (rand == 0) {
        int randX = randInt(0, VIEW_WIDTH - 1);
        //listOfActors.push_back(new OilSlick(randX));
    }
}

void StudentWorld::addNewHolyWaterRefills() {
    int chance = 100 + 10 * level;
    int rand = randInt(0, chance - 1);

    if (rand == 0) {
        int randX = randInt(0, VIEW_WIDTH - 1);
        //listOfActors.push_back(new HolyWater(randX));
    }
}

void StudentWorld::addNewLostSoulGoodies() {
    int chance = 100;
    int rand = randInt(0, chance - 1);

    if (rand == 0) {
        int randX = randInt(0, VIEW_WIDTH - 1);
        //listOfActors.push_back(new LostSoulGoodie(randX));
    }
}

void StudentWorld::cleanUp()
{
    delete ghostRacer;

    list<Actor*>::iterator actor;
    for (actor = listOfActors.begin(); actor != listOfActors.end(); actor++) {
        delete *actor;
    }
}

StudentWorld::~StudentWorld() {
    cleanUp();
}