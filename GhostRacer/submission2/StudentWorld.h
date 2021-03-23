#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <list>
#include "Actor.h"
#include <string>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class GhostRacer;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void ghostRacerWasKilled();
    string getStringForStats();
    void allocateNewSpray(int x, int y, int sd);
    void allocateNewHealingGoodie(int x, int y);
    void allocateNewOilSlick(int x, int y);
    void checkForAndHandleSprayCollisions(Actor* spray);
    bool isThereACwActorInFront(int currLane, ZombieCab* zc);
    bool isThereACwActorBehind(int currLane, ZombieCab* zc);
    ~StudentWorld();

private:
    void addNewBorders();
    void addNewZombieCabs();
    void addNewOilSlicks();
    void addNewZombiePeds();
    void addNewHumanPeds();
    void addNewHolyWaterRefills();
    void addNewLostSoulGoodies();

    GhostRacer* ghostRacer;
    list<Actor*> listOfActors;
    int level;
    int ghostRacerStatus; //0 = alive, 2 = dead
    double yCoordOfLastWhiteBorder;
    int m_bonus;
    int soulsToSave;
};

#endif // STUDENTWORLD_H_
