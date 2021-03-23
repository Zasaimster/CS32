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
	level = 1;
	m_bonus = 5000;
	soulsToSave = 0;
	yCoordOfLastWhiteBorder = 0;
}

int StudentWorld::init()
{
	ghostRacerStatus = 0;
	m_bonus = 5000;
	ghostRacer = new GhostRacer(this);
	GameWorld::setGameStatText(getStringForStats());
	soulsToSave = GameWorld::getLevel() * 2 + 5;
	//soulsToSave = 2;
	int countOfYellowBorders = VIEW_HEIGHT / SPRITE_HEIGHT;
	int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
	int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;

	//introduce all the required borders at the statt
	for (int i = 0; i < countOfYellowBorders; i++) {
		listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE,
			i * SPRITE_HEIGHT, ghostRacer, this));
		listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE,
			i * SPRITE_HEIGHT, ghostRacer, this));
	}

	int countOfWhiteBorders = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);

	for (int i = 0; i < countOfWhiteBorders; i++) {
		listOfActors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH / 3,
			i * (4 * SPRITE_HEIGHT), ghostRacer, this));
		listOfActors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH / 3,
			i * (4 * SPRITE_HEIGHT), ghostRacer, this));
	}

	yCoordOfLastWhiteBorder = (countOfWhiteBorders - 1) * (4 * SPRITE_HEIGHT);

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	if (ghostRacer->getSoulsSaved() >= soulsToSave) {
		GameWorld::increaseScore(m_bonus);
		level++;
		GameWorld::playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	list<Actor*>::iterator actor;

	if (ghostRacer->getIsAlive()) {
		ghostRacer->doSomething();
	}

	for (actor = listOfActors.begin(); actor != listOfActors.end(); actor++) {
		if ((*actor)->getIsAlive()) {
			(*actor)->doSomething();

			if (ghostRacerStatus == 2) {
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
		}
	}

	//go through all dead actors and erase them from memory
	for (actor = listOfActors.begin(); actor != listOfActors.end(); ) {
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
	addNewZombieCabs();
	addNewHolyWaterRefills();
	addNewLostSoulGoodies();

	m_bonus--;
	GameWorld::setGameStatText(getStringForStats());
	//update display text
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::ghostRacerWasKilled() {
	ghostRacerStatus = 2;
}

string StudentWorld::getStringForStats() {
	//use to_string to convert from numbers to a string to add to the new string 
	string score = to_string(GameWorld::getScore());
	string level = to_string(GameWorld::getLevel());
	string souls = to_string(soulsToSave - ghostRacer->getSoulsSaved());
	string lives = to_string(GameWorld::getLives());
	string health = to_string(ghostRacer->getHealth());
	string sprays = to_string(ghostRacer->getCountOfHolyWater());
	string bonus = to_string(m_bonus);

	//add all the values together in a string and return it 

	string toReturn = "Score: " + score + "  Lvl: " + level + "  Souls2Save: " + souls + "  Lives: " + lives
		+ "  Health: " + health + "  Sprays: " + sprays + "  Bonus: " + bonus;

	return toReturn;

}

void StudentWorld::addNewBorders() {
	int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
	double dy = new_border_y - yCoordOfLastWhiteBorder;

	yCoordOfLastWhiteBorder += -4 - ghostRacer->getVertSpeed();

	if (dy >= SPRITE_HEIGHT) {
		listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE,
			ROAD_CENTER - ROAD_WIDTH / 2, new_border_y, ghostRacer, this));
		listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE,
			ROAD_CENTER + ROAD_WIDTH / 2, new_border_y, ghostRacer, this));
	}

	if (dy >= 4 * SPRITE_HEIGHT) {
		listOfActors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 -
			ROAD_WIDTH / 3, new_border_y, ghostRacer, this));
		listOfActors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 +
			ROAD_WIDTH / 3, new_border_y, ghostRacer, this));
		yCoordOfLastWhiteBorder = new_border_y;
	}
}

