#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include<cmath>

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp)
	:GraphObject(imageID, SPRITE_WIDTH* startX, SPRITE_HEIGHT* startY, dir, depth, size)
{
	m_isWalking = false;
	//m_isWalking = true;
	m_World = wp;
	m_isAlive = true;
}

Actor::~Actor()
{

}

bool Actor::getWalkingStatus() const
{
	return m_isWalking;
}

void Actor::setWalking(bool active)
{
	m_isWalking = active;
}

bool Actor::getAliveStatus() const
{
	return m_isAlive;
}

void Actor::setAliveStatus(bool active)
{
	m_isAlive = active;
}

StudentWorld* Actor::world()
{
	return m_World;
}

bool Actor::isASquare()
{
	return false;
}

bool Actor::isVortexable()
{
	return false;
}

bool Actor::hasBeenHit()
{
	return m_hadBeenHit;
}

void Actor::setHasBeenHit(bool change)
{
	m_hadBeenHit = change;
}

//MOVERS

Movers::Movers(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp)
	:Actor(imageID, startX, startY, dir, depth, size, wp)
{
	m_walkingDirection = right;
	m_ticksMove = 0;
}

Movers::~Movers()
{

}

void Movers::setWalkingDirection(int dir)
{
	m_walkingDirection = dir;
}

int Movers::getWalkingDirection() const
{
	return m_walkingDirection;
}

bool Movers::isAtFork(Movers* mp)
{
	int count = 0;
	int dir = getWalkingDirection() + 180;
	if (dir >= 360) {
		dir -= 360;
	}
	if (mp->getX() % 16 != 0 || mp->getY() % 16 != 0) {
		return false;
	}
	if (!(world()->isEmptySpace(mp->getX(), mp->getY() + 16)) && dir != up) { //check if up path
		count++;
	}
	if (!(world()->isEmptySpace(mp->getX() + 16, mp->getY())) && dir != right) { //check if right path
		count++;
	}
	if (!(world()->isEmptySpace(mp->getX() - 16, mp->getY())) && dir != left) { //check if left path
		count++;
	}
	if (!(world()->isEmptySpace(mp->getX(), mp->getY() - 16)) && dir != down) { //check if down path
		count++;
	}
	if (count >= 2) {
		return true;
	}
	else {
		return false;
	}
}

bool Movers::isValidPath(int dir, Movers* mp)
{
	int newx, newy;
	mp->getPositionInThisDirection(dir, 16, newx, newy);
	if (world()->isEmptySpace(newx, newy) || world()->peach()->isOutOfBounds(newx, newy))
	{
		return false;
	}
	else {
		return true;
	}
}

int Movers::getTicks() const
{
	return m_ticksMove;
}

void Movers::setTicks(int change)
{
	m_ticksMove = change;
}

void Movers::changeTicks(int change)
{
	m_ticksMove += change;
}

void Movers::teleportToRandomSquare(Movers* mp)
{
	Actor* sq = world()->ObtainRandomSquare();
	int x = sq->getX();
	int y = sq->getY();
	mp->moveTo(x, y);
}

//PLAYER AVATAR IMPLEMENTATION

PlayerAvatar::PlayerAvatar(int imageID, int startX, int startY, int dir, int depth, double size, int playerNumber, StudentWorld* wp)
	:Movers(imageID, startX, startY, GraphObject::right, 0, 1, wp)
{
	m_nCoins = 0;
	m_nStars = 0;
	m_haveVortex = false;
	//m_ticksMove = 0;
	//m_isWaiting = true;
	m_playerNum = playerNumber;
	m_die_roll = 0;
	m_HasActivated = false;
	//m_Vortex = nullptr;
	m_wantVortex = false;
	m_wasSwapped = false;
	m_gamestart = true;
	m_BowserAffected = false;
	m_DropSqAffected = false;
	m_BooAffected = false;
}

PlayerAvatar::~PlayerAvatar()
{

}

int PlayerAvatar::getCoins() const
{
	return m_nCoins;
}

void PlayerAvatar::setCoins(int change)
{
	m_nCoins += change;
	if (m_nCoins < 0) {
		m_nCoins = 0;
	}
}

int PlayerAvatar::getStars() const
{
	return m_nStars;
}

void PlayerAvatar::setStars(int change)
{
	m_nStars += change;
}

bool PlayerAvatar::getVortex() const
{
	return m_haveVortex;
}

void PlayerAvatar::setVortex(bool change)
{
	m_haveVortex = change;
}

bool PlayerAvatar::isOutOfBounds(int x, int y)
{
	if (x >= 255 || x < 0 || y >= 255 || y < 0) {
		return true;
	}
	return false;
}

