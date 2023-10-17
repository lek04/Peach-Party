#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp);
	virtual ~Actor();
	virtual void doSomething() = 0;
	bool getWalkingStatus() const;
	void setWalking(bool active);
	bool getAliveStatus() const;
	void setAliveStatus(bool active);
	virtual bool isASquare();
	virtual bool isVortexable();
	bool hasBeenHit();
	void setHasBeenHit(bool change);

	StudentWorld* world();

private:
	bool m_isWalking;
	bool m_isAlive;
	bool m_hadBeenHit;
	StudentWorld* m_World;
};

class Movers: public Actor
{
public:
	Movers(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp);
	virtual ~Movers();
	int getWalkingDirection() const;
	void setWalkingDirection(int dir);
	bool isAtFork(Movers* mp);
	bool isValidPath(int dir, Movers* mp);
	void setTicks(int change);
	int getTicks() const;
	void changeTicks(int change);
	void teleportToRandomSquare(Movers* mp);
	//bool isBlocked(Movers* mp);
	//is at fork
	//is blocked (for corners)
	//set/get dir
private:
	int m_walkingDirection;
	int m_ticksMove;
};

class Vortex;
class PlayerAvatar : public Movers
{
public:
	PlayerAvatar(int imageID, int startX, int startY, int dir, int depth, double size, int playerNumber, StudentWorld* wp);
	virtual ~PlayerAvatar();
	int getCoins() const;
	void setCoins(int change);
	int getStars() const;
	void setStars(int change);
	bool getVortex() const;
	void setVortex(bool change);
	virtual void doSomething();
	int getPlayerNum() const;
	int getRoll() const;
	void setRoll(int change);
	void setHasActivated(bool activated);
	bool getHasActivated();
	bool getWantVortex();
	void setWantVortex(bool change);
	bool isOutOfBounds(int x, int y);
	void setWasSwapped(bool change);
	bool wasSwapped();
	bool isBowserAffected();
	void setBowserAffected(bool change);
	bool ActivatedDropSq();
	void setActvatedDropsq(bool change);
	bool isBooAffected();
	void setBooAffected(bool change);
	//bool hasGameStarted();

private:
	int m_nCoins;
	int m_nStars;
	bool m_haveVortex;
	//bool m_isWaiting;
	int m_playerNum;
	int m_die_roll;
	bool m_HasActivated;
	bool m_wantVortex;
	//Vortex* m_Vortex;
	bool m_wasSwapped;
	bool m_gamestart;
	bool m_BowserAffected;
	bool m_DropSqAffected;
	bool m_BooAffected;
};

class Vortex : public Movers
{
public:
	Vortex(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp, int vDir);
	virtual ~Vortex();
	virtual void doSomething();

private:

};

class Baddies : public Movers
{
public:
	Baddies(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp);
	virtual ~Baddies();
	virtual void doSomething() = 0;
	void setPause(int change);
	int getPause();
	int getMoveSquares();
	void setMoveSquares(int change);
	virtual bool isVortexable();
private:
	int m_pauseCounter;
	int m_moveSquares;
};

class Bowser : public Baddies
{
public:
	Bowser(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp);
	virtual void doSomething();
	virtual ~Bowser();

private:
};

class Boo : public Baddies
{
public:
	Boo(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* wp);
	virtual void doSomething();
	virtual ~Boo();

private:
};

/*class Peach :public PlayerAvatar
{
public:
	Peach(int imageID, int startX, int startY, int dir, int depth, double size, int playerNumber, StudentWorld* wp);
	virtual ~Peach();

private:

}; */

class Square : public Actor
{
public:
	Square(int imageID, int startX, int startY, int dir, StudentWorld* wp);
	virtual ~Square();
	bool isActivated() const;
	void setActivated(bool active);
	virtual bool isASquare();

private:
	bool m_isActivated;
};

class CoinSquare : public Square
{
public:
	CoinSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp);
	virtual ~CoinSquare();
	void changeCoins(PlayerAvatar* pp, int change);

private:
};

class BlueCoinSquare : public CoinSquare
{
public:
	BlueCoinSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp);
	virtual void doSomething();
	virtual ~BlueCoinSquare();

private:
};

class RedCoinSquare : public CoinSquare
{
public:
	RedCoinSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp);
	virtual void doSomething();
	virtual ~RedCoinSquare();

private:
};

class StarSquare : public CoinSquare
{
public:
	StarSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp);
	virtual void doSomething();
	//virtual void changeCoins(PlayerAvatar* pp, int change);
	virtual ~StarSquare();
private:

};

class BankSquare : public CoinSquare
{
public:
	BankSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp);
	virtual void doSomething();
	virtual ~BankSquare();

private:
};

class DroppingSquare : public CoinSquare
{
public:
	DroppingSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp);
	virtual void doSomething();
	virtual ~DroppingSquare();

private:
};

class EventSquare :public Square
{
public:
	EventSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp);
	virtual void doSomething();
	virtual ~EventSquare();
	void teleportRandomSquare(PlayerAvatar* pp);
	void swapPlayers(PlayerAvatar* p1, PlayerAvatar* p2);
	int roundTo16(int num);

private:
};

class DirectionalSquare : public Square
{
public:
	DirectionalSquare(int imageID, int startX, int startY, int dir, StudentWorld* wp);
	virtual void doSomething();
	virtual ~DirectionalSquare();

private:
};

#endif // ACTOR_H_
