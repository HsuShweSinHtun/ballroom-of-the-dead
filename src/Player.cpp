#include "actor/Player.h"
#include "utilities/Utils.h"
#include <algorithm>

using namespace std;

Player::Player() : username(""), gamesPlayed(0) { resetProgress(); }

Player::Player(const string& name) : username(name), gamesPlayed(0) { resetProgress(); }

void Player::resetProgress() {
    health = 100;
    maxHealth = 100;
    attackPower = 10;
    defense = 2;
    coins = 20;
    score = 0;
    currentStage = 1;
    zombiesDefeated = 0;
    totalDamageDealt = 0;
    playSeconds = 0;
    inventory.clear();
    inventory.addMedkits(1);
    weaponsUsed.clear();
}

int Player::attack(Weapon* weapon) {
    int damage;
    if (weapon != nullptr && !weapon->isBroken()) {
        damage = attackPower + weapon->use() + Utils::randomInt(0, 5);
        recordWeaponUsed(weapon->getName());
    } else {
        // Bare hands are weak
        damage = max(2, attackPower / 3) + Utils::randomInt(0, 2);
    }
    totalDamageDealt += damage;
    return damage;
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void Player::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

void Player::addScore(int points) {
    score += points;
    if (score < 0) score = 0;
}

bool Player::spendCoins(int amount) {
    if (coins < amount) return false;
    coins -= amount;
    return true;
}

void Player::recordWeaponUsed(const string& name) {
    if (find(weaponsUsed.begin(), weaponsUsed.end(), name) == weaponsUsed.end())
        weaponsUsed.push_back(name);
}

// players.txt line:
// username|health|maxHealth|attack|defense|coins|score|stage|zombies|games|medkits|damage|seconds|weapons|used
string Player::toFileString() const {
    return username + "|" + to_string(health) + "|" + to_string(maxHealth) + "|" +
           to_string(attackPower) + "|" + to_string(defense) + "|" +
           to_string(coins) + "|" + to_string(score) + "|" +
           to_string(currentStage) + "|" + to_string(zombiesDefeated) + "|" +
           to_string(gamesPlayed) + "|" + to_string(inventory.getMedkits()) + "|" +
           to_string(totalDamageDealt) + "|" + to_string(playSeconds) + "|" +
           inventory.toSaveString() + "|" + Utils::join(weaponsUsed, ",");
}

bool Player::fromFileString(const string& line, Player& out) {
    vector<string> f = Utils::split(line, '|');
    if (f.size() < 15) return false;
    out.username = f[0];
    out.health = Utils::toInt(f[1], 100);
    out.maxHealth = Utils::toInt(f[2], 100);
    out.attackPower = Utils::toInt(f[3], 10);
    out.defense = Utils::toInt(f[4], 2);
    out.coins = Utils::toInt(f[5]);
    out.score = Utils::toInt(f[6]);
    out.currentStage = Utils::toInt(f[7], 1);
    out.zombiesDefeated = Utils::toInt(f[8]);
    out.gamesPlayed = Utils::toInt(f[9]);
    out.totalDamageDealt = Utils::toInt(f[11]);
    out.playSeconds = Utils::toInt(f[12]);
    out.inventory.loadFromSaveString(f[13], Utils::toInt(f[10]));
    out.weaponsUsed.clear();
    if (!f[14].empty()) out.weaponsUsed = Utils::split(f[14], ',');
    return true;
}