void PlayerAvatar::doSomething()
{
	if (getWalkingStatus() == false) {
		int button = world()->getAction(getPlayerNum());
		if (button == ACTION_ROLL) {
			//m_die_roll = randInt(1, 10);
			m_die_roll = randInt(1, 10);
			setTicks(m_die_roll * 8);
			setWalking(true);
			setHasActivated(false);
			setWasSwapped(false);
			setBowserAffected(false);
			setActvatedDropsq(false);
			setBooAffected(false);
		}
		if (button == ACTION_FIRE) {
			if (getVortex() == false) {
				return;
			}
			world()->setWantVortex(true);
			world()->addVortex(this);
			world()->playSound(SOUND_PLAYER_FIRE);
		}
		else {
			return;
		}
	}

	if (getWalkingStatus() == true) {
		if (isAtFork(this) && m_gamestart == false && !(world()->isDirSquare(getX(), getY()))) {
			int button = world()->getAction(getPlayerNum());
			int dir = getWalkingDirection() + 180;
			if (dir >= 360) {
				dir -= 360;
			}
			switch (button)
			{
			case ACTION_RIGHT:
				if (right == dir) {
					return;
				}
				if (isValidPath(right, this)) {
					setWalkingDirection(right);
					setDirection(right);
				}
				else {
					return;
				}
				break;
			case ACTION_LEFT:
				if (left == dir) {
					return;
				}
				if (isValidPath(left, this)) {
					setWalkingDirection(left);
					setDirection(left);
				}
				else {
					return;
				}
				break;
			case ACTION_UP:
				if (up == dir) {
					return;
				}
				if (isValidPath(up, this)) {
					setWalkingDirection(up);
					setDirection(right);
				}
				else {
					return;
				}
				break;
			case ACTION_DOWN:
				if (down == dir) {
					return;
				}
				if (isValidPath(down, this)) {
					setWalkingDirection(down);
					setDirection(right);
				}
				else {
					return;
				}
				break;
			case ACTION_NONE:
				return;
			default:
				return;
			}
		}
		int currDir = getWalkingDirection();
		int newx, newy;
		if (currDir == up) {
			getPositionInThisDirection(currDir, 16, newx, newy);
			if (world()->isEmptySpace(newx, newy) || isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(right, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::right);
					moveTo(getX() + 2, getY());
					//m_ticksMove--;
					changeTicks(-1);
					setRoll(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::left);
					setWalkingDirection(GraphObject::left);
					moveTo(getX() - 2, getY());
					//m_ticksMove--;
					changeTicks(-1);
					setRoll(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX(), getY() + 2);
				//m_ticksMove--;
				changeTicks(-1);
				setRoll(ceil(getTicks() / 8.0));
			}
		}
		else if (currDir == right) {
			getPositionInThisDirection(currDir, 16, newx, newy);
			if (world()->isEmptySpace(newx, newy) || isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(up, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::up);
					moveTo(getX(), getY() + 2);
					//m_ticksMove--;
					changeTicks(-1);
					setRoll(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::down);
					moveTo(getX(), getY() - 2);
					//m_ticksMove--;
					changeTicks(-1);
					setRoll(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX() + 2, getY());
				//m_ticksMove--;
				changeTicks(-1);
				setRoll(ceil(getTicks() / 8.0));
			}
		}
		else if (currDir == down) {
			getPositionInThisDirection(currDir, 1, newx, newy);
			if (world()->isEmptySpace(newx, newy) || isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(right, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::right);
					moveTo(getX() + 2, getY());
					//m_ticksMove--;
					changeTicks(-1);
					setRoll(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::left);
					setWalkingDirection(GraphObject::left);
					moveTo(getX() - 2, getY());
					//m_ticksMove--;
					changeTicks(-1);
					setRoll(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX(), getY() - 2);
				//m_ticksMove--;
				changeTicks(-1);
				setRoll(ceil(getTicks() / 8.0));
			}
		}
		else {
			getPositionInThisDirection(currDir, 1, newx, newy);
			if (world()->isEmptySpace(newx, newy) || isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(up, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::up);
					moveTo(getX(), getY() + 2);
					//m_ticksMove--;
					changeTicks(-1);
					setRoll(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::down);
					moveTo(getX(), getY() - 2);
					//m_ticksMove--;
					changeTicks(-1);
					setRoll(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX() - 2, getY());
				//m_ticksMove--;
				changeTicks(-1);
				setRoll(ceil(getTicks() / 8.0));
			}
		}
	}
	m_gamestart = false;
	if (getTicks() <= 0) {
		setWalking(false);
	}
}

int PlayerAvatar::getPlayerNum() const
{
	return m_playerNum;
}

int PlayerAvatar::getRoll() const
{
	return m_die_roll;
}

void PlayerAvatar::setRoll(int change)
{
	m_die_roll = change;
}

void PlayerAvatar::setHasActivated(bool activated)
{
	m_HasActivated = activated;
}

bool PlayerAvatar::getHasActivated()
{
	return m_HasActivated;
}

bool PlayerAvatar::getWantVortex()
{
	return m_wantVortex;
}

void PlayerAvatar::setWantVortex(bool change)
{
	m_wantVortex = change;
}

void PlayerAvatar::setWasSwapped(bool change)
{
	m_wasSwapped = change;
}

bool PlayerAvatar::wasSwapped()
{
	return m_wasSwapped;
}

