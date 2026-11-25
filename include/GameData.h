// GameData.h - the data shared by all states: the player, the weapon list and the files
#pragma once

#include <ctime>
#include <string>
#include <vector>
#include "actor/Player.h"
#include "items/Weapon.h"
#include "utilities/FileManager.h"
#include "utilities/ReportGenerator.h"
using namespace std;

struct GameData {
    FileManager fileManager;
    ReportGenerator reporter;
    Player player;
    vector<Weapon> catalog;  // all weapons from weapons.txt
    time_t sessionStart;
    string notice;           // a short message shown on the menu screen
    int noticeColor;

    GameData();
    void Init();              // creates missing data files and loads the weapon list
    void StartSession();
    void UpdatePlaytime();
    void SaveProgress();
    void GiveStarterWeapon();
    void SetNotice(const string& text, int color);
};
