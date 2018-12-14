#include "StudentWorld.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir), m_ticks(0), m_tempWinner(NO_WINNER), m_nAnthills(0)
{
	for (int i = 0; i < 4; i++)
	{
		compilersForEntrant.push_back(new Compiler);
		m_nAnts[i] = INIT_NUMBER_OF_NEW_ANTS;
	}	
}

StudentWorld::~StudentWorld()
{
	cleanUp();
	// delete all compilers
	for (int i = 0; i < 4; i++)
		delete compilersForEntrant[i];
	fileNames.clear();
}

void StudentWorld::setEveryoneUnmoving()
{
	for (int x = 0; x < VIEW_WIDTH; x++)
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			if (!m_actors[y][x].empty())
			{
				for (list<Actor*>::iterator ite = m_actors[y][x].begin(); ite != m_actors[y][x].end(); ite++)
				{
					Actor* temp = *ite;
					temp->setDoneSomething(false);
				}
			}
		}
	}
}

int StudentWorld::init()
{
	// Load the current field details from the specified field data file
	string fieldFileName;
	fieldFileName = getFieldFilename();
	string error;
	if (m_field.loadField(fieldFileName, error) != Field::LoadResult::load_success)
		// file loading is not successful
	{
		setError(fieldFileName + " " + error);
		return GWSTATUS_LEVEL_ERROR;
	}

	// get and compile the ant files
	std::vector<string> fileNames = getFilenamesOfAntPrograms();
	string antFileError;
	m_nAnthills = fileNames.size();
	// if there are more than 4 ant files, only take the first 4.
	if (m_nAnthills > 4)
		m_nAnthills = 4;
	for (int i = 0; i < m_nAnthills; i++)
	{
		if (!compilersForEntrant[i]->compile(fileNames[i], antFileError))
		{
			setError(fileNames[i] + " " + antFileError);
			return GWSTATUS_LEVEL_ERROR;
		}
		else
			m_antNames[i] = compilersForEntrant[i]->getColonyName();
	}

	// convert contents of field to a virtual map
	for (int x = 0; x < VIEW_WIDTH; x++)
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			Field::FieldItem temp = m_field.getContentsOf(x, y);
			Actor* newActor = nullptr;
			switch (temp)
				//empty, anthill0, anthill1, anthill2, anthill3, food, grasshopper, water, rock, poison
			{
			case Field::anthill0:
				if (m_nAnthills > 0)
					m_actors[y][x].push_back(new Anthill(this, 0, x, y, compilersForEntrant[0]));
				break;
			case Field::anthill1:
				if (m_nAnthills > 1)
					m_actors[y][x].push_back(new Anthill(this, 1, x, y, compilersForEntrant[1]));
				break;
			case Field::anthill2:
				if (m_nAnthills > 2)
					m_actors[y][x].push_back(new Anthill(this, 2, x, y, compilersForEntrant[2]));
				break;
			case Field::anthill3:
				if (m_nAnthills > 3)
					m_actors[y][x].push_back(new Anthill(this, 3, x, y, compilersForEntrant[3]));
				break;
			case Field::rock:
				newActor = new Pebble(this, x, y);
				break;
			case Field::water:
				newActor = new PoolOfWater(this, x, y);
				break;
			case Field::grasshopper:
				newActor = new BabyGrassHopper(this, x, y);
				break;
			case Field::poison:
				newActor = new Poison(this, x, y);
				break;
			case Field::food:
				newActor = new Food(this, x, y);
				break;
			default:
				break;
			}
			if (newActor != nullptr)
				m_actors[y][x].push_back(newActor);
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	setEveryoneUnmoving();

	// give all actors a chance to doSomething()
	for (int x = 0; x < VIEW_WIDTH; x++)
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			if (!m_actors[y][x].empty())
			{
				for (list<Actor*>::iterator ite = m_actors[y][x].begin(); ite != m_actors[y][x].end(); )
				{
					Actor* temp = *ite;
					list<Actor*>::iterator next_ite = ++ite;
					if (!temp->doneSomethingOrNot()) // prevent multiple calls of doSomething() of the same object
						temp->doSomething();
					if (!temp->isAlive()) // remove dead objects
					{
						delete temp;
						m_actors[y][x].remove(temp);
					}
					ite = next_ite;
				}
			}
		}
	}

	// if tick count does not reach 2000, continue game
	if (tickCount() < MAX_NUMBER_OF_TICKS)
	{
		bool needToUpdate = true;
		// reset the winner
		if (m_tempWinner == NO_WINNER)
		{
			bool allEqual = true;
			for (int i = 0; i < m_nAnthills - 1; i++)
				if (m_nAnts[i] != m_nAnts[i + 1])
					allEqual = false;
			needToUpdate = !allEqual;
		}
		if (needToUpdate)
		{
			for (int i = 0; i < m_nAnthills; i++)
				updateWinner(i);
		}

		// update the text on the screen, and return CONTINUE_GAME status
		ostringstream output;
		output << "Ticks:" << setw(5) << MAX_NUMBER_OF_TICKS - m_ticks;
		if (m_nAnthills > 0)
			output << " - ";
		for (int i = 0; i < m_nAnthills; i++)
		{
			if (i == m_tempWinner)
			{
				if (m_nAnts[i] < 10)
					output << m_antNames[i] << "*: 0" << m_nAnts[i];
				else
					output << m_antNames[i] << "*: " << m_nAnts[i];
			}
			else
			{
				if (m_nAnts[i] < 10)
					output << m_antNames[i] << ": 0" << m_nAnts[i];
				else
					output << m_antNames[i] << ": " << m_nAnts[i];
			}
			output << " ants  ";
		}
		setGameStatText(output.str());
		increTicks();
		return GWSTATUS_CONTINUE_GAME;
	}
	else // game tick reaches 2000. end game here
	{
		if (m_tempWinner == NO_WINNER)
			return GWSTATUS_NO_WINNER;
		else
		{
			setWinner(compilersForEntrant[m_tempWinner]->getColonyName());
			return GWSTATUS_PLAYER_WON;
		}
	}
}