bool PlayerAvatar::isBowserAffected()
{
	return m_BowserAffected;
}

void PlayerAvatar::setBowserAffected(bool change)
{
	m_BowserAffected = change;
}

bool PlayerAvatar::isBooAffected()
{
	return m_BooAffected;
}

void PlayerAvatar::setBooAffected(bool change)
{
	m_BooAffected = change;
}

bool PlayerAvatar::ActivatedDropSq()
{
	return m_DropSqAffected;
}

void PlayerAvatar::setActvatedDropsq(bool change)
{
	m_DropSqAffected = change;
}

//VORTEX IMPLEMENTATION
Vortex::Vortex(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp, int vDir)
	:Movers(imageID, startX, startY, dir, depth, size, wp)
{
	setWalkingDirection(vDir);
}

Vortex::~Vortex()
{

}

void Vortex::doSomething()
{
	if (getAliveStatus() == false) {
		return;
	}
	
	if (world()->peach()->isOutOfBounds(this->getX(), this->getY())) {
		setAliveStatus(false);
		return;
	}

	if (world()->detectHit(this))
	{
		Actor* toBeTeleported = world()->hasBeenHit(this);
		toBeTeleported->setHasBeenHit(true);
		setAliveStatus(false);
		world()->playSound(SOUND_HIT_BY_VORTEX);

	}

	int dir = getWalkingDirection();
	switch (dir)
	{
	case right:
		moveTo(getX() + 2, getY());
		break;
	case down:
		moveTo(getX(), getY() - 2);
		break;
	case up:
		moveTo(getX(), getY() + 2);
		break;
	case left:
		moveTo(getX() - 2, getY());
		break;
	}
}

//BADDIES IMPLEMENATION

Baddies::Baddies(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp)
	:Movers(imageID, startX, startY, dir, depth, size, wp)
{
	m_pauseCounter = 180;
	m_moveSquares = 0;
}

Baddies::~Baddies()
{

}

bool Baddies::isVortexable()
{
	return true;
}

void Baddies::setPause(int change)
{
	m_pauseCounter += change;
}

int Baddies::getPause()
{
	return m_pauseCounter;
}

int Baddies::getMoveSquares()
{
	return m_moveSquares;
}

void Baddies::setMoveSquares(int change)
{
	m_moveSquares = change;
}

//BOWSER IMPLMENTATION
Bowser::Bowser(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp)
	:Baddies(imageID, startX, startY, dir, depth, size, wp)
{

}

Bowser::~Bowser()
{

}

