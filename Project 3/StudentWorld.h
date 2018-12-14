#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Field.h"
#include "Actor.h"
#include <list>
#include "GraphObject.h"
#include <cmath>

class Actor;
class Insect;
class Npc;
class AdultGrassHopper;
class Compiler;

const int MAX_NUMBER_OF_TICKS = 2000;
const double PI = 3.14159265;
const int INIT_NUMBER_OF_NEW_ANTS = 0;
const int INIT_NUMBER_OF_ANTS = 5;
const int NO_WINNER = -1;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	virtual ~StudentWorld();

	virtual int init();
	
	virtual int move();

	virtual void cleanUp();

	int tickCount() const { return m_ticks; }

	void increTicks() { m_ticks++; }

	bool thisIsAPebble(int& x, int& y);

	void helpAnActorMove(Actor* caller, int x, int y, int& destX, int& destY);

	void setEveryoneUnmoving();

	void actOnInsectsAt(Npc* caller, int x, int y);

	void stunTheInsect(Insect* poor);

	int consumeFoodAt(int x, int y, int amount);

	void addFoodAt(int x, int y, int amount);

	void addAdultGrassHopper(int x, int y);

	void attackOtherInsectAt(Actor* caller, int x, int y, bool& success, int attackValue);

	bool availableJumpingSpace(int x, int y);

	void addAnt(int x, int y, int colonyNumber);

	void addUpPheromone(int x, int y, int colonyNumber);

	bool thereIsFoodAt(int x, int y, Actor*& ptr);

	bool thisIsMyAnthill(int x, int y, int colonyNumber);

	bool thisIsMyPheromone(int x, int y, int colonyNumber);

	bool thereIsMyEnemy(int x, int y, int colonyNumber);

	void updateWinner(int potentialMax);

private:
	Field m_field;
	std::list<Actor*> m_actors[VIEW_HEIGHT][VIEW_WIDTH];
	int m_ticks;
	std::vector<Compiler*> compilersForEntrant;
	std::vector<std::string> fileNames;
	int m_nAnts[4];
	std::string m_antNames[4];
	int m_tempWinner;
	int m_nAnthills;
};

#endif // STUDENTWORLD_H_
