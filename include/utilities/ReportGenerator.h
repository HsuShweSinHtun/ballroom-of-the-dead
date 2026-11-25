// ReportGenerator.h - end-of-game report and leaderboard data
#pragma once

#include <string>
#include <vector>
#include "actor/Player.h"
#include "utilities/FileManager.h"
using namespace std;

class ReportGenerator {
private:
    FileManager& fileManager;

public:
    explicit ReportGenerator(FileManager& fm);

    string generateReport(const Player& player, bool won, int durationSeconds,
                          const string& date) const;
    void saveReport(const string& report);
    void updateLeaderboard(const Player& player);
    vector<LeaderboardEntry> getTopPlayers(int topN) const;  // sorted, highest score first
};
