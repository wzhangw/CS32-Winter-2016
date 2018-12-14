#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


//=============================================Pool of Water====================================
void Npc::doSomething()
{
	getWorld()->actOnInsectsAt(this, getX(), getY());
}

void PoolOfWater::specialActOnInsect(Insect* poor)
{
	if (!poor->returnStunnedState() && poor->getWaterSusceptibility())
	{
		poor->changeSleepTicksBy(2);
		poor->setStunnedState(true);
	}
}

void Poison::specialActOnInsect(Insect* poor)
{
	if (poor->getPoisonSusceptibility())
	{
		poor->changeHPBy(-150);
		poor->setPoisonedState(true);
	}
}

//=============================================Food====================================

int Food::pickUpFood(int amount)
{
	// assume amount is nonnegative
	if (amount < 0)
		return 0;

	if (amount < getHP())
	{
		changeHPBy(-amount);
		return amount;
	}
	else
	{
		int consumedAmount = getHP();
		changeHPBy(-consumedAmount);
		setDead();
		return consumedAmount;
	}
}

void Food::dropFood(int amount)
{
	// the amount is expected to be negative sometimes.
	changeHPBy(amount);
}

//=============================================Anthill====================================

void Anthill::doSomething()
{
	changeHPBy(-1);
	// return if anthill is already dead
	if (!isAlive())
		return;
	// try to consume as much as 10000 food
	int foodConsumed = getWorld()->consumeFoodAt(getX(), getY(), 10000);
	if (foodConsumed > 0)
	{
		changeHPBy(foodConsumed);
		return;
	}
	if (getHP() >= 2000)
	{
		getWorld()->addAnt(getX(), getY(), m_colonyNumber);
		changeHPBy(-1500);
	}
}

//=============================================Insect====================================

void Insect::addFoodTo(int x, int y, int amount) 
{
	// assume valid position.
	getWorld()->addFoodAt(x, y, amount);
}

void Insect::doSomething()
{
	bool endTick;
	doInsectThings(endTick);
	if (endTick)
		return;
	doDifferentInsectThings();
}

void Insect::doInsectThings(bool& endTick)
{
	changeHPBy(-1);

	// check whether doSomething is already called to prevent multiple calls
	if (doneSomethingOrNot())
	{
		endTick = true;
		return;
	}

	// check if the insect is already dead
	if (!isAlive())
	{
		addFoodTo(getX(), getY(), 100);
		setDoneSomething(true);
		endTick = true;
		return;
	}

	// check if the insect should sleep
	if (getSleepTicks() > 0)
	{
		changeSleepTicksBy(-1);
		setDoneSomething(true);
		endTick = true;
		return;
	}

	endTick = false;
}

void Insect::updateNewDest(int& x, int& y)
{
	// assume valid input position
	// assume the modified position is still valid
	switch (getDirection())
	{
	case Direction::up:
		y++;
		break;
	case Direction::down:
		y--;
		break;
	case Direction::left:
		x--;
		break;
	case Direction::right:
		x++;
		break;
	}
}

//=============================================Ant====================================

void Ant::doDifferentInsectThings()
// this function basically acts as an interpretor of commands from ant files
{
	Compiler::Command cmd;
	int nCommand = 0;
	while (true)
	{
		// fetch the command, and check fetching error
		if (!m_compiler->getCommand(m_ic, cmd))
		{
			setDead();
			return;
		}

		// interpret/execute the command
		bool incrementByOne = true;
		bool changeExternalState = true;
		switch (cmd.opcode)
		{
		case Compiler::Opcode::moveForward:
			moveForward(); // related
			nCommand++;
			break;
		case Compiler::Opcode::eatFood:
			eatFood();
			nCommand++;
			break;
		case Compiler::Opcode::dropFood:
			dropFoodHere();
			nCommand++;
			break;
		case Compiler::Opcode::bite:
			bite();
			nCommand++;
			break;
		case Compiler::Opcode::pickupFood:
			pickupFood();
			nCommand++;
			break;
		case Compiler::Opcode::emitPheromone:
			emitPheromone(); 
			nCommand++;
			break;
		case Compiler::Opcode::faceRandomDirection:
			faceRandomDirection();
			nCommand++;
			break;
		case Compiler::Opcode::rotateClockwise:
			rotateClockwise();
			nCommand++;
			break;
		case Compiler::Opcode::rotateCounterClockwise:
			rotateCounterClockwise();
			nCommand++;
			break;
		case Compiler::Opcode::generateRandomNumber:
			generateRandomNumber(cmd);
			nCommand++;
			changeExternalState = false;
			break;
		case Compiler::Opcode::goto_command:
			m_ic = stoi(cmd.operand1);
			nCommand++;
			incrementByOne = false;
			changeExternalState = false;
			break;
		case Compiler::Opcode::if_command:
			executeIfCommand(cmd, incrementByOne);
			changeExternalState = false;
			nCommand++;
			break;
		}

		// update the instruction counter
		if (incrementByOne)
			m_ic++;

		// return immediately if changed external state or handled ten commands
		if (changeExternalState == true || nCommand == 10)
			return;
	}
}

