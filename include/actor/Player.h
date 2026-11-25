// Player.h - the human player: stats, inventory and progress
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "items/Inventory.h"

using namespace std;

class Player {
private:
    string username;
    int health;
    int maxHealth;
    int attackPower;
    int defense;
    int coins;
    int score;
    int currentStage;      // 1-5 = stage in progress, 6 = all stages cleared
    int zombiesDefeated;
    int gamesPlayed;
    int totalDamageDealt;
    int playSeconds;
    Inventory inventory;
    vector<string> weaponsUsed;

public:
    Player();
    explicit Player(const string& username);

    // Getters
    string getUsername() const { return username; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getAttackPower() const { return attackPower; }
    int getDefense() const { return defense; }
    int getCoins() const { return coins; }
    int getScore() const { return score; }
    int getCurrentStage() const { return currentStage; }
    int getZombiesDefeated() const { return zombiesDefeated; }
    int getGamesPlayed() const { return gamesPlayed; }
    int getTotalDamageDealt() const { return totalDamageDealt; }
    int getPlaySeconds() const { return playSeconds; }
    int getStagesCompleted() const { return currentStage - 1; }
    const vector<string>& getWeaponsUsed() const { return weaponsUsed; }
    Inventory& getInventory() { return inventory; }

    // Actions
    int attack(Weapon* weapon);           // returns damage dealt (weapon may be nullptr)
    void takeDamage(int amount);
    void heal(int amount);
    void addScore(int points);
    void addCoins(int amount) { coins += amount; }
    bool spendCoins(int amount);
    void increaseDefense(int amount) { defense += amount; }
    void recordKill() { ++zombiesDefeated; }
    void advanceStage() { ++currentStage; }
    void addPlaytime(int seconds) { playSeconds += seconds; }
    void incrementGamesPlayed() { ++gamesPlayed; }
    bool isAlive() const { return health > 0; }
    void resetProgress();                 // new run, keeps username and games played

    // File support
    string toFileString() const;
    static bool fromFileString(const string& line, Player& out);

private:
    void recordWeaponUsed(const string& name);
};

#endif
