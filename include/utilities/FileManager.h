// FileManager.h - all reading and writing of the .txt data files
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "actor/Player.h"
#include "items/Weapon.h"

using namespace std;

struct LeaderboardEntry {
    string username;
    int score;
    // Operator overloading: lets us sort entries from highest to lowest score
    bool operator>(const LeaderboardEntry& other) const { return score > other.score; }
};

class FileManager {
private:
    string dataDir;

    string path(const string& fileName) const { return dataDir + fileName; }

public:
    explicit FileManager(const string& directory = "data/");

    void ensureFiles();  // creates the data folder and any missing files

    // players.txt
    bool playerExists(const string& username) const;
    bool loadPlayer(const string& username, Player& out) const;  // search + read
    void savePlayer(const Player& player);                            // update or add

    // weapons.txt
    vector<Weapon> loadWeapons() const;

    // game_history.txt and reports.txt (append only)
    void appendHistory(const string& line);
    void appendReport(const string& text);
    string readReports() const;

    // leaderboard.txt
    vector<LeaderboardEntry> loadLeaderboard() const;
    void saveLeaderboard(const vector<LeaderboardEntry>& entries);
};

#endif
