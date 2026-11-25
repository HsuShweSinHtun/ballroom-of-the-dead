// GameRules.h - game constants and the damage / score calculations
#pragma once

#include <algorithm>
#include <string>
using namespace std;

namespace GameRules {

constexpr int TOTAL_STAGES = 5;
constexpr int MEDKIT_HEAL = 30;
constexpr int MEDKIT_PRICE = 25;
constexpr int ARMOR_PRICE = 40;
constexpr int REPAIR_PRICE = 15;
constexpr int FLEE_PENALTY = 20;
constexpr int WIN_BONUS = 100;

inline string StageName(int stage) {
    static const char* names[TOTAL_STAGES] = {"The Lobby", "The Dance Floor", "The Kitchen",
                                              "The Balcony", "The Grand Ballroom"};
    if (stage < 1 || stage > TOTAL_STAGES) return "Unknown";
    return names[stage - 1];
}

inline string StageIntro(int stage) {
    static const char* intros[TOTAL_STAGES] = {
        "Dusty chandeliers sway over the reception desk. Something shuffles in the dark.",
        "Broken records spin on the polished floor. The dead still seem to be dancing.",
        "Meat hooks swing and pans clatter. Fast footsteps echo between the stoves.",
        "Cold wind howls across the balcony. The horde presses in from every side.",
        "The grand doors swing open. A crowned figure waits in the middle of the floor."};
    if (stage < 1 || stage > TOTAL_STAGES) return "";
    return intros[stage - 1];
}

// Damage a zombie does to the player (defense lowers it, defending halves it)
inline int CalculateZombieDamage(int rawDamage, int defense, bool defending) {
    int damage = max(1, rawDamage - defense);
    if (defending) damage = max(1, damage / 2);
    return damage;
}

// Bonus added to the score when the game ends
inline int CalculateFinalBonus(int remainingHealth, bool won) {
    return remainingHealth / 2 + (won ? WIN_BONUS : 0);
}

}  // namespace GameRules
