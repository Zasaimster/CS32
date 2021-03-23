#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(int imageID, double startX, double startY, double dx, double dy, int startDirection, double size,
		int depth, bool cw, StudentWorld* sw) :
		GraphObject(imageID, startX, startY, startDirection, size, depth)
	{
		isAlive = true;
		m_dx = dx;
		m_dy = dy;
		collisionWorthy = cw;
		m_sw = sw;

	}
	virtual void doSomething() = 0;

	StudentWorld* getWorld() {
		return m_sw;
	}
	
	void setToNotAlive() {
		isAlive = false;
	}

	bool getIsAlive() const {
		return isAlive;
	}

	double getHorizontalSpeed() const {
		return m_dx;
	}

	double getVertSpeed() const {
		return m_dy;
	}

	void setHorizontalSpeed(const double dx) {
		m_dx = dx;
	}

	void setVertSpeed(const double dy) {
		m_dy = dy;
	}

	bool isCollisionWorthy() const {
		return collisionWorthy;
	}
	
	virtual void handleSprayCollision() {
		return;
	}

	virtual bool isSprayable() const {
		return false;
	}

	void moveActorRelativeToDriver(Actor* driver);

	bool checkIfActorIsOffScreenAndSetToNotAlive();

	bool doesOverlap(Actor* a) const;

private:
	StudentWorld* m_sw;

	bool isAlive;
	double m_dx;
	double m_dy;
	bool collisionWorthy;
};

class LivingActor : public Actor {
public:
	LivingActor(int imageID, double startX, double startY, double dx, double dy, int sd, double size, int depth,
		int health, StudentWorld* sw)
		: Actor(imageID, startX, startY, dx, dy, sd, size, depth, true, sw)
	{
		m_health = health;
	}

	void adjustHealth(const int deltaHealth) {
		m_health += deltaHealth;
		if (m_health <= 0) {
			handleDeath();
		}
	}

	virtual void handleDeath() {
		Actor::setToNotAlive();
	}

	int getHealth() const {
		return m_health;
	}

private:
	int m_health;
};

class GhostRacer : public LivingActor {
public:
	GhostRacer(StudentWorld* sw)
		: LivingActor(IID_GHOST_RACER, 128, 32, 0, 0, 90, 4.0, 0, 100, sw)
	{
		m_sw = sw;
		countOfHolyWater = 10;
		soulsSaved = 0;
		
	}
	virtual void doSomething();
	void lostTheRound() ;
	void increaseHolyWater(const int num) {
		countOfHolyWater += num;
	}

	int getCountOfHolyWater() const {
		return countOfHolyWater;
	}
	void spinDueToOilSlick();
	virtual void handleDeath();
	int getSoulsSaved() const {
		return soulsSaved;
	}
	void adjustSoulsSaved(const int num = 1) {
		soulsSaved += num;
	}


private:
	void handlePressSpace();
	void handlePressLeft();
	void handlePressRight();
	void handlePressUp();
	void handlePressDown();
	void moveGhostRacer();

	StudentWorld* m_sw;
	int countOfHolyWater;
	int soulsSaved;
};

class BorderLine : public Actor {
public:
	BorderLine(int imageID, double startX, double startY, GhostRacer* gr, StudentWorld* sw)
		: Actor(imageID, startX, startY, 0, -4, 0, 2.0, 2, false, sw)
	{
		m_gr = gr;
	}
	virtual void doSomething();

private:
	GhostRacer* m_gr;
};


class Pedestrian : public LivingActor {
public:
	Pedestrian(int imageID, double startX, double size, GhostRacer* gr, StudentWorld* sw)
		: LivingActor(imageID, startX, VIEW_HEIGHT - 1, 0, -4, 0, size, 0, 2, sw)
	{
		m_gr = gr;
		movementPlanDistance = 0;
	}
	virtual void doSomething();
	int getMovementPlanDistance() const {
		return movementPlanDistance;
	}
	void decrementMovementPlanDistance() {
		movementPlanDistance--;
	}
	void setMovementPlanDistance(const int newDistance) {
		movementPlanDistance = newDistance;
	}

	void adjustMovementPlan();

	virtual bool isSprayable() const {
		return true;
	}

protected:
	GhostRacer* getPointerToGhostRacer() const {
		return m_gr;
	}

private:
	virtual void doSomethingSpecificAsPedestrian() = 0;
	virtual void overlapsGhostRacer(GhostRacer* gr) = 0;

	GhostRacer* m_gr;
	int movementPlanDistance;
};

class HumanPed : public Pedestrian {
public:
	HumanPed(double startX, GhostRacer* gr, StudentWorld* sw)
		: Pedestrian(IID_HUMAN_PED, startX, 2, gr, sw)
	{

	}

	virtual void handleSprayCollision();

private:
	virtual void doSomethingSpecificAsPedestrian();
	virtual void overlapsGhostRacer(GhostRacer* gr);
};

