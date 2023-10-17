#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "GraphObject.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
    m_Peach = nullptr;
    m_Yoshi = nullptr;
    m_wantVortex = false;
    m_BankCoins = 0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init() //FIX THIS LATER
{
    //Board bd;
    
    int x = getBoardNumber();

    string board_file;
    switch(x)
    {
    case 1:
        board_file = assetPath() + "board01.txt";
        break;
    case 2:
        board_file = assetPath() + "board02.txt";
        break;
    case 3:
        board_file = assetPath() + "board03.txt";
        break;
    case 4:
        board_file = assetPath() + "board04.txt";
        break;
    case 5:
        board_file = assetPath() + "board05.txt";
        break;
    case 6:
        board_file = assetPath() + "board06.txt";
        break;
    case 7:
        board_file = assetPath() + "board07.txt";
        break;
    case 8:
        board_file = assetPath() + "board08.txt";
        break;
    case 9:
        board_file = assetPath() + "board09.txt";
        break;
    }

    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found) {
        cerr << "Could not find board01.txt data file\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format) {
        cerr << "Your board was improperly formatted\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                Board::GridEntry ge = bd.getContentsOf(x, y); // x=5, y=10
                switch (ge) {
                case Board::empty:
                    cerr << "Location " << x << ", " << y << " is empty\n";
                    break;
                case Board::boo:
                    m_Actors.push_back(new Boo(IID_BOO,x, y, GraphObject::right, 0, 1, this));
                    m_Actors.push_back(new BlueCoinSquare(IID_BLUE_COIN_SQUARE, x, y, GraphObject::right, this));
                    cerr << "Location " << x << ", " << y << " has a Boo and a blue coin square\n";
                    break;
                case Board::bowser:
                    m_Actors.push_back(new Bowser(IID_BOWSER, x, y, GraphObject::right, 0, 1, this));
                    m_Actors.push_back(new BlueCoinSquare(IID_BLUE_COIN_SQUARE, x, y, GraphObject::right, this));
                    cerr << "Location " << x << ", " << y << " has a Bowser and a blue coin square\n";
                    break;
                case Board::player:
                    m_Peach = new PlayerAvatar(IID_PEACH, x, y, GraphObject::right, 0, 1, 1, this);
                    m_Yoshi = new PlayerAvatar(IID_YOSHI, x, y, GraphObject::right, 0, 1, 2, this);
                    m_Actors.push_back(m_Peach);
                    m_Actors.push_back(m_Yoshi);
                    m_Actors.push_back(new BlueCoinSquare(IID_BLUE_COIN_SQUARE, x, y, GraphObject::right, this));
                    cerr << "Location " << x << ", " << y << " has Peach & Yoshi and a blue coin square\n";
                    break;
                case Board::red_coin_square:
                    m_Actors.push_back(new RedCoinSquare(IID_RED_COIN_SQUARE, x, y, GraphObject::right, this));
                    cerr << "Location " << x << ", " << y << " has a red coin square\n";
                    break;
                case Board::blue_coin_square:
                    m_Actors.push_back(new BlueCoinSquare(IID_BLUE_COIN_SQUARE, x, y, GraphObject::right, this));
                    //m_Actors.push_back(new EventSquare(IID_EVENT_SQUARE, x, y, GraphObject::right, this));                    cerr << "Location " << x << ", " << y << " has a blue coin square\n";
                    break;
                case Board::up_dir_square:
                    m_Actors.push_back(new DirectionalSquare(IID_DIR_SQUARE, x, y, GraphObject::up, this));
                    cerr << "Location " << x << ", " << y << " has an up square\n";
                    break;
                case Board::right_dir_square:
                    m_Actors.push_back(new DirectionalSquare(IID_DIR_SQUARE, x, y, GraphObject::right, this));
                    cerr << "Location " << x << ", " << y << " has an right square\n";
                    break;
                case Board::down_dir_square:
                    m_Actors.push_back(new DirectionalSquare(IID_DIR_SQUARE, x, y, GraphObject::down, this));
                    cerr << "Location " << x << ", " << y << " has an down square\n";
                    break;
                case Board::left_dir_square:
                    m_Actors.push_back(new DirectionalSquare(IID_DIR_SQUARE, x, y, GraphObject::left, this));
                    cerr << "Location " << x << ", " << y << " has an left square\n";
                    break;
                case Board::event_square:
                    m_Actors.push_back(new EventSquare(IID_EVENT_SQUARE, x, y, GraphObject::right, this));
                    //m_Actors.push_back(new BankSquare(IID_BANK_SQUARE, x, y, GraphObject::right, this));
                    cerr << "Location " << x << ", " << y << " has an event square\n";
                    break;
                case Board::star_square:
                    m_Actors.push_back(new StarSquare(IID_STAR_SQUARE, x, y, GraphObject::right, this));
                    cerr << "Location " << x << ", " << y << " has a star square\n";
                    break;
                case Board::bank_square:
                    m_Actors.push_back(new BankSquare(IID_BANK_SQUARE, x, y, GraphObject::right, this));
                    cerr << "Location " << x << ", " << y << " has a bank square\n";
                    break;
                }
            }
        }
    }
    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setWantVortex(bool change)
{
    m_wantVortex = change;
}