void Bowser::doSomething()
{
	if (hasBeenHit() == true) {
		teleportToRandomSquare(this);
		setWalkingDirection(right);
		setDirection(right);
		setWalking(false);
		setPause((-getPause()) + 180);
		setHasBeenHit(false);
	}
	if (getWalkingStatus() == false) {
		if (world()->peach()->getX() == getX() && world()->peach()->getY() == getY() 
			&& world()->peach()->getWalkingStatus()==false &&world()->peach()->isBowserAffected() == false) {
			int chance = randInt(1, 2);
			if (chance == 1) {
				world()->peach()->setCoins(-(world()->peach()->getCoins()));
				world()->peach()->setBowserAffected(true);
				world()->playSound(SOUND_BOWSER_ACTIVATE);
				setPause(-1);
			}
			else {
				world()->peach()->setBowserAffected(true);
				setPause(-1);
			}
		}
		if (world()->yoshi()->getX() == getX() && world()->yoshi()->getY() == getY()
			&& world()->yoshi()->getWalkingStatus() == false && world()->yoshi()->isBowserAffected() == false) {
			int chance = randInt(1, 2);
			if (chance == 1) {
				world()->yoshi()->setCoins(-(world()->yoshi()->getCoins()));
				world()->yoshi()->setBowserAffected(true);
				world()->playSound(SOUND_BOWSER_ACTIVATE);
				setPause(-1);
			}
			else {
				world()->yoshi()->setBowserAffected(true);
				setPause(-1);
			}
		}
		setPause(-1);
		if (getPause() <= 0) {
			setMoveSquares(randInt(1, 10));
			int move = getMoveSquares();
			setTicks(getMoveSquares() * 8);

			int dir = randInt(1, 4);
			switch (dir)
			{
			case 1:
			{
				if (isValidPath(right, this)) {
					setWalkingDirection(right);
					setDirection(right);
					break;
				}
			}
			case 2:
			{
				if (isValidPath(left, this)) {
					setWalkingDirection(left);
					setDirection(left);
					break;
				}
			}
			case 3:
			{
				if (isValidPath(up, this)) {
					setWalkingDirection(up);
					setDirection(right);
					break;
				}
			}
			case 4:
			{
				if (isValidPath(down, this)) {
					setWalkingDirection(down);
					setDirection(right);
					break;
				}
			}
			}
			setWalking(true);
		}
	}
	if (getWalkingStatus() == true) {
		if (isAtFork(this) && !(world()->isDirSquare(getX(), getY()))) {
			int dir = randInt(1, 4);
			switch (dir)
			{
			case 1:
				if (isValidPath(right, this)) {
					setWalkingDirection(right);
					setDirection(right);
					break;
				}
			case 2:
				if (isValidPath(left, this)) {
					setWalkingDirection(left);
					setDirection(left);
					break;
				}
			case 3:
				if (isValidPath(up, this)) {
					setWalkingDirection(up);
					setDirection(right);
					break;
				}
			case 4:
				if (isValidPath(down, this)) {
					setWalkingDirection(down);
					setDirection(right);
					break;
				}
			}
		}
		int currDir = getWalkingDirection();
		int newx, newy;
		if (currDir == up) {
			getPositionInThisDirection(currDir, 16, newx, newy);
			if (world()->isEmptySpace(newx, newy) || world()->peach()->isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(right, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !world()->peach()->isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::right);
					moveTo(getX() + 2, getY());
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::left);
					setWalkingDirection(GraphObject::left);
					moveTo(getX() - 2, getY());
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX(), getY() + 2);
				changeTicks(-1);
				setMoveSquares(ceil(getTicks() / 8.0));
			}
		}
		else if (currDir == right) {
			getPositionInThisDirection(currDir, 16, newx, newy);
			if (world()->isEmptySpace(newx, newy) || world()->peach()->isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(up, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !world()->peach()->isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::up);
					moveTo(getX(), getY() + 2);
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::down);
					moveTo(getX(), getY() - 2);
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX() + 2, getY());
				changeTicks(-1);
				setMoveSquares(ceil(getTicks() / 8.0));
			}
		}
		else if (currDir == down) {
			getPositionInThisDirection(currDir, 1, newx, newy);
			if (world()->isEmptySpace(newx, newy) || world()->peach()->isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(right, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !world()->peach()->isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::right);
					moveTo(getX() + 2, getY());
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::left);
					setWalkingDirection(GraphObject::left);
					moveTo(getX() - 2, getY());
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX(), getY() - 2);
				changeTicks(-1);
				setMoveSquares(ceil(getTicks() / 8.0));
			}
		}
		else {
			getPositionInThisDirection(currDir, 1, newx, newy);
			if (world()->isEmptySpace(newx, newy) || world()->peach()->isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(up, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !world()->peach()->isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::up);
					moveTo(getX(), getY() + 2);
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::down);
					moveTo(getX(), getY() - 2);
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX() - 2, getY());
				changeTicks(-1);
				setMoveSquares(ceil(getTicks() / 8.0));
			}
		}
		if (getTicks() <= 0) { //LEFT OFF HERE, AUTOMATICALLY COMES HERE MISTAKENLY
			setWalking(false);
			setPause(180);

			int bowserDump = randInt(1, 4);
			if (bowserDump == 4) {
				world()->createDropping(this);
				world()->playSound(SOUND_DROPPING_SQUARE_CREATED); //this sound crashed my program
			}
		}
	}
}

//BOO IMPLEMENTATION
Boo::Boo(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp)
	:Baddies(imageID, startX, startY, dir, depth, size, wp)
{

}

Boo::~Boo()
{

}