void StudentWorld::addNewHumanPeds() {
	int chance = max(200 - level * 10, 30);
	int rand = randInt(0, chance - 1);

	if (rand == 0) {
		int randX = randInt(0, VIEW_WIDTH - 1);
		listOfActors.push_back(new HumanPed(randX, ghostRacer, this));
	}
}

void StudentWorld::addNewZombiePeds() {
	int chance = max(100 - level * 10, 20);
	int rand = randInt(0, chance - 1);

	if (rand == 0) {
		int randX = randInt(0, VIEW_WIDTH - 1);
		listOfActors.push_back(new ZombiePed(randX, ghostRacer, this));
	}
}

void StudentWorld::addNewZombieCabs() {
	int chance = max(100 - level * 10, 20);
	int rand = randInt(0, chance - 1);

	if (rand == 0) {
		int currLane = randInt(0, 2); //0 = left lane, 1 = middle lane, 2 = right lane
		int leftMostEdge = ROAD_CENTER - ROAD_WIDTH / 2;
		int rightMostEdge = ROAD_CENTER + ROAD_WIDTH / 2;
		double startX, startY, startSpeed;
		list<Actor*>::iterator actor;
		bool actorCreated = false;

		//iterate once per each lane. Break out when values are set
		for (int i = 0; i <= 2; i++) {
			if (actorCreated)
				break;
			
			int leftEdge = leftMostEdge + (ROAD_WIDTH / 3) * currLane;
			int rightEdge = rightMostEdge - (ROAD_WIDTH / 3) * (2 - currLane);
			//for efficieny, set two actor pointers and go through a for loop once to check for the Y coordinates
			Actor* closestCwActorToTheBottom = nullptr;
			Actor* closestCwActorToTheTop = nullptr;

			if (currLane == 0)
				startX = ROAD_CENTER - ROAD_WIDTH / 3;
			else
				startX = currLane == 1 ? ROAD_CENTER : ROAD_CENTER + ROAD_WIDTH / 3;

			for (actor = listOfActors.begin(); actor != listOfActors.end(); actor++) {
				Actor* a = *actor;
				if (a->isCollisionWorthy()) {
					if (a->getX() >= leftEdge && a->getX() < rightEdge) {
						if (closestCwActorToTheBottom == nullptr) {
							closestCwActorToTheBottom = a;
						}
						else if (closestCwActorToTheTop == nullptr) {
							closestCwActorToTheTop = a;
						}
						else {
							if (a->getY() < closestCwActorToTheBottom->getY()) {
								closestCwActorToTheBottom = a;
							}
							if (a->getY() > closestCwActorToTheTop->getY()) {
								closestCwActorToTheTop = a;
							}
						}
					}
				}
			}

			//only check the spec requirements if ghost racer is not in this lane 
			if (ghostRacer->getX() < leftEdge || ghostRacer->getX() >= rightEdge) {
				if (closestCwActorToTheBottom == nullptr ||
					(closestCwActorToTheBottom != nullptr && closestCwActorToTheBottom->getY() > VIEW_HEIGHT / 3)) {
					startY = SPRITE_HEIGHT / 2;
					startSpeed = ghostRacer->getVertSpeed() + randInt(2, 4);
					actorCreated = true;
					break;
				}
			}
			else if (closestCwActorToTheTop == nullptr ||
				(closestCwActorToTheTop != nullptr && closestCwActorToTheTop->getY() < VIEW_HEIGHT * 2 / 3)) {
				startY = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
				startSpeed = ghostRacer->getVertSpeed() - randInt(2, 4);
				actorCreated = true;
				break;
			}

			currLane = (currLane + 1) % 3;
		}

		if (actorCreated) {
			listOfActors.push_back(new ZombieCab(startX, startY, startSpeed, ghostRacer, this));
		}
	}
}

void StudentWorld::addNewOilSlicks() {
	int chance = max(150 - level * 10, 40);
	int rand = randInt(0, chance - 1);

	if (rand == 0) {
		int randX = randInt(0, VIEW_WIDTH - 1);
		listOfActors.push_back(new OilSlick(randX, VIEW_HEIGHT, randInt(2, 5), ghostRacer, this));
	}
}