void Ant::moveForward()
{
	int old_x = getX(), old_y = getY();
	int next_x = getX(), next_y = getY();
	updateNewDest(next_x, next_y);
	if (!checkIfBlocked(next_x, next_y)) // if not blocked by pebble
	{
		moveTo(next_x, next_y);
		getWorld()->helpAnActorMove(this, old_x, old_y, next_x, next_y);
		// reset states if necessary
		m_prevBlocked = false;
		m_prevBitten = false;
		if (returnStunnedState())
			setStunnedState(false);
		if (returnPoisonedState())
			setPoisonedState(false);
	}
	else // if blocked by pebble
		m_prevBlocked = true;
}

void Ant::eatFood()
{
	if (m_nFood > 100) // consume 100 units of food it takes
	{
		changeHPBy(100);
		m_nFood -= 100;
	}
	else // consume all food available, if any
	{
		changeHPBy(m_nFood);
		m_nFood = 0;
	}
}

void Ant::dropFoodHere()
{
	if (m_nFood > 0)
		getWorld()->addFoodAt(getX(), getY(), m_nFood);
	m_nFood = 0;
}

void Ant::bite()
{
	bool useless = false;
	if (getWorld()->thereIsMyEnemy(getX(), getY(), m_colonyNumber)) // bite only if there is any enemy
		getWorld()->attackOtherInsectAt(this, getX(), getY(), useless, 15);
}

void Ant::pickupFood()
{
	int capacityLeft = ANT_FOOD_CAPACITY - m_nFood;
	int amtPickUp = 0;
	if (capacityLeft > 400) // if can load more than 400 units of food
		amtPickUp = getWorld()->consumeFoodAt(getX(), getY(), 400);
	else // if cannot load more than 400 units of food
		amtPickUp = getWorld()->consumeFoodAt(getX(), getY(), capacityLeft);
	m_nFood += amtPickUp;
}

void Ant::emitPheromone()
{
	getWorld()->addUpPheromone(getX(), getY(), m_colonyNumber);
}

void Ant::faceRandomDirection()
{
	setDirection(static_cast<Direction>(randInt(up, left)));
}

void Ant::rotateClockwise()
{
	switch (getDirection())
	{
	case Direction::down:
		setDirection(left);
		break;
	case Direction::left:
		setDirection(up);
		break;
	case Direction::right:
		setDirection(down);
		break;
	case Direction::up:
		setDirection(right);
		break;
	}
}

void Ant::rotateCounterClockwise()
{
	switch (getDirection())
	{
	case Direction::down:
		setDirection(right);
		break;
	case Direction::left:
		setDirection(down);
		break;
	case Direction::right:
		setDirection(up);
		break;
	case Direction::up:
		setDirection(left);
		break;
	}
}

void Ant::generateRandomNumber(const Compiler::Command& c)
{
	std::string operand = c.operand1;
	int n = stoi(operand);
	if (n == 0)
		m_lastRandNumber = 0;
	else
		m_lastRandNumber = randInt(0, n - 1);
}

void Ant::executeIfCommand(const Compiler::Command& c, bool& notSuccess)
{
	Compiler::Condition condition = Compiler::Condition(stoi(c.operand1));
	Actor* useless = nullptr;
	int next_x = getX(), next_y = getY();
	bool ifStatementTriggers = false;
	switch (condition)
	{
	case Compiler::Condition::last_random_number_was_zero:
		if (m_lastRandNumber == 0)
			ifStatementTriggers = true;
		break;
	case Compiler::Condition::i_am_carrying_food:
		if (m_nFood > 0) // the ant is carrying food if its m_nFood is greater than 0
			ifStatementTriggers = true;
		break;
	case Compiler::Condition::i_am_hungry:
		if (getHP() <= 25) // the ant is hungry
			ifStatementTriggers = true;
		break;
	case Compiler::Condition::i_am_standing_with_an_enemy:
		if (getWorld()->thereIsMyEnemy(getX(), getY(), m_colonyNumber))
			ifStatementTriggers = true;
		break;
	case Compiler::Condition::i_am_standing_on_food:
		if (getWorld()->thereIsFoodAt(getX(), getY(), useless))
			ifStatementTriggers = true;
		break;
	case Compiler::Condition::i_am_standing_on_my_anthill:
		if (getWorld()->thisIsMyAnthill(getX(), getY(), m_colonyNumber))
			ifStatementTriggers = true;
		break;
	case Compiler::Condition::i_smell_pheromone_in_front_of_me:
		updateNewDest(next_x, next_y);
		if (getWorld()->thisIsMyPheromone(next_x, next_y, m_colonyNumber))
			ifStatementTriggers = true;
		break;
	case Compiler::Condition::i_smell_danger_in_front_of_me:
		updateNewDest(next_x, next_y);
		if (getWorld()->thereIsMyEnemy(next_x, next_y, m_colonyNumber))
			ifStatementTriggers = true;
		break;
	case Compiler::Condition::i_was_bit:
		if (m_prevBitten)
			ifStatementTriggers = true;
		break;
	case Compiler::Condition::i_was_blocked_from_moving:
		if (m_prevBlocked)
			ifStatementTriggers = true;
		break;
	default:
		break;
	}
	if (ifStatementTriggers) // commong thing for if statements
	{
		m_ic = stoi(c.operand2);
		notSuccess = false;
	}
}