void Boo::doSomething()
{
	if (hasBeenHit() == true) {
		teleportToRandomSquare(this);
		setWalkingDirection(right);
		setDirection(right);
		setWalking(false);
		setPause((-getPause()) + 180);
		setHasBeenHit(false);
	}
	if (getWalkingStatus() == false) {
		if (world()->peach()->getX() == getX() && world()->peach()->getY() == getY()
			&& world()->peach()->getWalkingStatus() == false && world()->peach()->isBooAffected() == false) {
			int chance = randInt(1, 2);
			if (chance == 1) {
				int tempCoins = world()->peach()->getCoins();
				world()->peach()->setCoins(-tempCoins + world()->yoshi()->getCoins());
				world()->yoshi()->setCoins(-world()->yoshi()->getCoins() + tempCoins);
				world()->peach()->setBooAffected(true);
				world()->playSound(SOUND_BOO_ACTIVATE);
				setPause(-1);
			}
			else {
				int tempStar = world()->peach()->getStars();
				world()->peach()->setStars(-tempStar + world()->yoshi()->getStars());
				world()->yoshi()->setStars(-world()->yoshi()->getStars() + tempStar);
				world()->peach()->setBooAffected(true);
				world()->playSound(SOUND_BOO_ACTIVATE);
				setPause(-1);
			}
		}
		if (world()->yoshi()->getX() == getX() && world()->yoshi()->getY() == getY()
			&& world()->yoshi()->getWalkingStatus() == false && world()->yoshi()->isBooAffected() == false) {
			int chance = randInt(1, 2);
			if (chance == 1) {
				int tempCoins = world()->peach()->getCoins();
				world()->peach()->setCoins(-tempCoins + world()->yoshi()->getCoins());
				world()->yoshi()->setCoins(-world()->yoshi()->getCoins() + tempCoins);
				world()->yoshi()->setBooAffected(true);
				world()->playSound(SOUND_BOO_ACTIVATE);
				setPause(-1);
			}
			else {
				int tempStar = world()->peach()->getStars();
				world()->peach()->setStars(-tempStar + world()->yoshi()->getStars());
				world()->yoshi()->setStars(-world()->yoshi()->getStars() + tempStar);
				world()->yoshi()->setBooAffected(true);
				world()->playSound(SOUND_BOO_ACTIVATE);
				setPause(-1);
			}
		}
		setPause(-1);
		if (getPause() <= 0) {
			setMoveSquares(randInt(1, 3));
			int move = getMoveSquares();
			setTicks(getMoveSquares() * 8);

			int dir = randInt(1, 4);
			switch (dir)
			{
			case 1:
			{
				if (isValidPath(right, this)) {
					setWalkingDirection(right);
					setDirection(right);
					break;
				}
			}
			case 2:
			{
				if (isValidPath(left, this)) {
					setWalkingDirection(left);
					setDirection(left);
					break;
				}
			}
			case 3:
			{
				if (isValidPath(up, this)) {
					setWalkingDirection(up);
					setDirection(right);
					break;
				}
			}
			case 4:
			{
				if (isValidPath(down, this)) {
					setWalkingDirection(down);
					setDirection(right);
					break;
				}
			}
			}
			setWalking(true);
		}
	}
	if (getWalkingStatus() == true) {
		if (isAtFork(this) && !(world()->isDirSquare(getX(), getY()))) {
			int dir = randInt(1, 4);
			switch (dir)
			{
			case 1:
				if (isValidPath(right, this)) {
					setWalkingDirection(right);
					setDirection(right);
					break;
				}
			case 2:
				if (isValidPath(left, this)) {
					setWalkingDirection(left);
					setDirection(left);
					break;
				}
			case 3:
				if (isValidPath(up, this)) {
					setWalkingDirection(up);
					setDirection(right);
					break;
				}
			case 4:
				if (isValidPath(down, this)) {
					setWalkingDirection(down);
					setDirection(right);
					break;
				}
			}
		}
		int currDir = getWalkingDirection();
		int newx, newy;
		if (currDir == up) {
			getPositionInThisDirection(currDir, 16, newx, newy);
			if (world()->isEmptySpace(newx, newy) || world()->peach()->isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(right, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !world()->peach()->isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::right);
					moveTo(getX() + 2, getY());
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::left);
					setWalkingDirection(GraphObject::left);
					moveTo(getX() - 2, getY());
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX(), getY() + 2);
				changeTicks(-1);
				setMoveSquares(ceil(getTicks() / 8.0));
			}
		}
		else if (currDir == right) {
			getPositionInThisDirection(currDir, 16, newx, newy);
			if (world()->isEmptySpace(newx, newy) || world()->peach()->isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(up, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !world()->peach()->isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::up);
					moveTo(getX(), getY() + 2);
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::down);
					moveTo(getX(), getY() - 2);
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX() + 2, getY());
				changeTicks(-1);
				setMoveSquares(ceil(getTicks() / 8.0));
			}
		}
		else if (currDir == down) {
			getPositionInThisDirection(currDir, 1, newx, newy);
			if (world()->isEmptySpace(newx, newy) || world()->peach()->isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(right, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !world()->peach()->isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::right);
					moveTo(getX() + 2, getY());
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::left);
					setWalkingDirection(GraphObject::left);
					moveTo(getX() - 2, getY());
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX(), getY() - 2);
				changeTicks(-1);
				setMoveSquares(ceil(getTicks() / 8.0));
			}
		}
		else {
			getPositionInThisDirection(currDir, 1, newx, newy);
			if (world()->isEmptySpace(newx, newy) || world()->peach()->isOutOfBounds(newx, newy)) {
				getPositionInThisDirection(up, 16, newx, newy);
				if (!world()->isEmptySpace(newx, newy) && !world()->peach()->isOutOfBounds(newx, newy)) {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::up);
					moveTo(getX(), getY() + 2);
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
				else {
					setDirection(GraphObject::right);
					setWalkingDirection(GraphObject::down);
					moveTo(getX(), getY() - 2);
					changeTicks(-1);
					setMoveSquares(ceil(getTicks() / 8.0));
				}
			}
			else {
				moveTo(getX() - 2, getY());
				changeTicks(-1);
				setMoveSquares(ceil(getTicks() / 8.0));
			}
		}
		if (getTicks() <= 0) { //LEFT OFF HERE, AUTOMATICALLY COMES HERE MISTAKENLY
			setWalking(false);
			setPause(180);
		}
	}
}

//SQUARE IMPLEMETATION
Square::Square(int imageID, int startX, int startY, int dir, StudentWorld* wp)
	:Actor(imageID, startX, startY, dir, 1, 1, wp)
{
	m_isActivated = false;
	//m_isASquare = false;
}

Square::~Square()
{

}

void Square::setActivated(bool active)
{
	m_isActivated = active;
}

bool Square::isActivated() const
{
	return m_isActivated;
}

bool Square::isASquare()
{
	return true;
}

