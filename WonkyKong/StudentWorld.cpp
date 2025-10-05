#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

int numLives = 3;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath), lev(assetPath), player(nullptr)
{
	levelCounter = 0;
}

// Load the level
int StudentWorld::loadLevel() {
	ostringstream oss;
	oss << levelCounter;
	string curLevel = "level0" + oss.str() +".txt";
	Level::LoadResult result = lev.loadLevel(curLevel);
	if (result == Level::load_fail_file_not_found ||
		result == Level::load_fail_bad_format)
	{
		cout << "Something bad happened\n";
		return GWSTATUS_LEVEL_ERROR;
	}

	// Add actors to the game
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			Level::MazeEntry item = lev.getContentsOf(j, i);
			switch (item) {
			case Level::empty:
				break;
			case Level::player:
				player = new Player(IID_PLAYER, j, i, this);
				actorList.push_back(player);
				break;
			case Level::left_kong:
				actorList.push_back(new Kong(IID_KONG, j, i, GraphObject::left, this));
				break;
			case Level::right_kong:
				actorList.push_back(new Kong(IID_KONG, j, i, GraphObject::right, this));
				break;
			case Level::floor:
				actorList.push_back(new Floor(IID_FLOOR, j, i, this));
				break;
			case Level::ladder:
				actorList.push_back(new Ladder(IID_LADDER, j, i, this));
				break;
			case Level::bonfire:
				actorList.push_back(new Bonfire(IID_BONFIRE, j, i, this));
				break;
			case Level::fireball:
				actorList.push_back(new Fireball(IID_FIREBALL, j, i, this));
				break;
			case Level::koopa:
				actorList.push_back(new Koopa(IID_KOOPA, j, i, this));
				break;
			case Level::extra_life:
				actorList.push_back(new ExtraLifeGoodie(IID_EXTRA_LIFE_GOODIE, j, i, this));
				break;
			case Level::garlic:
				actorList.push_back(new GarlicGoodie(IID_GARLIC_GOODIE, j, i, this));
				break;
			default:
				break;
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::init()
{
	return loadLevel();
}

int StudentWorld::move()
{
	// Update the game status text
	ostringstream score, level, lives, burps;
	score << setw(7) << setfill('0') << getScore();
	level << setw(2) << setfill('0') << getLevel();
	lives << setw(2) << setfill('0') << getLives();
	burps << setw(2) << setfill('0') << getPlayer()->getBurps();
	setGameStatText("Score: " + score.str() + "  Level: " + level.str() + "  Lives: " + lives.str() + "  Burps: " + burps.str());

	// Do something for each actor
	for (list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++) {
		(*it)->doSomething();
	}
	// Remove dead objects
	for (list<Actor*>::iterator it = actorList.begin(); it != actorList.end();) {
		if ((*it)->isDead()) {
			delete* it;
			it = actorList.erase(it);
		}
		else {
			it++;
		}
	}
	// If player gains a life
	if (getLives() > numLives) {
		numLives++;
		return GWSTATUS_CONTINUE_GAME;
	}
	// If player loses a life
	if (getLives() < numLives) {
		numLives--;
		return GWSTATUS_PLAYER_DIED;
	}
	// If player completes all levels
	if (levelCounter == 3) {
		return GWSTATUS_PLAYER_WON;
	}
	// If player completes a level
	if (levelCounter != 0 && getLevel() != levelCounter) {
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

Player* StudentWorld::getPlayer() {
	return player;
}

void StudentWorld::cleanUp()
{
	// Delete all actors
	for (list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++) {
		delete* it;
	}
	actorList.clear();
	player = nullptr;
}