void Ant::getAttacked()
{
	m_prevBitten = true;
}

//=============================================Grasshopper====================================

void GrassHopper::doDifferentInsectThings()
{
	bool endTick;
	doDifferentGHThings(endTick);
	if (endTick)
		return;
	doGrassHopperThings();
}

void GrassHopper::doGrassHopperThings()
{
	// eat food every tick, if there is any food.
	// notice if insect eats food and sleeps, next time it wakes up it will eat food again.
	int actualFoodConsumption = getWorld()->consumeFoodAt(getX(), getY(), 200);
	if (actualFoodConsumption > 0)
		// meaning the grasshopper does find some food here
	{
		changeHPBy(actualFoodConsumption);
		int whetherRest = randInt(0, 1);
		if (whetherRest == 1)
			// baby grasshopper has a 50% chance of sleeping after consuming food
		{
			changeSleepTicksBy(2);
			setDoneSomething(true);
			return;
		}
	}

	// change direction if necessary
	if (getDistance() == 0)
	{
		Direction nextDir = getDirection();
		Direction oldDir = nextDir;
		while (nextDir == getDirection())
			nextDir = static_cast<Direction>(randInt(up, left));
		// nextDir must be different from last direction
		setDirection(nextDir);
		changeDistanceBy(randInt(2, 10));
	}

	// attempt move
	int next_x = getX(), next_y = getY();
	int old_x = getX(), old_y = getY();
	updateNewDest(next_x, next_y);
	if (!checkIfBlocked(next_x, next_y)) // if the pebble is not in the way
	{
		changeDistanceBy(-1);
		moveTo(next_x, next_y);
		getWorld()->helpAnActorMove(this, old_x, old_y, next_x, next_y);
		if (returnStunnedState())
			setStunnedState(false);
		if (returnPoisonedState())
			setPoisonedState(false);
	}
	else
		changeDistanceBy(-getDistance());
	// reset sleeping state 
	changeSleepTicksBy(2);
	setDoneSomething(true);
}

void BabyGrassHopper::growUp()
{
	getWorld()->addAdultGrassHopper(getX(), getY());
}

void BabyGrassHopper::doDifferentGHThings(bool& success)
{
	// Evolve to adult grasshopper
	if (getHP() >= ADULT_GRASSHOPPER_INIT_HP)
	{
		growUp();
		setDead(); // old baby grasshopper is dead
		setDoneSomething(true);
		success = true;
		return;
	}
	success = false;
}

void AdultGrassHopper::doDifferentGHThings(bool& success)
{
	// 1/3 probability to attack on another insect
	int whetherAttack = randInt(0, 2);
	if (whetherAttack == 0)
	{
		bool attackSuccess;
		Actor* prey = nullptr;
		getWorld()->attackOtherInsectAt(this, getX(), getY(), attackSuccess, 50);
		if (attackSuccess)
		{
			changeSleepTicksBy(2);
			setDoneSomething(true);
			success = true;
			return;
		}
	}

	// 1/10 chance of jumping
	int whetherJump = randInt(0, 9);
	if (whetherJump == 0 && getWorld()->availableJumpingSpace(getX(), getY()))
		// have available space for jumping
	{
		double radius, theta;
		int temp_x, temp_y;
		int oldX = getX();
		int oldY = getY();
		while (1)
		{
			radius = randInt(2, 10);
			theta = randInt(0, 360) / 180 * PI;
			temp_x = static_cast<int>(oldX + radius * cos(theta));
			temp_y = static_cast<int>(oldY + radius * sin(theta));
			// check whether the coordinates are out of range
			if (temp_x < 0 || temp_x > VIEW_WIDTH - 1 || temp_y < 0 || temp_y > VIEW_HEIGHT - 1)
				continue;
			if (!getWorld()->thisIsAPebble(temp_x, temp_y))
				break;
		}
		moveTo(temp_x, temp_y);
		getWorld()->helpAnActorMove(this, oldX, oldY, temp_x, temp_y);
		changeSleepTicksBy(2);
		setDoneSomething(true);
		success = true;
		return;
	}

	success = false;
}

// temporary auxiliary functions go here

void AdultGrassHopper::getAttacked()
{
	int whetherRetaliate = randInt(0, 1);
	bool useless = false;
	if (whetherRetaliate == 1) // 50% chance of retaliation
		getWorld()->attackOtherInsectAt(this, getX(), getY(), useless, 50);
}

bool Actor::checkIfBlocked(int& destX, int& destY)
{
	return getWorld()->thisIsAPebble(destX, destY);
}

