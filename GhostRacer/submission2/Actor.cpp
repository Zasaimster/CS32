#define _USE_MATH_DEFINES

#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//Actor implementations

void Actor::moveActorRelativeToDriver(Actor* driver) {
	//assume driver is Ghost Racer for all cases this function is used in
	double dy = getVertSpeed() - driver->getVertSpeed();
	double dx = getHorizontalSpeed();
	double newX = GraphObject::getX() + dx;
	double newY = GraphObject::getY() + dy;
	GraphObject::moveTo(newX, newY);

}

bool Actor::checkIfActorIsOffScreenAndSetToNotAlive() {
	double x = GraphObject::getX();
	double y = GraphObject::getY();
	if (x > VIEW_WIDTH || y > VIEW_HEIGHT || x < 0 || y < 0) {
		Actor::setToNotAlive();
		return true;
	}
	else {
		return false;
	}
}

bool Actor::doesOverlap(Actor* a) const {
	double dx = abs(GraphObject::getX() - a->getX());
	double dy = abs(GraphObject::getY() - a->getY());
	double radiusSum = GraphObject::getRadius() + a->getRadius();

	if (dx < radiusSum * 0.25 && dy < radiusSum * 0.25)
		return true;
	else
		return false;
}
 

//GhostRider implementations
void GhostRacer::doSomething() {
	if (!Actor::getIsAlive())
		return;

	int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
	int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
	double xPos = GraphObject::getX();
	double yPos = GraphObject::getY();
	double theta = GraphObject::getDirection();

	if (xPos <= LEFT_EDGE) {
		if (theta > 90) {
			LivingActor::adjustHealth(-10);
			GraphObject::setDirection(82);
			m_sw->playSound(SOUND_VEHICLE_CRASH);

		}
	}
	else if (xPos >= RIGHT_EDGE) {
		if (theta < 90) {
			LivingActor::adjustHealth(-10);
			GraphObject::setDirection(98);
			m_sw->playSound(SOUND_VEHICLE_CRASH);
		}
	}
	else {
		int key;

		if (m_sw->getKey(key)) {
			switch (key)
			{
				case KEY_PRESS_SPACE:
					handlePressSpace();
					break;
				case KEY_PRESS_LEFT:
					handlePressLeft();
					break;
				case KEY_PRESS_RIGHT:
					handlePressRight();
					break;
				case KEY_PRESS_UP:
					handlePressUp();
					break;
				case KEY_PRESS_DOWN:
					handlePressDown();
					break;
				default:
					break;
			}
		}
	}

	moveGhostRacer();

}

void GhostRacer::spinDueToOilSlick() {
	int currentDirection = GraphObject::getDirection();
	int deltaDirection = randInt(5,20) * pow(-1, randInt(0, 1));
	int newDirection = currentDirection + deltaDirection;
	newDirection = min(newDirection, 120);
	newDirection = max(newDirection, 60);

	GraphObject::setDirection(newDirection);
}

void GhostRacer::handleDeath() {
	LivingActor::handleDeath();
	Actor::getWorld()->playSound(SOUND_PLAYER_DIE);
	Actor::getWorld()->ghostRacerWasKilled();

}

void GhostRacer::lostTheRound() {
	int totalHealth = LivingActor::getHealth();
	LivingActor::adjustHealth(-1 * totalHealth);
}

void GhostRacer::handlePressSpace() {
	if (countOfHolyWater > 0) {
		double toRad = 3.14 / 180;
		double newX = GraphObject::getX() + (cos(GraphObject::getDirection() * toRad) * SPRITE_HEIGHT);
		double newY = GraphObject::getY() + (sin(GraphObject::getDirection() * toRad) * SPRITE_HEIGHT);
		Actor::getWorld()->allocateNewSpray(newX, newY, GraphObject::getDirection());
		Actor::getWorld()->playSound(SOUND_PLAYER_SPRAY);
		countOfHolyWater--;
	}
}

void GhostRacer::handlePressLeft() {
	int theta = GraphObject::getDirection();
	if (theta < 114) {
		GraphObject::setDirection(theta + 8);
	}
}

void GhostRacer::handlePressRight() {
	int theta = GraphObject::getDirection();
	if (theta > 66) {
		GraphObject::setDirection(theta - 8);
	}
}

void GhostRacer::handlePressUp() {
	int dy = Actor::getVertSpeed();
	if (dy < 5) {
		Actor::setVertSpeed(dy + 1);
	}
}

void GhostRacer::moveGhostRacer() {
	double max_shift_per_tick = 4.0;
	int angle = GraphObject::getDirection();
	double theta = angle * M_PI / 180;
	double dx = cos(theta) * max_shift_per_tick;
	double xPos = GraphObject::getX();
	double yPos = GraphObject::getY();

	GraphObject::moveTo(xPos + dx, yPos);
}

void GhostRacer::handlePressDown() {
	double dy = Actor::getVertSpeed();
	if (dy > -1) {
		Actor::setVertSpeed(dy - 1);
	}
}

