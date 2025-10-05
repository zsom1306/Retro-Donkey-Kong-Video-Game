#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld() { cleanUp(); };
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	int loadLevel();
	Level getLevelObject() { return lev; };
	Player* getPlayer();
	int incrementLevelCounter() { return levelCounter++; };
	std::list<Actor*>& getActorList() { return actorList; };

private:
	std::list<Actor*> actorList; // Creates list of pointers to actors to store the objects
	int levelCounter; // Counts the number of levels completed
	Player* player; // Pointer to player object
	Level lev; // Level object
};

#endif // STUDENTWORLD_H_