void StudentWorld::cleanUp()
{
	for (int x = 0; x < VIEW_WIDTH; x++)
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			// delete all objects at each position
			if (!m_actors[y][x].empty())
			{
				for (list<Actor*>::iterator ite = m_actors[y][x].begin(); ite != m_actors[y][x].end(); ite++)
				{
					Actor* temp = *ite;
					delete temp;
				}
				// clear the list on each position
				m_actors[y][x].clear();
			}
		}
	}
}

bool StudentWorld::thisIsAPebble(int& x, int& y)
{
	// assume the input position isvalid
	if (!m_actors[y][x].empty())
	{
		Actor* temp = nullptr;
		for (std::list<Actor*>::iterator ite = m_actors[y][x].begin(); ite != m_actors[y][x].end(); ite++)
		{
			temp = dynamic_cast<Pebble*>(*ite);
			if (temp != nullptr)
				return true;
		}
	}
	return false;
}

void StudentWorld::helpAnActorMove(Actor* caller, int x, int y, int& destX, int& destY)
{
	// assume the input positions are both valid
	m_actors[destY][destX].push_back(caller);
	m_actors[y][x].remove(caller);
}

void StudentWorld::actOnInsectsAt(Npc* caller, int x, int y)
{
	// assume the input position isvalid
	Insect* temp = nullptr;
	for (std::list<Actor*>::iterator ite = m_actors[y][x].begin(); ite != m_actors[y][x].end();ite++)
	{
		temp = dynamic_cast<Insect*>(*ite);
		if (temp != nullptr)
			caller->specialActOnInsect(temp);
	}
}

void StudentWorld::stunTheInsect(Insect* poor)
{
	if (!poor->returnStunnedState())
	{
		// assume the number of sleep ticks is 0
		poor->changeSleepTicksBy(2);
		poor->setStunnedState(true);
	}
}

int StudentWorld::consumeFoodAt(int x, int y, int amount)
{
	// assume the input position is valid
	// assume the amount is nonnegative
	Actor* temp = nullptr;
	if (thereIsFoodAt(x, y, temp))
	{
		if (!temp->isAlive())
		{
			delete temp;
			m_actors[y][x].remove(temp);
		}
		else
			return temp->pickUpFood(amount);
	}
	return 0;
}

void StudentWorld::addFoodAt(int x, int y, int amount)
{
	// assume the input position is valid
	// assume the amount is nonnegative
	std::list<Actor*> current = m_actors[y][x];
	Actor* temp = nullptr;

	// find existing food object, if any
	for (std::list<Actor*>::iterator ite = current.begin(); ite != current.end(); ite++)
	{
		temp = dynamic_cast<Food*>(*ite);
		if (temp != nullptr)
			break;
	}
	
	if (temp == nullptr)
		// needs to create a new food because there is no food object
	{
		temp = new Food(this, x, y);
		temp->dropFood(amount - FOOD_INIT_HP);
		m_actors[y][x].push_back(temp);
	}
	else
		// simply add food to existing food object
		temp->dropFood(amount);
}

void StudentWorld::addAdultGrassHopper(int x, int y)
{
	// assume the input position is valid
	m_actors[y][x].push_back(new AdultGrassHopper(this, x, y));
}

void StudentWorld::attackOtherInsectAt(Actor* caller, int x, int y, bool& success, int attackValue)
{
	success = false;
	// assume input position is valid. assume attackValue is positive
	if (attackValue <= 0)
		return;

	std::vector<Actor*> potentialPrey;
	std::list<Actor*> current = m_actors[y][x];
	Actor* temp = nullptr;

	// collect all insects (including the caller/attacker) at this position
	for (std::list<Actor*>::iterator ite = current.begin(); ite != current.end(); ite++)
	{
		temp = dynamic_cast<Insect*>(*ite);
		if (temp != nullptr)
			potentialPrey.push_back(temp);
	}

	// attack
	if (potentialPrey.size() == 1) // no prey
		success = false;
	else // there is some prey
	{
		int nPrey = potentialPrey.size();
		Actor* temp_prey = caller;
		Actor* attacker = caller;
		while (temp_prey == attacker) // attacker cannot attack itself
		{
			int lottery = randInt(0, nPrey - 1);
			temp_prey = potentialPrey[lottery];
		}
		temp_prey->changeHPBy(-attackValue);
		success = true;
	}
}

