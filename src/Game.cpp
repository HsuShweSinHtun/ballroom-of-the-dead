#include "Game.h"
#include "states/TitleState.h"
#include "utilities/Terminal.h"

#include <chrono>
#include <cstdio>
#include <thread>

Game::Game()
    : m_bQuit(false),
      m_Context{m_Console, m_Keyboard, m_StateMachine, m_GameData, m_bQuit} {}

Game::~Game() { Terminal::Shutdown(); }

void Game::Run() {
    if (!Terminal::Init()) {
        printf("Ballroom of the Dead needs to run in a real terminal window.\n");
        printf("Open Terminal (macOS/Linux) or Windows Terminal and run it again.\n");
        return;
    }
    Terminal::RequestSize(Console::WIDTH + 2, Console::HEIGHT + 2);
    this_thread::sleep_for(chrono::milliseconds(150));

    m_GameData.Init();
    m_StateMachine.PushState(make_unique<TitleState>(m_Context));
    m_StateMachine.ApplyPending();

    while (!m_bQuit && !m_StateMachine.Empty()) {
        m_Keyboard.Poll();

        // Pause the game while the terminal window is too small
        if (m_Console.UpdateSize()) {
            IState* state = m_StateMachine.Current();
            state->ProcessInputs();
            state->Update();

            m_Console.ClearBuffer();
            state->Draw();
            m_Console.Draw();
        }

        m_StateMachine.ApplyPending();
        this_thread::sleep_for(chrono::milliseconds(33));  // about 30 frames per second
    }

    if (!m_StateMachine.Empty()) m_GameData.SaveProgress();
}
