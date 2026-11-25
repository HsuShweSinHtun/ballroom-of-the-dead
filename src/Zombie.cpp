#include "actor/Zombie.h"
#include "utilities/Utils.h"

using namespace std;

namespace {
int scaled(int base, int stage) {
    return static_cast<int>(base * (1.0 + 0.08 * (stage - 1)));
}
}

Zombie::Zombie(const string& t, int baseHealth, int baseAttack, int baseScore,
               int baseCoins, int stage)
    : type(t),
      health(scaled(baseHealth, stage)),
      maxHealth(scaled(baseHealth, stage)),
      attackPower(scaled(baseAttack, stage)),
      scoreValue(baseScore),
      coinReward(baseCoins) {}

int Zombie::attack(int, string& message) {
    message = "The " + type + " claws at you!";
    return attackPower;
}

string Zombie::getIntro() const { return "A " + type + " shambles toward you."; }

void Zombie::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

// ----- Walker -----
Walker::Walker(int stage) : Zombie("Walker", 30, 6, 10, 5, stage) {}
string Walker::getIntro() const { return "A Walker drags itself across the floor..."; }

// ----- Runner -----
Runner::Runner(int stage) : Zombie("Runner", 40, 9, 20, 10, stage) {}

int Runner::attack(int, string& message) {
    if (Utils::randomInt(1, 100) <= 25) {
        message = "The Runner lunges at you TWICE!";
        return attackPower * 2;
    }
    message = "The Runner slashes at you!";
    return attackPower;
}

string Runner::getIntro() const { return "A Runner sprints out of the shadows!"; }

// ----- Brute -----
Brute::Brute(int stage) : Zombie("Brute", 70, 12, 40, 20, stage) {}
string Brute::getIntro() const { return "A huge Brute smashes through the door!"; }

// ----- BallroomKing (boss) -----
BallroomKing::BallroomKing(int stage) : Zombie("Ballroom King", 150, 14, 200, 100, stage) {}

int BallroomKing::attack(int turn, string& message) {
    if (turn % 3 == 0) {
        message = "The Ballroom King unleashes the WALTZ OF DEATH!";
        return attackPower * 7 / 4;
    }
    message = "The Ballroom King swings his rotting scepter!";
    return attackPower;
}

string BallroomKing::getIntro() const {
    return "The chandelier flickers... the Ballroom King rises from his throne!";
}