//BorderLine implementations
void BorderLine::doSomething() {
	Actor::moveActorRelativeToDriver(m_gr);
	if (Actor::checkIfActorIsOffScreenAndSetToNotAlive())
		return;
}

//pedestrian implementations
void Pedestrian::doSomething() {
	if (!Actor::getIsAlive()) {
		return;
	}

	if (Actor::doesOverlap(m_gr)) {//implement overlap
		overlapsGhostRacer(m_gr);
		return;
	}

	doSomethingSpecificAsPedestrian();

}

void Pedestrian::adjustMovementPlan() {
	double horizontalSpeedMagnitude = randInt(1, 3);
	int randDirection = randInt(1, 2) == 1 ? 1 : -1;
	double newSpeed = horizontalSpeedMagnitude * randDirection;
	Actor::setHorizontalSpeed(newSpeed);
	Pedestrian::setMovementPlanDistance(randInt(-4, 32));
	if (newSpeed < 0)
		GraphObject::setDirection(180);
	else
		GraphObject::setDirection(0);
}

//Human ped implementations
void HumanPed::doSomethingSpecificAsPedestrian() {
	Actor::moveActorRelativeToDriver(Pedestrian::getPointerToGhostRacer());
	if (Actor::checkIfActorIsOffScreenAndSetToNotAlive())
		return;

	//movement plan code is not called in the pedestrian class since the spec for human
	//peds and zombie peds was slightly different
	Pedestrian::decrementMovementPlanDistance();
	if (Pedestrian::getMovementPlanDistance() > 0) {
		return;
	}
	else {
		Pedestrian::adjustMovementPlan();
	}
}

void HumanPed::overlapsGhostRacer(GhostRacer* gr) {
	gr->lostTheRound();
}

void HumanPed::handleSprayCollision() {
	Actor::setHorizontalSpeed(Actor::getHorizontalSpeed() * -1);
	int d = GraphObject::getDirection();
	int newD = d == 0 ? 180 : 0;
	GraphObject::setDirection(newD);
	Actor::getWorld()->playSound(SOUND_PED_HURT);
}

//Zombie ped implementations
void ZombiePed::doSomethingSpecificAsPedestrian() {
	double xCoord = GraphObject::getX();
	double yCoord = GraphObject::getY();
	GhostRacer* gr = Pedestrian::getPointerToGhostRacer();
	double dx = xCoord - gr->getX();
	if (abs(dx) < 30 && yCoord > gr->getY()) { //code follows spec 
		GraphObject::setDirection(270);
		if (dx < 0)
			Actor::setHorizontalSpeed(1);
		else if (dx > 0)
			Actor::setHorizontalSpeed(-1);
		else
			Actor::setHorizontalSpeed(0);

		decrementTicksUntilGrunt();
		if (getTicksUntilGrunt() <= 0) {
			Actor::getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
			setNumberOfTicksUntilGrunt(20);
		}
	}

	Actor::moveActorRelativeToDriver(Pedestrian::getPointerToGhostRacer());
	if (Actor::checkIfActorIsOffScreenAndSetToNotAlive())
		return;

	if (Pedestrian::getMovementPlanDistance() > 0) {
		Pedestrian::decrementMovementPlanDistance();
		return;
	}
	else {
		adjustMovementPlan();
	}
}

void ZombiePed::overlapsGhostRacer(GhostRacer* gr) {
	gr->adjustHealth(-5);
	LivingActor::adjustHealth(-2);
}

void ZombiePed::handleSprayCollision() {
	LivingActor::adjustHealth(-1);
	Actor::getWorld()->playSound(SOUND_PED_HURT);
}

void ZombiePed::handleDeath() {
	LivingActor::handleDeath();
	Actor::getWorld()->playSound(SOUND_PED_DIE);
	if (!(Actor::doesOverlap(Pedestrian::getPointerToGhostRacer()))) {
		if (randInt(1, 1) == 1) {
			Actor::getWorld()->allocateNewHealingGoodie(GraphObject::getX(), GraphObject::getY());
		}
	}
}


//zombie cab implementations

void ZombieCab::doSomething() {
	if (!Actor::getIsAlive())
		return;


	if (Actor::doesOverlap(m_gr)) {
		if (!hasDamagedGhostRacer) {
			Actor::getWorld()->playSound(SOUND_VEHICLE_CRASH);
			m_gr->adjustHealth(-20);
			//check to see what gr needs to do when damaged
			double xPos = GraphObject::getX();
			double grXPos = m_gr->getX();
			if (xPos <= grXPos) {
				Actor::setHorizontalSpeed(-5);
				GraphObject::setDirection(120 + randInt(0, 19));
			}
			else {
				Actor::setHorizontalSpeed(5);
				GraphObject::setDirection(60 - randInt(0, 19));
			}
			hasDamagedGhostRacer = true;
		}
	}

	Actor::moveActorRelativeToDriver(m_gr);
	if (Actor::checkIfActorIsOffScreenAndSetToNotAlive())
		return;

	int currLane = determineLane();
	if (currLane != -1) {
		if (Actor::getVertSpeed() > m_gr->getVertSpeed()) {
			//both functions update speed within the function, so no need for extra code here
			 if (Actor::getWorld()->isThereACwActorInFront(currLane, this)) {
				return;
			}
		}
		else {
			if (Actor::getWorld()->isThereACwActorBehind(currLane, this)) {
				return;
			}
		}
	}

	movementPlanDistance--;
	if (movementPlanDistance > 0) {
		return;
	}
	else {
		movementPlanDistance = randInt(4, 32);
		setVertSpeed(Actor::getVertSpeed() + randInt(-2, 2));
	}

}

