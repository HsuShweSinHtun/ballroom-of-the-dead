#include "items/Inventory.h"
#include "utilities/Utils.h"

using namespace std;

Inventory::Inventory() : medkits(0) {}

void Inventory::clear() {
    weapons.clear();
    medkits = 0;
}

void Inventory::addWeapon(const Weapon& weapon) { weapons.push_back(weapon); }

bool Inventory::hasWeapon(const string& name) const {
    for (const Weapon& w : weapons)
        if (w.getName() == name) return true;
    return false;
}

Weapon* Inventory::getWeapon(int index) {
    if (index < 0 || index >= getWeaponCount()) return nullptr;
    return &weapons[index];
}

int Inventory::getUsableWeaponCount() const {
    int count = 0;
    for (const Weapon& w : weapons)
        if (!w.isBroken()) ++count;
    return count;
}

bool Inventory::useMedkit() {
    if (medkits <= 0) return false;
    --medkits;
    return true;
}

string Inventory::toSaveString() const {
    vector<string> parts;
    for (const Weapon& w : weapons) parts.push_back(w.toSaveString());
    return Utils::join(parts, ";");
}

void Inventory::loadFromSaveString(const string& text, int medkitCount) {
    weapons.clear();
    medkits = medkitCount;
    if (text.empty()) return;
    for (const string& piece : Utils::split(text, ';')) {
        Weapon w;
        if (Weapon::fromSaveString(piece, w)) weapons.push_back(w);
    }
}
