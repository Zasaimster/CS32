#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, int dx, int dy, int startDirection, double size, int depth) :
		GraphObject(imageID, startX, startY, startDirection, size, depth)
	{
		isAlive = true;
		m_dx = dx;
		m_dy = dy;

	}
	void move();
	virtual void doSomething() = 0;
	
	void setToNotAlive() {
		isAlive = false;
	}

	bool getIsAlive() {
		return isAlive;
	}

	int getHorizontalSpeed() {
		return m_dx;
	}

	int getVertSpeed() {
		return m_dy;
	}

	void setHorizontalSpeed(int dx) {
		m_dx = dx;
	}

	void setVertSpeed(int dy) {
		m_dy = dy;
	}


private:
	bool isAlive;
	int m_dx;
	int m_dy;
};

class LivingActor : public Actor {
public:
	LivingActor(int imageID, int startX, int startY, int dx, int dy, int sd, double size, int depth, int health)
		: Actor(imageID, startX, startY, dx, dy, sd, size, depth)
	{
		m_health = health;
	}

	void adjustHealth(int deltaHealth) {
		m_health += deltaHealth;
	}

	int getHealth() {
		return m_health;
	}

private:
	int m_health;
};

class GhostRacer : public LivingActor {
public:
	GhostRacer(GameWorld* gw) 
		: LivingActor(IID_GHOST_RACER, 128, 32, 0, 0, 90, 4.0, 0, 100)
	{
		countOfHolyWater = 100;
		m_gw = gw;
	}
	virtual void doSomething();

private:
	void handlePressSpace();
	void handlePressLeft();
	void handlePressRight();
	void handlePressUp();
	void handlePressDown();
	void moveGhostRacer();

	int countOfHolyWater;
	GameWorld* m_gw;
};

class BorderLine : public Actor {
public:
	BorderLine(int imageID, int startX, int startY, Actor* gr)
		: Actor(imageID, startX, startY, 0, -4, 0, 2.0, 2)
	{
		m_gr = gr;
	}
	virtual void doSomething();

private:
	Actor* m_gr;
};


class Pedestrian : public LivingActor {
public:
	Pedestrian(int imageID, int startX, int size)
		: LivingActor(imageID, startX, VIEW_HEIGHT - 1, 0, -4, 0, size, 0, 2)
	{

	}

private:

};

class HumanPed : public Pedestrian {
public:
	HumanPed(int startX)
		: Pedestrian(IID_HUMAN_PED, startX, 2)
	{

	}
	virtual void doSomething();

private:

};

class ZombiePed : public Pedestrian {
public:
	ZombiePed(int startX)
		: Pedestrian(IID_ZOMBIE_PED, startX, 3)
	{

	}
	virtual void doSomething();

private:

};












#endif // ACTOR_H_