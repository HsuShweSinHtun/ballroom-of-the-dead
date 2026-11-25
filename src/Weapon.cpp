#include "items/Weapon.h"
#include "utilities/Utils.h"

using namespace std;

Weapon::Weapon() : name("Bare Hands"), damage(0), durability(0), maxDurability(0), price(0) {}

Weapon::Weapon(const string& n, int d, int dur, int p)
    : name(n), damage(d), durability(dur), maxDurability(dur), price(p) {}

int Weapon::use() {
    if (durability <= 0) return 0;
    --durability;
    return damage;
}

bool Weapon::isBroken() const { return durability <= 0; }

bool Weapon::needsRepair() const { return durability < maxDurability; }

void Weapon::repair() { durability = maxDurability; }

string Weapon::toCatalogString() const {
    return name + "|" + to_string(damage) + "|" + to_string(maxDurability) + "|" +
           to_string(price);
}

string Weapon::toSaveString() const {
    return name + "," + to_string(damage) + "," + to_string(durability) + "," +
           to_string(maxDurability) + "," + to_string(price);
}

bool Weapon::fromSaveString(const string& text, Weapon& out) {
    vector<string> f = Utils::split(text, ',');
    if (f.size() < 5) return false;
    out.name = f[0];
    out.damage = Utils::toInt(f[1]);
    out.durability = Utils::toInt(f[2]);
    out.maxDurability = Utils::toInt(f[3]);
    out.price = Utils::toInt(f[4]);
    return true;
}