bool StudentWorld::availableJumpingSpace(int x, int y)
{
	// assume input position is valid
	for (double radius = 2; radius <= 10; radius += 1)
		// probably better start off from radius = 2 rather than 1
	{
		for (double theta = 0; theta < PI; theta += PI / 360)
		{
			int temp_x = static_cast<int>(x + radius * cos(theta));
			int temp_y = static_cast<int>(y + radius * sin(theta));
			// check whether the coordinates are out of range
			if (temp_x < 0 || temp_x > VIEW_WIDTH - 1 || temp_y < 0 || temp_y > VIEW_HEIGHT - 1)
				continue;
			// check if the valid dimensions contain pebble
			if (!thisIsAPebble(temp_x, temp_y))
				return true;
		}
	}
	return false;
}

void StudentWorld::addAnt(int x, int y, int colonyNumber)
{
	// assume input position is valid
	// assume valid colony number
	const int IID_LIST[4] = { IID_ANT_TYPE0, IID_ANT_TYPE1, IID_ANT_TYPE2, IID_ANT_TYPE3 };
	m_actors[y][x].push_back(new Ant(this, IID_LIST[colonyNumber], x, y, colonyNumber, compilersForEntrant[colonyNumber]));
	m_nAnts[colonyNumber]++;
}

void StudentWorld::addUpPheromone(int x, int y, int colonyNumber)
{
	// assume input position is valid
	// assume valid colony number
	const int IID_LIST[4] = { IID_PHEROMONE_TYPE0, IID_PHEROMONE_TYPE1, IID_PHEROMONE_TYPE2, IID_PHEROMONE_TYPE3 };
	for (std::list<Actor*>::iterator ite = m_actors[y][x].begin(); ite != m_actors[y][x].end(); ite++)
	{
		Actor* temp = *ite;
		if (temp->isMyPheromone(colonyNumber))
			// found my pheromone
		{
			if (temp->getHP() < 768 - 256)
				temp->changeHPBy(256);
			else
				temp->changeHPBy(768 - temp->getHP());
			return;
		}
	}
	// there is no pheromone, so need to create one
	m_actors[y][x].push_back(new Pheromone(this, IID_LIST[colonyNumber], x, y, colonyNumber));
}

bool StudentWorld::thereIsFoodAt(int x, int y, Actor*& ptr)
{
	// assume valid input position
	Actor* temp = nullptr;
	for (std::list<Actor*>::iterator ite = m_actors[y][x].begin(); ite != m_actors[y][x].end(); ite++)
	{
		temp = dynamic_cast<Food*>(*ite);
		if (temp != nullptr)
		{
			ptr = temp;
			return true;
		}
	}
	return false;
}

bool StudentWorld::thisIsMyAnthill(int x, int y, int colonyNumber)
{
	// assume valid input position
	// assume valid colony number
	for (std::list<Actor*>::iterator ite = m_actors[y][x].begin(); ite != m_actors[y][x].end(); ite++)
	{
		Actor* temp = *ite;
		if (temp->isThatKindOfAnthill(colonyNumber))
			return true;
	}
	return false;
}

bool StudentWorld::thisIsMyPheromone(int x, int y, int colonyNumber)
{
	// assume input position is valid
	// assume valid colony number
	for (std::list<Actor*>::iterator ite = m_actors[y][x].begin(); ite != m_actors[y][x].end(); ite++)
	{
		Actor* temp = *ite;
		if (temp->isMyPheromone(colonyNumber))
			return true;
	}
	return false;
}

bool StudentWorld::thereIsMyEnemy(int x, int y, int colonyNumber)
{
	// assume input position is valid
	// assume valid colony number
	for (std::list<Actor*>::iterator ite = m_actors[y][x].begin(); ite != m_actors[y][x].end(); ite++)
	{
		Actor* temp = *ite;
		if (temp->thisIsMyEnemy(colonyNumber))
			return true;
	}
	return false;
}

void StudentWorld::updateWinner(int potentialMax)
{
	if (m_tempWinner == NO_WINNER)
	{
		m_tempWinner = 0;
		for (int i = 0; i < m_nAnthills; i++)
		{
			if (m_nAnts[m_tempWinner] < m_nAnts[i])
				m_tempWinner = i;
		}
	}
	if (m_tempWinner == potentialMax)
		return;
	else
		if (m_nAnts[potentialMax] > m_nAnts[m_tempWinner])
			m_tempWinner = potentialMax;
}