// Zombie.h - base Zombie class and the four zombie types (inheritance + polymorphism)
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <string>

using namespace std;

class Zombie {
protected:
    string type;
    int health;
    int maxHealth;
    int attackPower;
    int scoreValue;
    int coinReward;

public:
    // Stats are scaled up by the stage number so later stages are harder
    Zombie(const string& type, int baseHealth, int baseAttack, int baseScore,
           int baseCoins, int stage);
    virtual ~Zombie() {}

    // Returns the damage of this attack. 'message' describes what happened.
    virtual int attack(int turn, string& message);
    virtual bool canFlee() const { return true; }
    virtual string getIntro() const;

    void takeDamage(int amount);
    bool isAlive() const { return health > 0; }

    string getType() const { return type; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getAttackPower() const { return attackPower; }
    int getScoreValue() const { return scoreValue; }
    int getCoinReward() const { return coinReward; }
};

// Slow and weak
class Walker : public Zombie {
public:
    explicit Walker(int stage);
    string getIntro() const override;
};

// Fast: sometimes strikes twice in one turn
class Runner : public Zombie {
public:
    explicit Runner(int stage);
    int attack(int turn, string& message) override;
    string getIntro() const override;
};

// Lots of health and hits hard
class Brute : public Zombie {
public:
    explicit Brute(int stage);
    string getIntro() const override;
};

// Final boss: special move every third turn, cannot be escaped
class BallroomKing : public Zombie {
public:
    explicit BallroomKing(int stage);
    int attack(int turn, string& message) override;
    bool canFlee() const override { return false; }
    string getIntro() const override;
};

#endif
