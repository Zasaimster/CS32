#define _USE_MATH_DEFINES

#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//Actor implementations


//GhostRider implementations
void GhostRacer::doSomething() {
	if (!Actor::getIsAlive())
		return;

	int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
	int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
	int xPos = GraphObject::getX();
	int yPos = GraphObject::getY();
	int theta = GraphObject::getDirection();

	if (xPos <= LEFT_EDGE) {
		if (theta > 90) {
			LivingActor::adjustHealth(-10);
			GraphObject::setDirection(82);
			m_gw->playSound(SOUND_VEHICLE_CRASH);
		}
	}
	else if (xPos >= RIGHT_EDGE) {
		if (theta < 90) {
			LivingActor::adjustHealth(-10);
			GraphObject::setDirection(98);
			m_gw->playSound(SOUND_VEHICLE_CRASH);
		}
	}
	else {
		int key;

		if (m_gw->getKey(key)) {
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

void GhostRacer::handlePressSpace() {
	//implement holy water stuff
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
	int dy = Actor::getVertSpeed();
	if (dy > -1) {
		Actor::setVertSpeed(dy - 1);
	}
}

//BorderLine implementations
void BorderLine::doSomething() {
	int dy = Actor::getVertSpeed() - m_gr->getVertSpeed();
	int dx = Actor::getHorizontalSpeed();
	int xPos = GraphObject::getX();
	int yPos = GraphObject::getY();
	int new_y = yPos + dy;
	int new_x = xPos + dx;
	GraphObject::moveTo(new_x, new_y);

	//check to see if it went off the max edge of the screen
	if (xPos > VIEW_WIDTH || yPos > VIEW_HEIGHT) {
		Actor::setToNotAlive();
		return;
	}

	//check to see if it went off the lower edge of the screen
	if (xPos < 0 || yPos < 0) {
		Actor::setToNotAlive();
		return;
	}
}

//Human ped implementations
void HumanPed::doSomething() {

}

//Zombie ped implementations
void ZombiePed::doSomething() {

}