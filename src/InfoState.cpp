#include "states/InfoState.h"
#include "GameData.h"
#include "inputs/Keyboard.h"
#include "inputs/Keys.h"
#include "ui/Console.h"
#include "utilities/Utils.h"

#include <sstream>

InfoState::InfoState(StateContext& context, const string& title, const vector<InfoLine>& lines,
                     bool startAtEnd)
    : m_Context(context), m_Title(title), m_Lines(lines), m_Scroll(0), m_VisibleRows(22) {
    if (startAtEnd) m_Scroll = MaxScroll();
}

int InfoState::MaxScroll() const {
    return max(0, static_cast<int>(m_Lines.size()) - m_VisibleRows);
}

void InfoState::OnEnter() {}
void InfoState::OnExit() {}
void InfoState::Update() {}

void InfoState::ProcessInputs() {
    Keyboard& kb = m_Context.keyboard;
    if (kb.IsKeyJustPressed(KEY_ESCAPE) || kb.IsKeyJustPressed(KEY_BACKSPACE) ||
        kb.IsKeyJustPressed(KEY_ENTER)) {
        m_Context.stateMachine.PopState();
        return;
    }
    if (kb.IsKeyJustPressed(KEY_UP) || kb.IsKeyJustPressed(KEY_W))
        m_Scroll = max(0, m_Scroll - 1);
    if (kb.IsKeyJustPressed(KEY_DOWN) || kb.IsKeyJustPressed(KEY_S))
        m_Scroll = min(MaxScroll(), m_Scroll + 1);
}

void InfoState::Draw() {
    Console& c = m_Context.console;
    c.DrawHeader(m_Title);

    int y = 4;
    for (int i = m_Scroll; i < static_cast<int>(m_Lines.size()) && i < m_Scroll + m_VisibleRows;
         ++i, ++y)
        c.Write(6, y, m_Lines[i].text, m_Lines[i].color);

    if (m_Scroll > 0) c.Write(Console::WIDTH - 12, 3, "^ more ^", GRAY);
    if (m_Scroll < MaxScroll()) c.Write(Console::WIDTH - 12, 26, "v more v", GRAY);

    c.DrawPanelHorz(1, 27, Console::WIDTH - 2, BLUE);
    c.WriteCentered(28, "[Up/Down] Scroll    [Esc / Enter] Back", GRAY);
}

// ---------------------------------------------------------------------------

StatePtr InfoState::CreateLeaderboard(StateContext& context) {
    vector<InfoLine> lines;
    vector<LeaderboardEntry> top = context.gameData.reporter.getTopPlayers(10);

    lines.push_back({"", WHITE});
    if (top.empty()) {
        lines.push_back({"No scores yet. Be the first to escape the hotel!", GRAY});
    } else {
        lines.push_back({Utils::padRight("RANK", 8) + Utils::padRight("PLAYER", 24) + "BEST SCORE",
                         LIGHT_AQUA});
        lines.push_back({string(50, '-'), BLUE});
        for (size_t i = 0; i < top.size(); ++i) {
            int color = (i == 0) ? LIGHT_YELLOW : (i == 1) ? BRIGHT_WHITE : (i == 2) ? YELLOW : WHITE;
            lines.push_back({Utils::padRight("#" + to_string(i + 1), 8) +
                                 Utils::padRight(top[i].username, 24) + to_string(top[i].score),
                             color});
        }
    }
    return make_unique<InfoState>(context, "~ LEADERBOARD ~", lines);
}

StatePtr InfoState::CreateReports(StateContext& context) {
    vector<InfoLine> lines;
    string text = context.gameData.fileManager.readReports();

    if (text.empty()) {
        lines.push_back({"", WHITE});
        lines.push_back({"No reports yet. Finish a game to create one.", GRAY});
        return make_unique<InfoState>(context, "~ PAST REPORTS ~", lines);
    }

    istringstream stream(text);
    string line;
    while (getline(stream, line)) {
        int color = WHITE;
        if (line.rfind("=====", 0) == 0)
            color = BLUE;
        else if (line == "GAME REPORT")
            color = LIGHT_YELLOW;
        else if (line.rfind("Status: WIN", 0) == 0)
            color = LIGHT_GREEN;
        else if (line.rfind("Status: LOSE", 0) == 0)
            color = LIGHT_RED;
        lines.push_back({line, color});
    }
    return make_unique<InfoState>(context, "~ PAST REPORTS (newest at the bottom) ~", lines, true);
}

StatePtr InfoState::CreateHowToPlay(StateContext& context) {
    vector<InfoLine> lines = {
        {"", WHITE},
        {"THE STORY", LIGHT_YELLOW},
        {"You are trapped in an abandoned grand hotel full of zombies.", WHITE},
        {"Fight through 5 rooms and defeat the Ballroom King to escape.", WHITE},
        {"", WHITE},
        {"CONTROLS", LIGHT_YELLOW},
        {"Up / Down (or W / S)   Move the cursor", WHITE},
        {"Left / Right (or A / D) Move between columns", WHITE},
        {"Enter or Space          Choose", WHITE},
        {"Esc or Backspace        Go back", WHITE},
        {"", WHITE},
        {"COMBAT (turn-based)", LIGHT_YELLOW},
        {"Attack   Hit the zombie with a weapon. Weapons lose durability.", WHITE},
        {"Defend   Take half damage from the next zombie attack.", WHITE},
        {"Medkit   Restore 30 health (uses your turn).", WHITE},
        {"Run Away 50% chance to escape. Costs 20 score. Not possible vs the boss.", WHITE},
        {"", WHITE},
        {"SURVIVING", LIGHT_YELLOW},
        {"Kills give score and coins. Spend coins in the Shop on weapons, medkits,", WHITE},
        {"armor and repairs. Progress is saved after every stage.", WHITE},
        {"If your health reaches 0, the game is over.", WHITE},
    };
    return make_unique<InfoState>(context, "~ HOW TO PLAY ~", lines);
}
