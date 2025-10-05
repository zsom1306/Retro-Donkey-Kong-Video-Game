#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
#include <ctime>

// Reverse the horizontal direction of the Actor
void Actor::reverseHorizontalDirection() {
	if (getDirection() == GraphObject::left) {
		setDirection(GraphObject::right);
	}
	else {
		setDirection(GraphObject::left);
	}
}

// Jump sequence for player
bool isJumping = false;
int jumpCounter = 0;
void Actor::jumpSequence(int direction) {
	if (getWorld()->getLevelObject().getContentsOf(getX(), getY()) == Level::ladder && jumpCounter > 0) { // If the player jumped onto a ladder mid jump, end jump sequence.
		isJumping = false;
		jumpCounter = 0;
		return;
	}
	if (direction == GraphObject::left && jumpCounter < 3) { // If the player is jumping left, move left.
		if (getWorld()->getLevelObject().getContentsOf(getX() - 1, getY()) != Level::floor) {
			moveTo(getX() - 1, getY());
			jumpCounter++;
			return;
		}
	}
	if (direction == GraphObject::right && jumpCounter < 3) { // If the player is jumping right, move right.
		if (getWorld()->getLevelObject().getContentsOf(getX() + 1, getY()) != Level::floor) {
			moveTo(getX() + 1, getY());
			jumpCounter++;
			return;
		}
	}
	if (getWorld()->getLevelObject().getContentsOf(getX(), getY() - 1) != Level::floor) { // Once jumpcounter reaches 3, move down
		moveTo(getX(), getY() - 1);
		isJumping = false; // End jump sequence
		jumpCounter = 0;
		return;
	}
	isJumping = false; // End jump sequence
	jumpCounter = 0;
	return;
}

