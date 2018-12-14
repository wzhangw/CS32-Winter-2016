#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "Compiler.h"
using namespace std;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
// Derived classes for Actor Class:
//		Ant, Anthill, Poison, Pool of Water, Pheromone, Pebble, Grasshopper (with baby and adult as derived class)

// initial hit points for some actors
const int BABY_GRASSHOPPER_INIT_HP = 500;
const int ADULT_GRASSHOPPER_INIT_HP = 1600;
const int FOOD_INIT_HP = 6000;
const int ANT_INIT_HP = 1500;
const int ANTHILL_INIT_HP = 8999;
const int ANT_FOOD_CAPACITY = 1800;
const int MAX_PHEROMONE_HP = 768;
const int NOT_A_PHEROMONE = -1;
const int NO_HP = -10000;
const int ROTATE_CLOCKWISE = 0;
const int ROTATE_COUNTERCLOCKWISE = 1;

class StudentWorld;
class Insect;
class Compiler;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* caller, int iid, int startX, int startY, Direction dir = none, int depth = 0)
		: GraphObject(iid, startX, startY, dir, depth), m_world(caller), m_haveDoneSomethingThisTurn(false), m_alive(true)
	{}

	virtual ~Actor() {}

	virtual void doSomething() = 0; // do nothing

	StudentWorld* getWorld() { return m_world; }

	virtual bool blockInsect() { return false; }

	bool checkIfBlocked(int& destX, int& destY);

	bool doneSomethingOrNot() { return m_haveDoneSomethingThisTurn; }

	void setDoneSomething(bool value) { m_haveDoneSomethingThisTurn = value; }

	bool isAlive() const { return m_alive; }

	void setDead() { m_alive = false; }

	virtual int pickUpFood(int amount) { return 0; }

	virtual void dropFood(int amount) { return; }

	virtual void changeHPBy(int amount) { return; }

	virtual int getHP() { return NO_HP; }

	virtual bool isThatKindOfAnthill(int colonyNumber) { return false; }

	virtual bool thisIsMyEnemy(int colonyNumber) { return false; }
	
	virtual void getAttacked() { return; }

	virtual bool isMyPheromone(int colonyNumber) { return false; }

private:
	StudentWorld* m_world;
	bool m_haveDoneSomethingThisTurn;
	bool m_alive;
};

//========================================Pebble Class==============================================

class Pebble : public Actor
{
public:
	Pebble(StudentWorld* caller, int startX, int startY) 
		: Actor(caller, IID_ROCK, startX, startY, right, 1) {}
	virtual ~Pebble() {}
	virtual void doSomething() { return; } // This is necessary to prevent Pebble from becoming an abstract class
	virtual bool blockInsect() { return true; }
private:
};

//========================================Npc Class==============================================
/*Npc class is a derived class from Actor, and base class for PoolOfWater and Poison*/
class Npc : public Actor
{
public:
	Npc(StudentWorld* caller, int startX, int startY, int iid)
		: Actor(caller, iid, startX, startY, right, 2) {}
	virtual ~Npc() {}
	virtual void doSomething();
	virtual void specialActOnInsect(Insect* poor) = 0;
};

/*PoolOfWater class*/
class PoolOfWater : public Npc
{
public:
	PoolOfWater(StudentWorld* caller, int startX, int startY) 
		: Npc(caller, startX, startY, IID_WATER_POOL) {}
	virtual ~PoolOfWater() {}
	virtual void specialActOnInsect(Insect* poor);
};

/*Poison class*/

class Poison : public Npc
{
public:
	Poison(StudentWorld* caller, int startX, int startY)
		: Npc(caller, startX, startY, IID_POISON) {}
	virtual ~Poison() {}
	virtual void specialActOnInsect(Insect* poor);
};

//========================================EnergyHolder Class==============================================
/* Energy Holder class is a derived class from Actor, and base class for Insect, food and pheromone classes*/

class EnergyHolder : public Actor
{
public:
	EnergyHolder(StudentWorld* caller, int iid, int startX, int startY, Direction dir, int depth = 0)
		: Actor(caller, iid, startX, startY, dir, depth), m_HP(0)
	{}

	virtual ~EnergyHolder() {}

	virtual void changeHPBy(int dHP) 
	{ 
		m_HP += dHP; 
		if (m_HP <= 0)
			setDead();
	}

	virtual int getHP() const { return m_HP; }

	virtual void doSomething() = 0;
private:
	int m_HP;
};

/*Food class*/
class Food : public EnergyHolder
{
public:
	Food(StudentWorld* caller, int startX, int startY)
		: EnergyHolder(caller, IID_FOOD, startX, startY, right, 2)
	{
		changeHPBy(FOOD_INIT_HP);
	}

	virtual ~Food() {}

	virtual void doSomething() { return; }

	virtual int pickUpFood(int amount);

	virtual void dropFood(int amount);
private:
};

/*Anthill class*/
class Anthill : public EnergyHolder
{
public:
	Anthill(StudentWorld* caller, int colonyNumber, int startX, int startY, Compiler* compiler)
		: EnergyHolder(caller, IID_ANT_HILL, startX, startY, right, 2), m_compiler(compiler), m_colonyNumber(colonyNumber)
	{
		changeHPBy(ANTHILL_INIT_HP);
	}

	virtual ~Anthill() {}
	virtual void doSomething();
	virtual bool isThatKindOfAnthill(int colonyNumber) { return colonyNumber == m_colonyNumber; }
private:
	Compiler* m_compiler;
	int m_colonyNumber;
};