void ZombieCab::handleSprayCollision() {
	LivingActor::adjustHealth(-1);
	if (Actor::getIsAlive()) {
		Actor::getWorld()->playSound(SOUND_VEHICLE_HURT);
	}
}

void ZombieCab::handleDeath() {
	LivingActor::handleDeath();
	Actor::getWorld()->playSound(SOUND_VEHICLE_DIE);
	if (randInt(1, 5) == 1) {
		Actor::getWorld()->allocateNewOilSlick(GraphObject::getX(), GraphObject::getY());
	}
}

//using a lane, find out what the lane's edges are
void ZombieCab::calculateLaneEdges(int& leftEdge, int& rightEdge, const int currLane) const {
	leftEdge = (ROAD_CENTER - ROAD_WIDTH / 2) + (ROAD_WIDTH / 3) * currLane;
	rightEdge = (ROAD_CENTER + ROAD_WIDTH / 2) - (ROAD_WIDTH / 3) * (2 - currLane);
}

int ZombieCab::determineLane() {
	double xPos = GraphObject::getX();
	int leftMostEdge = ROAD_CENTER - ROAD_WIDTH / 2;
	int rightMostEdge = ROAD_CENTER + ROAD_WIDTH / 2;

	//i indicates the lane, so lanes 0,1,2
	for (int i = 0; i <= 2; i++) {
		int leftEdge = leftMostEdge + ROAD_WIDTH / 3 * i;
		int rightEdge = rightMostEdge - ROAD_WIDTH / 3 * (2 - i);
		if (xPos >= leftEdge && xPos < rightEdge)
			return i;
	}

	if (xPos < leftMostEdge || xPos > rightMostEdge) {
		return -1;
	}

}


//GhostRacer Activated Object implementations

void GhostRacerActivatedObject::doSomething() {
	if (!Actor::getIsAlive())
		return;

	Actor::moveActorRelativeToDriver(m_gr);
	if (Actor::checkIfActorIsOffScreenAndSetToNotAlive())
		return;

	if (doSomethingAsAnObject()) {
		Actor::getWorld()->playSound(getSound());
		Actor::getWorld()->increaseScore(getScoreIncrease());
	}
}


//oil slick implementations

bool OilSlick::doSomethingAsAnObject() {
	GhostRacer* gr = GhostRacerActivatedObject::getPointerToGhostRacer();
	if (Actor::doesOverlap(gr)) {
		gr->spinDueToOilSlick();
		return true;
	}
	return false;
}

//healing goodie implementations
bool HealingGoodie::doSomethingAsAnObject() {
	GhostRacer* gr = GhostRacerActivatedObject::getPointerToGhostRacer();
	if (Actor::doesOverlap(gr)) {
		if (gr->getHealth() <= 90)
			gr->adjustHealth(10);
		else
			gr->adjustHealth(100 - gr->getHealth());
			//add as much health is needed to get exactly 100 hp		
		Actor::setToNotAlive();
		return true;
	}
	return false;
}

void HealingGoodie::handleSprayCollision() {
	Actor::setToNotAlive();
}


//soul goodie implementations
bool SoulGoodie::doSomethingAsAnObject() {
	GhostRacer* gr = GhostRacerActivatedObject::getPointerToGhostRacer();
	if (Actor::doesOverlap(gr)) {
		gr->adjustSoulsSaved();
		Actor::setToNotAlive();
		return true;
	}
	else {
		GraphObject::setDirection(GraphObject::getDirection() - 10);
		return false;
	}
}


//holy water implementations

bool HolyWater::doSomethingAsAnObject() {
	GhostRacer* gr = GhostRacerActivatedObject::getPointerToGhostRacer();
	if (gr->doesOverlap(this)) {
		gr->increaseHolyWater(10);
		Actor::setToNotAlive();
		return true;
	}
	return false;
}

void HolyWater::handleSprayCollision() {
	Actor::setToNotAlive();
}


//holy water spray implementations

void HolyWaterSpray::doSomething() {
	if (!Actor::getIsAlive())
		return;
	
	//check to see if it overlaps with anything it can overlap with
	Actor::getWorld()->checkForAndHandleSprayCollisions(this);

	GraphObject::moveForward(SPRITE_HEIGHT);
	travelDistance -= SPRITE_HEIGHT;

	if (Actor::checkIfActorIsOffScreenAndSetToNotAlive())
		return;

	if (travelDistance <= 0) {
		Actor::setToNotAlive();
	}

	//implement other circumstances
}