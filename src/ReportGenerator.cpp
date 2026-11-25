#include "utilities/ReportGenerator.h"
#include "utilities/Utils.h"

#include <algorithm>
#include <functional>
#include <sstream>

ReportGenerator::ReportGenerator(FileManager& fm) : fileManager(fm) {}

string ReportGenerator::generateReport(const Player& player, bool won, int durationSeconds,
                                       const string& date) const {
    ostringstream r;
    string weapons = player.getWeaponsUsed().empty()
                         ? "None (bare hands)"
                         : Utils::join(player.getWeaponsUsed(), ", ");

    r << "====================================\n"
      << "GAME REPORT\n"
      << "Date: " << date << "\n"
      << "Player: " << player.getUsername() << "\n"
      << "Status: " << (won ? "WIN" : "LOSE") << "\n"
      << "Zombies Defeated: " << player.getZombiesDefeated() << "\n"
      << "Stages Completed: " << player.getStagesCompleted() << " / 5\n"
      << "Weapons Used: " << weapons << "\n"
      << "Total Damage Dealt: " << player.getTotalDamageDealt() << "\n"
      << "Final Health: " << player.getHealth() << " / " << player.getMaxHealth() << "\n"
      << "Total Score: " << player.getScore() << "\n"
      << "Game Duration: " << Utils::formatDuration(durationSeconds) << "\n"
      << "====================================\n";
    return r.str();
}

void ReportGenerator::saveReport(const string& report) { fileManager.appendReport(report); }

void ReportGenerator::updateLeaderboard(const Player& player) {
    vector<LeaderboardEntry> entries = fileManager.loadLeaderboard();

    bool found = false;
    for (LeaderboardEntry& e : entries) {
        if (Utils::toLower(e.username) == Utils::toLower(player.getUsername())) {
            if (player.getScore() > e.score) e.score = player.getScore();  // keep best score
            found = true;
        }
    }
    if (!found) entries.push_back({player.getUsername(), player.getScore()});

    sort(entries.begin(), entries.end(), greater<LeaderboardEntry>());
    fileManager.saveLeaderboard(entries);
}

vector<LeaderboardEntry> ReportGenerator::getTopPlayers(int topN) const {
    vector<LeaderboardEntry> entries = fileManager.loadLeaderboard();
    sort(entries.begin(), entries.end(), greater<LeaderboardEntry>());
    if (static_cast<int>(entries.size()) > topN) entries.resize(static_cast<size_t>(topN));
    return entries;
}
