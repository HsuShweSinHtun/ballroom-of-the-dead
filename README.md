# Ballroom of the Dead (Text RPG)

A text-based zombie survival RPG written in C++17 using Object-Oriented Programming.
It runs in the terminal and uses colors, bordered panels, arrow-key menus, a typewriter
text effect and a state machine. No external libraries. Works on macOS, Linux and Windows.

## Build and run

**macOS / Linux** (open Terminal in this folder):

```
make
./ballroom
```

No `make`? Compile every file in `src/` together (do not compile only one file, or you
get "symbol(s) not found" linker errors):

```
clang++ -std=c++17 -Iinclude src/*.cpp -o ballroom
./ballroom
```

(`g++` works the same way.) **Windows:** use Windows Terminal with MinGW
(`g++ -std=c++17 -Iinclude src/*.cpp -o ballroom.exe`) or CMake
(`cmake -S . -B build` then `cmake --build build`, and run it from the project folder).

Always run the game from the project folder, because it reads and writes `data/`.

### Terminal size

The game needs a terminal of at least **90 columns x 30 rows**. It asks the terminal to
resize itself, but some terminals ignore that. If the window is too small, the game pauses
and shows a message until you make the window bigger.

## Controls

| Key | Action |
|---|---|
| Up / Down (or W / S) | Move the cursor |
| Left / Right (or A / D) | Move between columns |
| Enter or Space | Choose / skip text |
| Esc or Backspace | Go back |

## Game screens (each one is a state)

| State | Purpose |
|---|---|
| `TitleState` | Main menu with the animated title |
| `LoginState` | Type a username: registers a new player or loads a saved one |
| `HubState` | The menu between fights: Fight, Shop, Inventory, Status, Save & Quit |
| `BattleState` | One stage of turn-based combat with typewriter messages |
| `ShopState` | Buy weapons, medkits, armor plates and repairs |
| `InventoryState` | Look at weapons and use medkits |
| `StatusState` | Character sheet for the current run |
| `InfoState` | Scrollable pages: leaderboard, past reports, how to play |
| `GameOverState` | Result, automatic report and leaderboard |

## Project structure

```
include/                 headers          src/                 source files
  Game.h  GameData.h                        Game.cpp  GameData.cpp  main.cpp
  actor/   Player.h  Zombie.h               Player.cpp  Zombie.cpp
  items/   Weapon.h  Inventory.h            Weapon.cpp  Inventory.cpp
  inputs/  Keyboard.h  Keys.h               Keyboard.cpp
  states/  IState.h  StateMachine.h ...     StateMachine.cpp  *State.cpp
  ui/      Console.h  Selector.h  Art.h     Console.cpp  Selector.cpp
  utilities/ FileManager.h ReportGenerator.h GameRules.h MessageLog.h
             Typewriter.h Timer.h Terminal.h Utils.h
data/                    weapons.txt, players.txt, game_history.txt, leaderboard.txt, reports.txt
```

## How it matches the course requirements

* **Classes and objects:** `Player`, `Zombie` (+ `Walker`, `Runner`, `Brute`, `BallroomKing`),
  `Weapon`, `Inventory`, `Game`, `ReportGenerator`, `FileManager`, plus the engine classes
  `Console`, `Keyboard`, `StateMachine`, `Selector`, `Typewriter`, `MessageLog`.
* **OOP concepts:** encapsulation, constructors, inheritance and polymorphism (`Zombie::attack`
  is virtual, `IState` is an interface for every screen), composition, operator overloading
  (`LeaderboardEntry::operator>`), `unique_ptr`, `std::function` callbacks.
* **Functions:** start/end game (`Game::Run`, `GameOverState::OnEnter`), main menu (`TitleState`),
  player actions and zombie attacks (`BattleState`), damage and score (`GameRules.h`),
  inventory and resources (`Inventory`, `ShopState`), reports (`ReportGenerator`).
* **Text files** (`FileManager`): read `weapons.txt`; search/update/add records in `players.txt`;
  append to `game_history.txt` and `reports.txt`; read, sort and write `leaderboard.txt`.
  Progress is saved after every stage and when you choose Save & Quit.

## Balancing

Zombie stats: `src/Zombie.cpp`. Weapons: edit `data/weapons.txt` (no code change needed).
Prices and bonuses: `include/utilities/GameRules.h`. Stage layouts: `BattleState::BuildStage`.
