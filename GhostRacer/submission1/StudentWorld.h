#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <list>
#include "Actor.h"
#include <string>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld();

private:
    void addNewBorders();
    void addNewZombieCabs();
    void addNewOilSlicks();
    void addNewZombiePeds();
    void addNewHumanPeds();
    void addNewHolyWaterRefills();
    void addNewLostSoulGoodies();

    Actor* ghostRacer;
    list<Actor*> listOfActors;
    int level;
    int ghostRacerStatus; //0 = alive, 1 = completed the level, 2 = dead
    int yCoordOfLastWhiteBorder;
};

#endif // STUDENTWORLD_H_