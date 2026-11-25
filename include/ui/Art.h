// Art.h - ASCII art for the title screen and the zombies
#pragma once

#include <string>
#include <vector>
using namespace std;

namespace Art {

inline const vector<string> BALLROOM = {
    R"ART( ____    _    _     _     ____   ___   ___  __  __ )ART",
    R"ART(| __ )  / \  | |   | |   |  _ \ / _ \ / _ \|  \/  |)ART",
    R"ART(|  _ \ / _ \ | |   | |   | |_) | | | | | | | |\/| |)ART",
    R"ART(| |_) / ___ \| |___| |___|  _ <| |_| | |_| | |  | |)ART",
    R"ART(|____/_/   \_\_____|_____|_| \_\\___/ \___/|_|  |_|)ART"};
inline const vector<string> OF_THE_DEAD = {
    R"ART(  ___  ___   _____ _  _ ___   ___  ___   _   ___  )ART",
    R"ART( / _ \| __| |_   _| || | __| |   \| __| /_\ |   \ )ART",
    R"ART(| (_) | _|    | | | __ | _|  | |) | _| / _ \| |) |)ART",
    R"ART( \___/|_|     |_| |_||_|___| |___/|___/_/ \_\___/ )ART"};
inline const vector<string> VICTORY = {
    R"ART(__   _____ ___ _____ ___  _____   __)ART",
    R"ART(\ \ / /_ _/ __|_   _/ _ \| _ \ \ / /)ART",
    R"ART( \ V / | | (__  | || (_) |   /\ V / )ART",
    R"ART(  \_/ |___\___| |_| \___/|_|_\ |_|  )ART"};
inline const vector<string> GAME_OVER = {
    R"ART(  ___   _   __  __ ___    _____   _____ ___ )ART",
    R"ART( / __| /_\ |  \/  | __|  / _ \ \ / / __| _ \)ART",
    R"ART(| (_ |/ _ \| |\/| | _|  | (_) \ V /| _||   /)ART",
    R"ART( \___/_/ \_\_|  |_|___|  \___/ \_/ |___|_|_\)ART"};
inline const vector<string> CHANDELIER = {
    R"ART(                      _                      )ART",
    R"ART(                     (_)                     )ART",
    R"ART(                     |_|                     )ART",
    R"ART(                 .---'-'---.                 )ART",
    R"ART(         o   o  /  o  o  o  \  o   o         )ART",
    R"ART(        _|___|__|___|__|___|__|___|_         )ART",
    R"ART(       ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( )       )ART"};
inline const vector<string> WALKER = {
    R"ART(        ,-"""-.         )ART",
    R"ART(       / x   x \        )ART",
    R"ART(      |    __    |      )ART",
    R"ART(       \  \__/  /       )ART",
    R"ART(      __)-.__.-(__      )ART",
    R"ART(     /  |      |  \     )ART",
    R"ART(    / /\|      |/\ \    )ART",
    R"ART(   (_/  |      |  \_)   )ART",
    R"ART(        |  ||  |        )ART",
    R"ART(        |  ||  |        )ART",
    R"ART(       _|  ||  |_       )ART",
    R"ART(      (__)  (__)        )ART"};
inline const vector<string> RUNNER = {
    R"ART(          ,--.          )ART",
    R"ART(         ( o o)         )ART",
    R"ART(         |/--\|         )ART",
    R"ART(      _.-|    |-._      )ART",
    R"ART(     /   '.__.'   \     )ART",
    R"ART(    /  _/|    |\_  \    )ART",
    R"ART(   '__/  |    |  \__'   )ART",
    R"ART(        /|    |\        )ART",
    R"ART(       / |    | \       )ART",
    R"ART(      /  /\  /\  \      )ART",
    R"ART(     /__/  \/  \__\     )ART",
    R"ART(    (__)        (__)    )ART"};
inline const vector<string> BRUTE = {
    R"ART(        _________         )ART",
    R"ART(      /  ^     ^  \       )ART",
    R"ART(     |   O     O   |      )ART",
    R"ART(     |     ___     |      )ART",
    R"ART(     \    |VVV|    /      )ART",
    R"ART(  ____\___|___|___/____   )ART",
    R"ART( /  __ \         / __  \  )ART",
    R"ART(| [____]|  | | | |[____] |)ART",
    R"ART( \_____/|  | | | |\_____/ )ART",
    R"ART(        |  |_|_|  |       )ART",
    R"ART(       /  /     \  \      )ART",
    R"ART(      /__/       \__\     )ART"};
inline const vector<string> BALLROOM_KING = {
    R"ART(      \  /\  /\  /       )ART",
    R"ART(       \/  \/  \/        )ART",
    R"ART(      [===========]      )ART",
    R"ART(       /  .---.  \       )ART",
    R"ART(      |  ( @ @ )  |      )ART",
    R"ART(      |   \ - /   |      )ART",
    R"ART(    __\  /|||||\  /__    )ART",
    R"ART(   / /\\/ ||||| \//\ \   )ART",
    R"ART(  | |  | ||||| |  | |    )ART",
    R"ART(   \ \_/_|||||_\_/ /     )ART",
    R"ART(    \___/ ||||| \___/    )ART",
    R"ART(         /|   |\         )ART",
    R"ART(        /_/   \_\        )ART"};

}  // namespace Art
