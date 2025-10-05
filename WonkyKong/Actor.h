#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Actor Base Class
class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, int startDir, StudentWorld* sw) 
		: GraphObject(imageID, startX, startY, startDir), m_world(sw), isPlayerFrozen(false), frozenTimer(0), dead(false) {};
	
	virtual void doSomething() = 0; // Pure virtual doSomething function
	virtual bool isDead() { return dead; } // Is the Actor dead?
	virtual void setDead(bool state) { dead = state; } // Set the Actor to dead
	virtual bool canBeBlasted() { return false; } // Can the Actor be blasted?

	StudentWorld* getWorld() { return m_world; }; // Returns the StudentWorld object
	void reverseHorizontalDirection(); // Reverse the horizontal direction of the Actor
	void jumpSequence(int direction); // Jump sequence for player
	bool& getFrozen() { return isPlayerFrozen; } // Is the player frozen?
	int& frozeTimer() { return frozenTimer; } // Frozen timer

private:
	StudentWorld* m_world;
	bool isPlayerFrozen;
	int frozenTimer;
	bool dead;
};

// Enemy Base Class
class Enemy : public Actor {
public:
	Enemy(int imageID, int startX, int startY, int direction, StudentWorld* sw) : Actor(imageID, startX, startY, direction, sw) {};
	virtual void doSomething() = 0;
	virtual bool canBeBlasted() { return true; }
};

// Goodie Base Class
class Goodie : public Actor {
public:
	Goodie(int imageID, int startX, int startY, StudentWorld* sw) : Actor(imageID, startX, startY, GraphObject::none, sw) {};
	virtual void doSomething();
	virtual void conferPowerUp() = 0;
};

class Player : public Actor {
public:
	Player(int imageID, int startX, int startY, StudentWorld* sw) : Actor(imageID, startX, startY, GraphObject::right, sw) { burps = 0; };
	virtual void doSomething();
	void addBurps(int amt) { burps += amt; }
	int getBurps() { return burps; }
private:
	int burps;
};

class Kong : public Actor {
public:
	Kong(int imageID, int startX, int startY, int direction, StudentWorld* sw) : Actor(imageID, startX, startY, direction, sw), direction(direction) { numTicksPassed = 0; fleeState = false; fleeTicks = 0; };
	virtual void doSomething();
private:
	int direction;
	int numTicksPassed;
	int fleeState;
	int fleeTicks;
};

class Barrel : public Enemy {
public:
	Barrel(int imageID, int startX, int startY, int direction, StudentWorld* sw) : Enemy(imageID, startX, startY, direction, sw), direction(direction) { barrelTicks = 0; };
	virtual void doSomething();
private:
	int direction;
	int barrelTicks;
};

class Floor : public Actor {
public:
	Floor(int imageID, int startX, int startY, StudentWorld* sw) : Actor(imageID, startX, startY, GraphObject::none, sw) {};
	virtual void doSomething() {};
};

class Ladder : public Actor {
public:
	Ladder(int imageID, int startX, int startY, StudentWorld* sw) : Actor(imageID, startX, startY, GraphObject::none, sw) {};
	virtual void doSomething() {};
};

class Bonfire : public Actor {
public:
	Bonfire(int imageID, int startX, int startY, StudentWorld* sw) : Actor(imageID, startX, startY, GraphObject::none, sw) {};
	virtual void doSomething();
};

class Fireball : public Enemy {
public:
	Fireball(int imageID, int startX, int startY, StudentWorld* sw);
	virtual void doSomething();
private:
	int fireballTicks;
	bool climbingUp;
	bool climbingDown;
};

class Koopa : public Enemy {
public:
	Koopa(int imageID, int startX, int startY, StudentWorld* sw);
	virtual void doSomething();
private:
	int moveTimer;
};

class ExtraLifeGoodie : public Goodie {
public:
	ExtraLifeGoodie(int imageID, int startX, int startY, StudentWorld* sw) : Goodie(imageID, startX, startY, sw) {};
	virtual void conferPowerUp();
};

class GarlicGoodie : public Goodie {
public:
	GarlicGoodie(int imageID, int startX, int startY, StudentWorld* sw) : Goodie(imageID, startX, startY, sw) {};
	virtual void conferPowerUp();
public:
};

class Burp : public Actor {
public:
	Burp(int imageID, int startX, int startY, int direction, StudentWorld* sw) : Actor(imageID, startX, startY, direction, sw), burpTicks(5) {};
	virtual void doSomething();
private:
	int burpTicks;
};

#endif // ACTOR_H_
