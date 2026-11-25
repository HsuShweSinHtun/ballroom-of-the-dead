// Inventory.h - the weapons and medkits the player is carrying
#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include "items/Weapon.h"

using namespace std;

class Inventory {
private:
    vector<Weapon> weapons;
    int medkits;

public:
    Inventory();

    void clear();
    void addWeapon(const Weapon& weapon);
    bool hasWeapon(const string& name) const;
    Weapon* getWeapon(int index);
    int getWeaponCount() const { return static_cast<int>(weapons.size()); }
    int getUsableWeaponCount() const;

    int getMedkits() const { return medkits; }
    void addMedkits(int amount) { medkits += amount; }
    bool useMedkit();

    string toSaveString() const;
    void loadFromSaveString(const string& text, int medkitCount);
};

#endif