//COINSQUARE IMPLEMETATION
CoinSquare::CoinSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp)
	:Square(imageID, startX, startY, dir, wp)
{
}

CoinSquare::~CoinSquare()
{

}

void CoinSquare::changeCoins(PlayerAvatar* pp, int change)
{
	pp->setCoins(change);
}

//BLUE COIN SQUARE IMPLEMENTATION
BlueCoinSquare::BlueCoinSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp)
	:CoinSquare(imageID, startX, startY, dir, wp)
{

}

void BlueCoinSquare::doSomething()
{

	if (!getAliveStatus()) {
		return;
	}

	int x2 = getX();
	int y2 = getY();

	int xp = world()->peach()->getX();
	int yp = world()->peach()->getY();

	int xy = world()->yoshi()->getX();
	int yy = world()->yoshi()->getY();

	if (world()->peach()->getWalkingStatus() == true && world()->peach()->getHasActivated() == true) {
		world()->peach()->setHasActivated(false);
	}
	if (world()->yoshi()->getWalkingStatus() == true && world()->yoshi()->getHasActivated() == true) {
		world()->yoshi()->setHasActivated(false);
	}

	if (xp == x2 && yp == y2 && world()->peach()->getWalkingStatus() == false && 
		world()->peach()->getTicks() <= 0 && world()->peach()->getHasActivated() == false) {
		changeCoins(world()->peach(), 3);
		world()->playSound(SOUND_GIVE_COIN);
		setActivated(true);
		world()->peach()->setHasActivated(true);
	}
	if (xy == x2 && yy == y2 && world()->yoshi()->getWalkingStatus() == false &&
		world()->yoshi()->getTicks() <= 0 && world()->yoshi()->getHasActivated() == false) {
		changeCoins(world()->yoshi(), 3);
		world()->playSound(SOUND_GIVE_COIN);
		setActivated(true);
		world()->yoshi()->setHasActivated(true);
	}
}

BlueCoinSquare::~BlueCoinSquare()
{

}

//RED COIN SQUARE IMPLMENTATION
RedCoinSquare::RedCoinSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp)
	:CoinSquare(imageID, startX, startY, dir, wp)
{

}

RedCoinSquare::~RedCoinSquare()
{

}

void RedCoinSquare::doSomething()
{
	if (!getAliveStatus()) {
		return;
	}

	int x2 = getX();
	int y2 = getY();

	int xp = world()->peach()->getX();
	int yp = world()->peach()->getY();

	int xy = world()->yoshi()->getX();
	int yy = world()->yoshi()->getY();

	if (world()->peach()->getWalkingStatus() == true && world()->peach()->getHasActivated() == true) {
		world()->peach()->setHasActivated(false);
	}
	if (world()->yoshi()->getWalkingStatus() == true && world()->yoshi()->getHasActivated() == true) {
		world()->yoshi()->setHasActivated(false);
	}

	if (xp == x2 && yp == y2 && world()->peach()->getWalkingStatus() == false &&
		world()->peach()->getTicks() <= 0 && world()->peach()->getHasActivated() == false) {
		changeCoins(world()->peach(), -3);
		world()->playSound(SOUND_TAKE_COIN);
		setActivated(true);
		world()->peach()->setHasActivated(true);
	}
	if (xy == x2 && yy == y2 && world()->yoshi()->getWalkingStatus() == false &&
		world()->yoshi()->getTicks() <= 0 && world()->yoshi()->getHasActivated() == false) {
		changeCoins(world()->yoshi(), -3);
		world()->playSound(SOUND_TAKE_COIN);
		setActivated(true);
		world()->yoshi()->setHasActivated(true);
	}
}

//STAR SQUARE IMPLEMENTATION
StarSquare::StarSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp)
	:CoinSquare(imageID, startX, startY, dir, wp)
{

}

StarSquare::~StarSquare()
{

}

void StarSquare::doSomething()
{

	int x2 = getX();
	int y2 = getY();

	int xp = world()->peach()->getX();
	int yp = world()->peach()->getY();

	int xy = world()->yoshi()->getX();
	int yy = world()->yoshi()->getY();


	if (xp == x2 && yp == y2 && world()->peach()->getWalkingStatus() == true && isActivated() == false
		&&world()->peach()->getCoins() >= 20) { //passing
		changeCoins(world()->peach(), -20);
		world()->playSound(SOUND_GIVE_STAR);
		world()->peach()->setStars(1);
	}

	if (xy == x2 && yy == y2 && world()->yoshi()->getWalkingStatus() == true && isActivated() == false
		&& world()->yoshi()->getCoins() >= 20) { //passing
		changeCoins(world()->yoshi(), -20);
		world()->playSound(SOUND_GIVE_STAR);
		world()->yoshi()->setStars(1);
	}

	if (world()->peach()->getRoll() > 0) {
		world()->peach()->setHasActivated(false);
		setActivated(false);
	}
	if (world()->yoshi()->getRoll() > 0) {
		world()->yoshi()->setHasActivated(false);
		setActivated(false);
	}

	if (xp == x2 && yp == y2 && world()->peach()->getWalkingStatus() == false
		&& world()->peach()->getHasActivated() == false &&world()->peach()->getCoins() >= 20) { //standing on bank
		changeCoins(world()->peach(), -20);
		world()->playSound(SOUND_GIVE_STAR);
		world()->peach()->setStars(1);
		setActivated(true);
		world()->peach()->setHasActivated(true);
	}
	if (xy == x2 && yy == y2 && world()->yoshi()->getWalkingStatus() == false
		&& world()->yoshi()->getHasActivated() == false && world()->yoshi()->getCoins() >= 20) { //standing on bank
		changeCoins(world()->peach(), -20);
		world()->playSound(SOUND_GIVE_STAR);
		world()->yoshi()->setStars(1);
		setActivated(true);
		world()->yoshi()->setHasActivated(true);
	}
}