void StudentWorld::addNewHolyWaterRefills() {
	int chance = 100 + 10 * level;
	int rand = randInt(0, chance - 1);

	if (rand == 0) {
		int randX = randInt(0, VIEW_WIDTH - 1);
		listOfActors.push_back(new HolyWater(randX, ghostRacer, this));
	}
}

void StudentWorld::addNewLostSoulGoodies() {
	int chance = 100;
	int rand = randInt(0, chance - 1);

	if (rand == 0) {
		int randX = randInt(0, VIEW_WIDTH - 1);
		listOfActors.push_back(new SoulGoodie(randX, ghostRacer, this));
	}
}

void StudentWorld::allocateNewSpray(int x, int y, int sd) {
	listOfActors.push_back(new HolyWaterSpray(x, y, sd, ghostRacer, this));
}

void StudentWorld::allocateNewHealingGoodie(int x, int y) {
	listOfActors.push_back(new HealingGoodie(x, y, ghostRacer, this));
}

void StudentWorld::allocateNewOilSlick(int x, int y) {
	listOfActors.push_back(new OilSlick(x, y, randInt(2, 5), ghostRacer, this));
}

void StudentWorld::checkForAndHandleSprayCollisions(Actor* spray) {
	list<Actor*>::iterator actor;
	for (actor = listOfActors.begin(); actor != listOfActors.end(); actor++) {
		Actor* a = *actor;
		if (a->isSprayable()) {
			if (spray->doesOverlap(a)) {
				a->handleSprayCollision();
				spray->setToNotAlive();
				return;
			}
		}
	}
}

bool StudentWorld::isThereACwActorInFront(int currLane, ZombieCab* zc) {
	list<Actor*>::iterator actor;
	double distance = VIEW_HEIGHT;
	int leftEdge, rightEdge;
	zc->calculateLaneEdges(leftEdge, rightEdge, currLane);
	//iterate through all actors 
	//check if they're collision worthy
		//if so, check the x coordinate values and y distance
	for (actor = listOfActors.begin(); actor != listOfActors.end(); actor++) {
		Actor* a = *actor;
		if (a != zc) {
			if (a->isCollisionWorthy()) {
				if (a->getX() >= leftEdge && a->getX() < rightEdge) {
					double tempDistance = a->getY() - zc->getY();
					if (tempDistance >= 0 && tempDistance < distance)
						distance = tempDistance;
				}
			}
		}
	}
	if (distance < 96) {
		zc->setVertSpeed(zc->getVertSpeed() - 0.5);
		return true;
	}
	else {
		return false;
	}
}


bool StudentWorld::isThereACwActorBehind(int currLane, ZombieCab* zc) {
	list<Actor*>::iterator actor;
	double distance = -1 * VIEW_HEIGHT;
	int leftEdge, rightEdge;
	zc->calculateLaneEdges(leftEdge, rightEdge, currLane);
	//iterate through all actors 
	//check if they're collision worthy
		//if so, check the x coordinate values and y distance
	for (actor = listOfActors.begin(); actor != listOfActors.end(); actor++) {
		Actor* a = *actor;
		if (a != zc) {
			if (a->isCollisionWorthy()) {
				if (a->getX() >= leftEdge && a->getX() < rightEdge) {
					double tempDistance = a->getY() - zc->getY();
					if (tempDistance <= 0 && tempDistance > distance)
						distance = tempDistance;
				}
			}
		}
	}
	if (abs(distance) < 96) {
		zc->setVertSpeed(zc->getVertSpeed() + 0.5);
		return true;
	}
	else {
		return false;
	}
}

void StudentWorld::cleanUp()
{
	delete ghostRacer;

	list<Actor*>::iterator actor;
	for (actor = listOfActors.begin(); actor != listOfActors.end();) {
		if (actor == listOfActors.end()) break;
		Actor* toDelete = *actor;
		actor = listOfActors.erase(actor);
		delete toDelete;
	}
}

StudentWorld::~StudentWorld() {
	cleanUp();
}