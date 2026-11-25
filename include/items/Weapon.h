// Weapon.h - one weapon with damage and durability
#ifndef WEAPON_H
#define WEAPON_H

#include <string>

using namespace std;

class Weapon {
private:
    string name;
    int damage;
    int durability;
    int maxDurability;
    int price;

public:
    Weapon();
    Weapon(const string& name, int damage, int durability, int price);

    string getName() const { return name; }
    int getDamage() const { return damage; }
    int getDurability() const { return durability; }
    int getMaxDurability() const { return maxDurability; }
    int getPrice() const { return price; }

    int use();                    // uses the weapon once, returns its damage
    bool isBroken() const;
    bool needsRepair() const;
    void repair();

    string toCatalogString() const;  // name|damage|durability|price (weapons.txt)
    string toSaveString() const;     // name,damage,durability,max,price (players.txt)
    static bool fromSaveString(const string& text, Weapon& out);
};

#endif