/*Pheromone class*/

class Pheromone : public EnergyHolder
{
public:
	Pheromone(StudentWorld* caller, int iid, int startX, int startY, int colonyNumber)
		: EnergyHolder(caller, iid, startX, startY, right, 2), m_colonyNumber(colonyNumber)
	{
		changeHPBy(256);
	}
	
	virtual ~Pheromone() {}

	virtual void doSomething() { changeHPBy(-1); }

	virtual bool isMyPheromone(int colonyNumber) { return m_colonyNumber == colonyNumber; }
private:
	int m_colonyNumber;
};

//========================================Insect Class==============================================

class Insect : public EnergyHolder
{
public:
	Insect(StudentWorld* caller, int iid, int startX, int startY)
		: EnergyHolder(caller, iid, startX, startY, static_cast<Direction>(randInt(up, left)), 1), // assume grasshoppers have depth = 1 
		  m_sleepTicks(0), m_distance(randInt(2,10)), m_haveBeenStunnedHere(false),
		  m_haveBeenPoisonedHere(false)
	{
	}

	virtual ~Insect() {}

	int getSleepTicks() const { return m_sleepTicks; }

	void changeSleepTicksBy(int dST) { m_sleepTicks += dST; }

	int getDistance() const { return m_distance; }

	void changeDistanceBy(int dD) { m_distance += dD; }

	bool returnStunnedState() { return m_haveBeenStunnedHere; }

	void setStunnedState(bool value) { m_haveBeenStunnedHere = value; }

	bool returnPoisonedState() { return m_haveBeenPoisonedHere; }

	void setPoisonedState(bool value) { m_haveBeenPoisonedHere = value; }

	void addFoodTo(int x, int y, int amount);

	void setSusceptibility(bool value1, bool value2) { m_susceptibleToPoison = value1; m_susceptibleToWater = value2; }

	bool getPoisonSusceptibility() { return m_susceptibleToPoison; }

	bool getWaterSusceptibility() { return m_susceptibleToWater; }

	void doInsectThings(bool& endTick);

	virtual void doDifferentInsectThings() = 0;

	virtual void doSomething();

	void updateNewDest(int& x, int& y);

	virtual bool thisIsMyEnemy(int colonyNumber) { return true; }

private:
	int m_sleepTicks;
	int m_distance;
	bool m_haveBeenStunnedHere;
	bool m_haveBeenPoisonedHere;
	bool m_susceptibleToPoison;
	bool m_susceptibleToWater;
};

/*Ant class*/
class Ant : public Insect
{
public:
	Ant(StudentWorld* caller, int iid, int startX, int startY, int colonyNumber, Compiler* compiler)
		: Insect(caller, iid, startX, startY), m_nFood(0), m_prevBitten(false), m_prevBlocked(false),
		  m_lastRandNumber(0), m_ic(0), m_colonyNumber(colonyNumber), m_compiler(compiler)
	{
		changeHPBy(ANT_INIT_HP);
		setSusceptibility(true, true);
	}

	virtual ~Ant() {}

	bool getBlockedState() { return m_prevBlocked; }

	void setBlockedState(bool value) { m_prevBlocked = value; }

	bool getBittenState() { return m_prevBitten; }

	void setBittenState(bool value) { m_prevBitten = value; }

	virtual void doDifferentInsectThings();

	virtual bool thisIsMyEnemy(int colonyNumber) { return m_colonyNumber != colonyNumber; }

	virtual void getAttacked();
private:
	int m_colonyNumber;
	Compiler* m_compiler;
	int m_nFood;
	bool m_prevBlocked;
	bool m_prevBitten;
	int m_lastRandNumber;
	int m_ic; // instruction counter value

	void moveForward();
	void eatFood();
	void dropFoodHere();
	void bite();
	void pickupFood();
	void emitPheromone();
	void faceRandomDirection();
	void rotateClockwise();
	void rotateCounterClockwise();
	void generateRandomNumber(const Compiler::Command& c);
	void executeIfCommand(const Compiler::Command& c, bool& notSuccess);
};

/*Grasshopper class*/
class GrassHopper : public Insect
{
public:
	GrassHopper(StudentWorld* caller, int iid, int startX, int startY)
		: Insect(caller, iid, startX, startY)
	{}
	virtual ~GrassHopper() {}
	virtual void doDifferentInsectThings();
	virtual void doDifferentGHThings(bool& success) = 0;
	void doGrassHopperThings();
};

/*BabyGrassHopper class*/
class BabyGrassHopper : public GrassHopper
{
public:
	BabyGrassHopper(StudentWorld* caller, int startX, int startY)
		: GrassHopper(caller, IID_BABY_GRASSHOPPER, startX, startY)
	{
		changeHPBy(BABY_GRASSHOPPER_INIT_HP);
		setSusceptibility(true, true);
	}
	virtual ~BabyGrassHopper() {}
	virtual void doDifferentGHThings(bool& success);
private:
	void growUp();
};

/*AdultGrassHopper class*/
class AdultGrassHopper : public GrassHopper
{
public:
	AdultGrassHopper(StudentWorld* caller, int startX, int startY)
		: GrassHopper(caller, IID_ADULT_GRASSHOPPER, startX, startY)
	{
		changeHPBy(ADULT_GRASSHOPPER_INIT_HP);
		setSusceptibility(false, false);
	}

	virtual ~AdultGrassHopper() {}

	virtual void doDifferentGHThings(bool& success);

	virtual void getAttacked();
};

#endif // ACTOR_H_