//BANKSQUARE IJMPLEMETATION
BankSquare::BankSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp)
	:CoinSquare(imageID, startX, startY, dir, wp)
{

}

BankSquare::~BankSquare()
{

}

void BankSquare::doSomething()
{
	int x2 = getX();
	int y2 = getY();

	int xp = world()->peach()->getX();
	int yp = world()->peach()->getY();

	int xy = world()->yoshi()->getX();
	int yy = world()->yoshi()->getY();


	if (xp == x2 && yp == y2 && world()->peach()->getWalkingStatus() == true && isActivated() == false) { //passing
		if (world()->peach()->getCoins() < 5) {
			world()->setBankCoins((world()->peach()->getCoins()));
			changeCoins(world()->peach(), -(world()->peach()->getCoins()));
		}
		else {
			changeCoins(world()->peach(), -5);
			world()->setBankCoins(5);
		}
		world()->playSound(SOUND_DEPOSIT_BANK);
	}

	if (xy == x2 && yy == y2 && world()->yoshi()->getWalkingStatus() == true && isActivated() == false) { //passing
		if (world()->yoshi()->getCoins() < 5) {
			world()->setBankCoins((world()->yoshi()->getCoins()));
			changeCoins(world()->yoshi(), -(world()->yoshi()->getCoins()));
		}
		else {
			changeCoins(world()->yoshi(), -5);
			world()->setBankCoins(5);
		}
		world()->playSound(SOUND_DEPOSIT_BANK);
	}

	if (world()->peach()->getRoll() > 0 ) {
		world()->peach()->setHasActivated(false);
		setActivated(false);
	}
	if (world()->yoshi()->getRoll() > 0) {
		world()->yoshi()->setHasActivated(false);
		setActivated(false);
	}

	if (xp == x2 && yp == y2 && world()->peach()->getWalkingStatus() == false 
			&& world()->peach()->getHasActivated() == false) { //standing on bank
		changeCoins(world()->peach(), world()->getBankCoins());
		world()->setBankCoins(-(world()->getBankCoins()));
		world()->playSound(SOUND_WITHDRAW_BANK);
		setActivated(true);
		world()->peach()->setHasActivated(true);
	}
	if (xy == x2 && yy == y2 && world()->yoshi()->getWalkingStatus() == false
		&& world()->yoshi()->getHasActivated() == false) { //standing on bank
		changeCoins(world()->yoshi(), world()->getBankCoins());
		world()->setBankCoins(-(world()->getBankCoins()));
		world()->playSound(SOUND_WITHDRAW_BANK);
		setActivated(true);
		world()->yoshi()->setHasActivated(true);
	}
}

//DOPPING SQUARE
DroppingSquare::DroppingSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp)
	:CoinSquare(imageID, startX, startY, dir, wp)
{

}

DroppingSquare::~DroppingSquare()
{

}

void DroppingSquare::doSomething()
{
	if (world()->peach()->getX() == getX() && world()->peach()->getY() == getY()
		&& world()->peach()->getWalkingStatus() == false && world()->peach()->ActivatedDropSq() == false)
	{
		int chance = randInt(1, 2);
		if (chance == 1) {
			world()->peach()->setCoins(-10);
			world()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			world()->peach()->setActvatedDropsq(true);
		}
		else {
			if (world()->peach()->getStars() >= 1) {
				world()->peach()->setStars(-1);
				world()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
				world()->peach()->setActvatedDropsq(true);
			}
			else {
				world()->peach()->setCoins(-10);
				world()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
				world()->peach()->setActvatedDropsq(true);
			}
		}
	}
	if (world()->yoshi()->getX() == getX() && world()->yoshi()->getY() == getY()
		&& world()->yoshi()->getWalkingStatus() == false && world()->yoshi()->ActivatedDropSq() == false)
	{
		int chance = randInt(1, 2);
		if (chance == 1) {
			world()->yoshi()->setCoins(-10);
			world()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			world()->yoshi()->setActvatedDropsq(true);
		}
		else {
			if (world()->yoshi()->getStars() >= 1) {
				world()->yoshi()->setStars(-1);
				world()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
				world()->yoshi()->setActvatedDropsq(true);
			}
			else {
				world()->yoshi()->setCoins(-10);
				world()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
				world()->yoshi()->setActvatedDropsq(true);
			}
		}
	}
}

