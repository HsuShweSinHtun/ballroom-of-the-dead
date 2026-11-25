#include "GameData.h"
#include "ui/Colors.h"

GameData::GameData()
    : fileManager("data/"), reporter(fileManager), sessionStart(0), noticeColor(WHITE) {}

void GameData::Init() {
    fileManager.ensureFiles();
    catalog = fileManager.loadWeapons();
    if (catalog.empty()) catalog.push_back(Weapon("Pistol", 8, 40, 0));
}

void GameData::StartSession() { sessionStart = time(nullptr); }

void GameData::UpdatePlaytime() {
    time_t now = time(nullptr);
    player.addPlaytime(static_cast<int>(now - sessionStart));
    sessionStart = now;
}

void GameData::SaveProgress() {
    UpdatePlaytime();
    fileManager.savePlayer(player);
}

void GameData::GiveStarterWeapon() { player.getInventory().addWeapon(catalog[0]); }

void GameData::SetNotice(const string& text, int color) {
    notice = text;
    noticeColor = color;
}