// Player's doSomething function
void Player::doSomething() {
	// Runs into bonfire
	if (getWorld()->getLevelObject().getContentsOf(getX(), getY()) == Level::bonfire) {
		getWorld()->decLives();
		getWorld()->playSound(SOUND_PLAYER_DIE);
	}
	// Player is frozen
	if (getFrozen() == true)
	{
		if (frozeTimer() != 0) {
			frozeTimer()--;
			return;
		}
		else {
			getFrozen() = false;
			frozeTimer() = 50;
			return;
		}
	}
	// Currently jumping
	if(isJumping) {
		jumpSequence(getDirection());
		return;
	}
	// Gravity
	if (getWorld()->getLevelObject().getContentsOf(getX(), getY() - 1) != Level::floor && getWorld()->getLevelObject().getContentsOf(getX(), getY() - 1) != Level::ladder) {
		if (getWorld()->getLevelObject().getContentsOf(getX(), getY()) != Level::ladder) { // If currently on a ladder, do not fall.
			moveTo(getX(), getY() - 1);
			return;
		}
	}

	// Key press
	int key;
	if (getWorld()->getKey(key)) {
		switch (key) {
		case KEY_PRESS_LEFT:
			if (getDirection() != GraphObject::left) {
				reverseHorizontalDirection();
				break;
			}
			if (getWorld()->getLevelObject().getContentsOf(getX() - 1, getY()) != Level::floor) {
				moveTo(getX() - 1, getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != GraphObject::right) {
				reverseHorizontalDirection();
				break;
			}
			if (getWorld()->getLevelObject().getContentsOf(getX() + 1, getY()) != Level::floor) {
				moveTo(getX() + 1, getY());
			}
			break;
		case KEY_PRESS_UP:
			if (getWorld()->getLevelObject().getContentsOf(getX(), getY()) == Level::ladder && getWorld()->getLevelObject().getContentsOf(getX(), getY() + 1) != Level::floor) {
				moveTo(getX(), getY() + 1);
			}
			break;
		case KEY_PRESS_DOWN:
			if ((getWorld()->getLevelObject().getContentsOf(getX(), getY()) == Level::ladder || getWorld()->getLevelObject().getContentsOf(getX(), getY() - 1) == Level::ladder) && getWorld()->getLevelObject().getContentsOf(getX(), getY() - 1) != Level::floor) {
				moveTo(getX(), getY() - 1);
			}
			break;
		case KEY_PRESS_SPACE:
			if ((getWorld()->getLevelObject().getContentsOf(getX(), getY() - 1) == Level::floor || getWorld()->getLevelObject().getContentsOf(getX(), getY() - 1) == Level::ladder) || getWorld()->getLevelObject().getContentsOf(getX(),getY()) == Level::ladder){
				if (getWorld()->getLevelObject().getContentsOf(getX(), getY() + 1) == Level::floor) {
					break;
				}
				if (getWorld()->getLevelObject().getContentsOf(getX(), getY() + 1) == Level::ladder) { // If the object above a player is a ladder, jump onto it.	
					moveTo(getX(), getY() + 1);
					getWorld()->playSound(SOUND_JUMP);
					break;
				}
				if (getDirection() == GraphObject::left) {
					if (getWorld()->getLevelObject().getContentsOf(getX(), getY() + 1) == Level::empty && getWorld()->getLevelObject().getContentsOf(getX() - 1, getY() + 1) == Level::floor) {
						moveTo(getX(), getY() + 1);
						getWorld()->playSound(SOUND_JUMP);
						break;
					}
				}
				else {
					if (getWorld()->getLevelObject().getContentsOf(getX(), getY() + 1) == Level::empty && getWorld()->getLevelObject().getContentsOf(getX() + 1, getY() + 1) == Level::floor) {
						moveTo(getX(), getY() + 1);
						getWorld()->playSound(SOUND_JUMP);
						break;
					}
				}
				moveTo(getX(), getY() + 1);
				getWorld()->playSound(SOUND_JUMP);
				isJumping = true; // Starting jump sequence
			}
			break;
		case KEY_PRESS_TAB:
			if (getBurps() > 0) {
				getWorld()->playSound(SOUND_BURP);
				if (getDirection() == GraphObject::left) {
					getWorld()->getActorList().push_back(new Burp(IID_BURP, getX() - 1, getY(), getDirection(), getWorld()));
				}
				else {
					getWorld()->getActorList().push_back(new Burp(IID_BURP, getX() + 1, getY(), getDirection(), getWorld()));
				}
				addBurps(-1);
			}
		}
	}
}

// Bonfire's doSomething function
void Bonfire::doSomething() {
	increaseAnimationNumber();
}

// Kong's doSomething function
void Kong::doSomething() {
	increaseAnimationNumber();

	// Calculate Euclidean distance between Kong and the player
	int playerX = getWorld()->getPlayer()->getX();
	int playerY = getWorld()->getPlayer()->getY();
	int euclideanDistance = sqrt(pow(getX() - playerX, 2) + pow(getY() - playerY, 2));

	// Check if the player is within the flee distance
	if (euclideanDistance <= 2 && isJumping == false) {
		fleeState = true;
	}

	// If Kong is in flee state
	if (fleeState) {
		if (fleeTicks == 5) {
			if (getWorld()->getLevelObject().getContentsOf(getX(), getY() + 1) == Level::empty) {
				moveTo(getX(), getY() + 1);
				getWorld()->playSound(SOUND_FINISHED_LEVEL);
				getWorld()->increaseScore(1000);
				getWorld()->incrementLevelCounter();
			}
			else if (getWorld()->getLevelObject().getContentsOf(getX(), getY() + 1) == Level::ladder) {
				moveTo(getX(), getY() + 1);
				fleeTicks = 0;
			}
		}
		else {
			fleeTicks++;
		}
		return;
	}
	
	// Barrel throwing 
	int ticksBetweenBarrels = 200 - 50 * getWorld()->getLevel();
	if (numTicksPassed == ticksBetweenBarrels) {
		if (direction == GraphObject::left) {
			getWorld()->getActorList().push_back(new Barrel(IID_BARREL, getX() - 1, getY(), direction, getWorld()));
		}
		else {
			getWorld()->getActorList().push_back(new Barrel(IID_BARREL, getX() + 1, getY(), direction, getWorld()));
		}
		numTicksPassed = 0;
	}
	numTicksPassed++;
}

// Barrel's doSomething function
void Barrel::doSomething() {
	// Check if the barrel is on a bonfire
	if (getWorld()->getLevelObject().getContentsOf(getX(), getY()) == Level::bonfire) {
		setDead(true);
		return;
	}
	Player* player = getWorld()->getPlayer();
	if (player && (getX() == player->getX()) && (getY() == player->getY())) {
		getWorld()->decLives();
		return;
	}

	// Barell movement
	if (getWorld()->getLevelObject().getContentsOf(getX(), getY() - 1) != Level::floor) {
		moveTo(getX(), getY() - 1);
		if (getWorld()->getLevelObject().getContentsOf(getX(), getY() - 1) == Level::floor) {
			if (direction == GraphObject::left) {
				direction = GraphObject::right;
			}
			else {
				direction = GraphObject::left;
			}
		}
		barrelTicks++;
		return;
	}

	if (barrelTicks >= 10) {
		if (direction == GraphObject::left) {
			if (getWorld()->getLevelObject().getContentsOf(getX() - 1, getY()) != Level::floor) {
				moveTo(getX() - 1, getY());
			}
			else {
				direction = GraphObject::right;
			}
		}
		else {
			if (getWorld()->getLevelObject().getContentsOf(getX() + 1, getY()) != Level::floor) {
				moveTo(getX() + 1, getY());
			}
			else {
				direction = GraphObject::left;
			}
		}
		barrelTicks = 0;
	}
	barrelTicks++;
}

// Kooppa Constructor
Koopa::Koopa(int imageID, int startX, int startY, StudentWorld* sw)
	: Enemy(imageID, startX, startY, (randInt(0,1) == 0 ? GraphObject::right : GraphObject::left), sw), moveTimer(10) {
}

// Kooppa's doSomething function
void Koopa::doSomething() {
	Player* player = getWorld()->getPlayer();
	if ((getX() == player->getX()) && (getY() == player->getY()) && frozeTimer() == 0) {
		player->getFrozen() = true;
		player->frozeTimer() = 50;
	}
	else if(frozeTimer() > 0) {
		frozeTimer()--;
	}

	if (moveTimer == 0) {
		if(getDirection() == GraphObject::right) {
			if (getWorld()->getLevelObject().getContentsOf(getX() + 1, getY()) != Level::floor && (getWorld()->getLevelObject().getContentsOf(getX() + 1, getY() - 1) == Level::floor || getWorld()->getLevelObject().getContentsOf(getX() + 1, getY() - 1) == Level::ladder)) {
				moveTo(getX() + 1, getY());
			}
			else {
				reverseHorizontalDirection();
			}
			moveTimer = 10;
		}
		else {
			if (getWorld()->getLevelObject().getContentsOf(getX() - 1, getY()) != Level::floor && (getWorld()->getLevelObject().getContentsOf(getX() - 1, getY() - 1) == Level::floor || getWorld()->getLevelObject().getContentsOf(getX() - 1, getY() - 1) == Level::ladder)) {
				moveTo(getX() - 1, getY());
			}
			else {
				reverseHorizontalDirection();
			}
			moveTimer = 10;
		}
	}
	else {
		moveTimer--;
	}
}

// Fireball Constructor
Fireball::Fireball(int imageID, int startX, int startY, StudentWorld* sw)
	: Enemy(imageID, startX, startY, (randInt(0, 1) == 0 ? GraphObject::right : GraphObject::left), sw), fireballTicks(0), climbingUp(false), climbingDown(false) {
}

// Fireball's doSomething function
void Fireball::doSomething() {
	Player* player = getWorld()->getPlayer();
	if (player && (getX() == player->getX()) && (getY() == player->getY())) {
		getWorld()->decLives();
		return;
	}
	fireballTicks++;
	if (fireballTicks % 10 == 0) {
		// Climbing up
		if (getWorld()->getLevelObject().getContentsOf(getX(), getY()) == Level::ladder && getWorld()->getLevelObject().getContentsOf(getX(), getY() + 1) != Level::floor && !climbingDown) {
			if (climbingUp || randInt(0, 2) == 0) {
				climbingUp = true;
				moveTo(getX(), getY() + 1);
				return;
			}
		}
		// Climbing down
		else if (getWorld()->getLevelObject().getContentsOf(getX(), getY() - 1) == Level::ladder && !climbingUp) {
			if (climbingDown || randInt(0, 2) == 0) {
				climbingDown = true;
				moveTo(getX(), getY() - 1);
				return;
			}
		}
		// Transition to non-climbing state
		if (climbingUp) {
			climbingUp = false;
		}
		if (climbingDown) {
			climbingDown = false;
		}
		// Horizontal movement
		if (getDirection() == GraphObject::right) {
			if (getWorld()->getLevelObject().getContentsOf(getX() + 1, getY()) == Level::floor || getWorld()->getLevelObject().getContentsOf(getX() + 1, getY() - 1) == Level::empty) {
				reverseHorizontalDirection();
			}
			else {
				moveTo(getX() + 1, getY());
			}
		}
		else {
			if (getWorld()->getLevelObject().getContentsOf(getX() - 1, getY()) == Level::floor || getWorld()->getLevelObject().getContentsOf(getX() - 1, getY() - 1) == Level::empty) {
				reverseHorizontalDirection();
			}
			else {
				moveTo(getX() - 1, getY());
			}
		}
		// Attack player
		if (player && (getX() == player->getX()) && (getY() == player->getY())) {
			getWorld()->decLives();
			return;
		}
	}
}

// Goodie's doSomething function
void Goodie::doSomething() {
	Player* player = getWorld()->getPlayer();
	if (player && (getX() == player->getX()) && (getY() == player->getY())) {
		conferPowerUp();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		setDead(true);
	}
}

// Garlic Goodie's specifc power up
void GarlicGoodie::conferPowerUp() {
	getWorld()->increaseScore(25);
	getWorld()->getPlayer()->addBurps(5);
}

// Extra Life Goodie's specific power up
void ExtraLifeGoodie::conferPowerUp() {
	getWorld()->increaseScore(50);
	getWorld()->incLives();
}

// Burp's doSomething function
void Burp::doSomething() {
	burpTicks--;
	if (burpTicks == 0) {
		setDead(true);
		return;
	}
	for (Actor* actor : getWorld()->getActorList()) {
		if (actor->getX() == getX() && actor->getY() == getY() && actor != this) {
			if (actor->canBeBlasted()) {
				// Play sound and increase score
				getWorld()->playSound(SOUND_ENEMY_DIE);
				getWorld()->increaseScore(100);

				// Drop goodies based on actor type
				if (dynamic_cast<Koopa*>(actor)) {
					if (randInt(0, 2) == 0) { // 1/3 chance
						getWorld()->getActorList().push_back(new ExtraLifeGoodie(IID_EXTRA_LIFE_GOODIE, actor->getX(), actor->getY(), getWorld()));
					}
				}
				else if (dynamic_cast<Fireball*>(actor)) {
					if (randInt(0, 2) == 0) { // 1/3 chance
						getWorld()->getActorList().push_back(new GarlicGoodie(IID_GARLIC_GOODIE, actor->getX(), actor->getY(), getWorld()));
					}
				}

				// Set actor to dead
				actor->setDead(true);
			}
		}
	}
}