//EVENT SQAURE IMPLEMENTATION
EventSquare::EventSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp)
	:Square(imageID, startX, startY, dir, wp)
{

}

EventSquare::~EventSquare()
{

}

void EventSquare::teleportRandomSquare(PlayerAvatar* pp)
{
	int x,y;
	x = (randInt(0, 15));
	y = (randInt(0, 15));
	while (world()->isEmptySpace(x * 16, y * 16) || pp->isOutOfBounds(x * 16, y * 16)) {
		x = (randInt(0, 15));
		y = (randInt(0, 15));
	}
	pp->moveTo(x * 16, y * 16);
}

int EventSquare::roundTo16(int num)
{
	int remainder = num % 16;
	if (remainder == 0)
		return num;

	return num + 16 - remainder;
}

void EventSquare::swapPlayers(PlayerAvatar* p1, PlayerAvatar* p2)
{

	//set x and y
	int x1, y1, x2, y2;
	x1 = p1->getX();
	y1 = p1->getY();
	x2 = p2->getX();
	y2 = p2->getY();

	p1->moveTo(x2, y2);
	p2->moveTo(x1, y1);

	//swap ticks
	int tempTicks = p1->getTicks();
	p1->setTicks(p2->getTicks());
	p2->setTicks(tempTicks);

	//swap walk dirs
	int tempDir = p1->getWalkingDirection();
	p1->setWalking(p2->getWalkingDirection());
	p2->setWalking(tempDir);

	//swap sprite dir
	int tempSprite = p1->getDirection();
	p1->setDirection(p2->getDirection());
	p2->setDirection(tempSprite);

	//swap walkstate
	bool tempWalk = p1->getWalkingStatus();
	p1->setWalking(p2->getWalkingDirection());
	p2->setWalking(tempWalk);
}

void EventSquare::doSomething()
{
	int x2 = getX();
	int y2 = getY();

	int xp = world()->peach()->getX();
	int yp = world()->peach()->getY();

	int xy = world()->yoshi()->getX();
	int yy = world()->yoshi()->getY();


	if (xp == x2 && yp == y2 && world()->peach()->getWalkingStatus() == false &&
		world()->peach()->getTicks() <= 0 && world()->peach()->wasSwapped() == false) {
		int event = randInt(1,3);
		switch (event)
		{
			case 1:
				teleportRandomSquare(world()->peach());
				world()->playSound(SOUND_PLAYER_TELEPORT);
				break;
			case 2:
				swapPlayers(world()->peach(), world()->yoshi());
				world()->playSound(SOUND_PLAYER_TELEPORT);
				world()->yoshi()->setWasSwapped(true);
				world()->peach()->setWasSwapped(true);
				break;
			case 3:
				if (world()->peach()->getVortex() == false) {
					world()->peach()->setVortex(true);
				}
				world()->peach()->setWasSwapped(true);
				world()->playSound(SOUND_GIVE_VORTEX);
				break;
		}
	}
	if (xy == x2 && yy == y2 && world()->yoshi()->getWalkingStatus() == false &&
		world()->yoshi()->getTicks() <= 0 && world()->yoshi()->wasSwapped() == false) {
		int event = randInt(1, 3);
		switch (event)
		{
		case 1:
			teleportRandomSquare(world()->yoshi());
			world()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 2:
			swapPlayers(world()->yoshi(), world()->peach());
			world()->playSound(SOUND_PLAYER_TELEPORT);
			world()->peach()->setWasSwapped(true);
			world()->yoshi()->setWasSwapped(true);
			break;
		case 3:
			if (world()->yoshi()->getVortex() == false) {
				world()->yoshi()->setVortex(true);
			}
			world()->yoshi()->setWasSwapped(true);
			world()->playSound(SOUND_GIVE_VORTEX);
			break;
		}
	}
}

//DirectionSquare Implementation
DirectionalSquare::DirectionalSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp)
	:Square(imageID, startX, startY, dir, wp)
{

}

DirectionalSquare::~DirectionalSquare()
{

}

void DirectionalSquare::doSomething()
{
	int x2 = getX();
	int y2 = getY();

	int xp = world()->peach()->getX();
	int yp = world()->peach()->getY();

	int xy = world()->yoshi()->getX();
	int yy = world()->yoshi()->getY();

	if (xy == x2 && yy == y2 && world()->yoshi()->getWalkingStatus() == true) {
		int dir = getDirection();
		world()->yoshi()->setWalkingDirection(dir);
		if (dir == left)
		{
			world()->yoshi()->setDirection(dir);
		}
	}

	if (xp == x2 && yp == y2 && world()->peach()->getWalkingStatus() == true) {
		int dir = getDirection();
		world()->peach()->setWalkingDirection(dir);
		if (dir == left)
		{
			world()->peach()->setDirection(dir);
		}
	}
}