bool StudentWorld::getWantVortex()
{
    return m_wantVortex;
}

void StudentWorld::addVortex(PlayerAvatar* mp) //FIX THIS LATER
{
    if (getWantVortex() == true) {
        int dir = mp->getWalkingDirection();
        switch (dir)
        {
        case GraphObject::right:
            m_Actors.push_back(new Vortex(IID_VORTEX, (mp->getX() + 16) / 16, (mp->getY())/16, GraphObject::right, 0, 1, this, GraphObject::right));
            break;
        case GraphObject::up:
            m_Actors.push_back(new Vortex(IID_VORTEX, (mp->getX()/16), (mp->getY()+16)/16, GraphObject::right, 0, 1, this, GraphObject::up));
            break;
        case GraphObject::down:
            m_Actors.push_back(new Vortex(IID_VORTEX, (mp->getX()/16), (mp->getY()-16)/16, GraphObject::right, 0, 1, this, GraphObject::down));
            break;
        case GraphObject::left:
            m_Actors.push_back(new Vortex(IID_VORTEX, (mp->getX() - 16 ) / 16, (mp->getY()/16), GraphObject::right, 0, 1, this, GraphObject::left));
            break;
        }
        setWantVortex(false);
        mp->setVortex(false);
    }
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->getAliveStatus() == true) {
            (*it)->doSomething();
        }
    }

    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); ) {
        Actor* chars = *it;
        if ((*it)->getAliveStatus() == false) {
            list<Actor*>::iterator del;
            del = it;
            delete chars;
            it++;
            m_Actors.erase(del);
        }
        else {
            it++;
        }
    }

    if (peach()->getVortex() == true) {
        ostringstream oss;
        oss << "P1 Roll: ";
        oss << peach()->getRoll() << " Stars: " << peach()->getStars() << " $$: " << peach()->getCoins()
            <<" VOR " << " | Time: " << timeRemaining() << " | Bank: " << getBankCoins() << " | P2 Roll: "
            << yoshi()->getRoll() << " Stars: " << yoshi()->getStars() << " $$: " << yoshi()->getCoins() << endl;
        string s = oss.str();
        setGameStatText(s);
    }
    else if (yoshi()->getVortex() == true) {
        ostringstream oss;
        oss << "P1 Roll: ";
        oss << peach()->getRoll() << " Stars: " << peach()->getStars() << " $$: " << peach()->getCoins()
            << " | Time: " << timeRemaining() << " | Bank: " << getBankCoins() << " | P2 Roll: "
            << yoshi()->getRoll() << " Stars: " << yoshi()->getStars() << " $$: " << yoshi()->getCoins() << " VOR" 
            << endl;
        string s = oss.str();
        setGameStatText(s);
    }
    else if (peach()->getVortex() == true && yoshi()->getVortex() == true) {
        ostringstream oss;
        oss << "P1 Roll: ";
        oss << peach()->getRoll() << " Stars: " << peach()->getStars() << " $$: " << peach()->getCoins()
            << " VOR " << " | Time: " << timeRemaining() << " | Bank: " << getBankCoins() << " | P2 Roll: "
            << yoshi()->getRoll() << " Stars: " << yoshi()->getStars() << " $$: " << yoshi()->getCoins() << " VOR"
            << endl;
        string s = oss.str();
        setGameStatText(s);
    }
    else if (peach()->getVortex() == false && yoshi()->getVortex() == false) {
        ostringstream oss;
        oss << "P1 Roll: ";
        oss << peach()->getRoll() << " Stars: " << peach()->getStars() << " $$: " << peach()->getCoins() 
            << " | Time: " << timeRemaining() << " | Bank: " << getBankCoins() << " | P2 Roll: "
            << yoshi()->getRoll() << " Stars: " << yoshi()->getStars() << " $$: " << yoshi()->getCoins() << endl;
        string s = oss.str();
        setGameStatText(s);
    }

    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        if (peach()->getStars() > yoshi()->getStars()) {
            setFinalScore(peach()->getStars(), peach()->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (yoshi()->getStars() > peach()->getStars()) {
            setFinalScore(yoshi()->getStars(), yoshi()->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        else {
            if (peach()->getCoins() > yoshi()->getCoins()) {
                setFinalScore(peach()->getStars(), peach()->getCoins());
                return GWSTATUS_PEACH_WON;
            }
            else if(yoshi()->getCoins() > peach()->getCoins()) {
                setFinalScore(yoshi()->getStars(), yoshi()->getCoins());
                return GWSTATUS_YOSHI_WON;
            }
            else {
                int decider = randInt(1, 2);
                if (decider == 1) {
                    setFinalScore(peach()->getStars(), peach()->getCoins());
                    return GWSTATUS_PEACH_WON;
                }
                else {
                    setFinalScore(yoshi()->getStars(), yoshi()->getCoins());
                    return GWSTATUS_YOSHI_WON;
                }
            }
        }
    }



    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); ) {
        list<Actor*>::iterator del;
        Actor* chars = *it;
        del = it;
        delete chars;
        it++;
        m_Actors.erase(del);
    }
}

