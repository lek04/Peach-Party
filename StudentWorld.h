#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class PlayerAvatar;
class Movers;
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	PlayerAvatar* peach();
	PlayerAvatar* yoshi();
	bool isEmptySpace(int x, int y);
	void setWantVortex(bool change);
	bool getWantVortex();
	void addVortex(PlayerAvatar* mp);
	void setBankCoins(int change);
	int getBankCoins();
	bool isDirSquare(int x, int y);
	Actor* ObtainRandomSquare();
	void createDropping(Movers* mp);
	Actor* hasBeenHit(Movers* mp);
	bool detectHit(Movers* mp);

private:
	PlayerAvatar* m_Peach;
	PlayerAvatar* m_Yoshi;
	Board bd;
	bool m_wantVortex;
	int m_BankCoins;
	std::list<Actor*> m_Actors;
};

#endif // STUDENTWORLD_H_