class ZombiePed : public Pedestrian {
public:
	ZombiePed(double startX, GhostRacer* gr, StudentWorld* sw)
		: Pedestrian(IID_ZOMBIE_PED, startX, 3, gr, sw)
	{
		ticksUntilGrunt = 0;
	}

	void decrementTicksUntilGrunt() {
		ticksUntilGrunt--;
	}

	int getTicksUntilGrunt() const {
		return ticksUntilGrunt;
	}

	void setNumberOfTicksUntilGrunt(const int newValue) {
		ticksUntilGrunt = newValue;
	}

	virtual void handleSprayCollision();
	virtual void handleDeath();

private:
	virtual void doSomethingSpecificAsPedestrian();
	virtual void overlapsGhostRacer(GhostRacer* gr);
	
	int ticksUntilGrunt;
};

class ZombieCab : public LivingActor {
public:
	ZombieCab(double startX, double startY, double dy, GhostRacer* gr, StudentWorld* sw)
		: LivingActor(IID_ZOMBIE_CAB, startX, startY, 0, dy, 90, 4.0, 0, 3, sw)
	{
		m_gr = gr;
		hasDamagedGhostRacer = false;
		movementPlanDistance = 0;
	}


	void calculateLaneEdges(int& leftEdge, int& rightEdge, const int currLane) const;

	virtual void doSomething();
	virtual void handleSprayCollision();
	virtual void handleDeath();

	virtual bool isSprayable() const {
		return true;
	}

private:
	int determineLane();

	GhostRacer* m_gr;
	bool hasDamagedGhostRacer;
	int movementPlanDistance;
};


class GhostRacerActivatedObject : public Actor {
public:
	GhostRacerActivatedObject(int imageId, double startX, double startY, double dx, double dy, int sd, double size,
		int depth, bool cw, int scoreIncrease, int sound, GhostRacer* gr, StudentWorld* sw)
		: Actor(imageId, startX, startY, dx, dy, sd, size, depth, cw, sw)
	{
		m_gr = gr;
		m_sound = sound;
		m_scoreIncrease = scoreIncrease;
	}
	virtual void doSomething();
	virtual bool doSomethingAsAnObject() = 0;
	virtual int getScoreIncrease() const {
		return m_scoreIncrease;
	}
	virtual int getSound() const {
		return m_sound;
	}
	GhostRacer* getPointerToGhostRacer() const {
		return m_gr;
	}

private:
	GhostRacer* m_gr;
	int m_scoreIncrease;
	int m_sound;
};

class OilSlick : public GhostRacerActivatedObject{
public:
	OilSlick(double startX, double startY, double size, GhostRacer* gr, StudentWorld* sw)
		: GhostRacerActivatedObject(IID_OIL_SLICK, startX, startY, 0, -4, 0, size, 2, false, 0, 
			SOUND_OIL_SLICK, gr, sw)
	{
	}
	virtual bool doSomethingAsAnObject();

private:

};

class HealingGoodie : public GhostRacerActivatedObject{
public:
	HealingGoodie(double startX, double startY, GhostRacer* gr, StudentWorld* sw)
		: GhostRacerActivatedObject(IID_HEAL_GOODIE, startX, startY, 0, -4, 0, 1.0, 2, false,
			250, SOUND_GOT_GOODIE, gr, sw)
	{
	}
	virtual bool doSomethingAsAnObject();
	virtual void handleSprayCollision();
	virtual bool isSprayable() const {
		return true;
	}

private:

};

class SoulGoodie : public GhostRacerActivatedObject{
public:
	SoulGoodie(double startX, GhostRacer* gr, StudentWorld* sw)
		: GhostRacerActivatedObject(IID_SOUL_GOODIE, startX, VIEW_HEIGHT, 0, -4, 0, 4.0, 2, 
			false, 100, SOUND_GOT_SOUL, gr, sw)
	{

	}
	virtual bool doSomethingAsAnObject();


private:

};

class HolyWater : public GhostRacerActivatedObject{
public:
	HolyWater(double startX, GhostRacer* gr, StudentWorld* sw)
		: GhostRacerActivatedObject(IID_HOLY_WATER_GOODIE, startX, VIEW_HEIGHT, 0, -4, 90, 2.0, 2, false,
			50, SOUND_GOT_GOODIE, gr, sw)
	{

	}
	virtual bool doSomethingAsAnObject();
	virtual void handleSprayCollision();
	virtual bool isSprayable() const {
		return true;
	}

private:

};

class HolyWaterSpray : public Actor{
public:
	HolyWaterSpray(double startX, double startY, int sd, GhostRacer* gr, StudentWorld* sw)
		: Actor(IID_HOLY_WATER_PROJECTILE, startX, startY, 0, 0, sd, 1.0, 1, false, sw)
	{
		travelDistance = 160;
	}
	virtual void doSomething();


private:
	int travelDistance;
};



#endif // ACTOR_H_