bool StudentWorld::detectHit(Movers* mp)
{
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        Actor* trav = (*it);
        if (trav->getX() == mp->getX() && trav->getY() == mp->getY() && trav->isVortexable() == true) {
            return true;
        }
    }
    return false;
}

Actor* StudentWorld::hasBeenHit(Movers* mp)
{
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        Actor* trav = (*it);
        if (trav->getX() == mp->getX() && trav->getY() == mp->getY() && trav->isVortexable() == true) {
            return trav;
        }
    }
    return nullptr;
}

PlayerAvatar* StudentWorld::peach() {
    return m_Peach;
}

PlayerAvatar* StudentWorld::yoshi() {
    return m_Yoshi;
}

void StudentWorld::setBankCoins(int change)
{
    m_BankCoins += change;
}

int StudentWorld::getBankCoins()
{
    return m_BankCoins;
}

bool StudentWorld::isEmptySpace(int x, int y)
{
    Board::GridEntry ge = bd.getContentsOf(x/16, y/16);
    if (ge == Board::empty) {
        return true;
    }
    else {
        return false;
    }
}

bool StudentWorld::isDirSquare(int x, int y)
{
    Board::GridEntry ge = bd.getContentsOf(x / 16, y / 16);
    if (ge == Board::up_dir_square || ge == Board::down_dir_square ||
        ge == Board::right_dir_square || ge == Board::left_dir_square) {
        return true;
    }
    else {
        return false;
    }
}

void StudentWorld::createDropping(Movers* mp)
{
    list<Actor*>::iterator it;
    it = m_Actors.begin();
    while (it != m_Actors.end()) {
        Actor* finder = *it;
        if (finder->getX() == mp->getX() && finder->getY() == mp->getY() && finder->isASquare() == true) {
            finder->setAliveStatus(false);
            finder = new DroppingSquare(IID_DROPPING_SQUARE, mp->getX()/16, mp->getY()/16, GraphObject::right, this);
            m_Actors.push_back(finder);
            break;
        }
        else {
            it++;
        }
    }
}

Actor* StudentWorld::ObtainRandomSquare()
{
    list<Actor*> squares;
    int count = 0;
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        Actor* trav = (*it);
        if (trav->isASquare()) {
            squares.push_back(trav);
            count++;
        }
    }
    
    int rand = randInt(0, count);
    list<Actor*>::iterator it = squares.begin();
    for (int k = 0; k < rand; k++) {
        it++;
    }
    Actor* sq = *it;
    return sq;
}