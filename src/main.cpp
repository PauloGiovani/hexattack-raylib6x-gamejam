// ----------------------------------------------------------------------------
// src/main.cpp
//
// Based on raylib gamejam template (C), created by Ramon Santamaria
// ----------------------------------------------------------------------------

#include "raylib.h"

#include "Core/GameConfig.h"
#include "Game.h"

// ----------------------------------------------------------------------------

// Emscripten library, for web WASM
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// ----------------------------------------------------------------------------
// Global Variables Definition
// ----------------------------------------------------------------------------

static Game game;

// ----------------------------------------------------------------------------
// Module Functions Declaration
// ----------------------------------------------------------------------------

static void UpdateDrawFrame(void);

// ----------------------------------------------------------------------------
// Program main entry point
// ----------------------------------------------------------------------------

int main(void) {

// Disable raylib trace log messages
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);
#endif

    // ------------------------------------------------------------------------
    // Initialization
    // ------------------------------------------------------------------------

    // Initialize raylib window
    InitWindow(
        Config::ScreenWidth,
        Config::ScreenHeight,
        Config::WindowTitle
    );

    // Disable ESC key to close window
    SetExitKey(KEY_NULL); 

    // Initialize game
    game.Initialize();

#if defined(PLATFORM_WEB)

    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);

#else

    // Game FPS
    SetTargetFPS(60);

    // Game loop
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

#endif

    // ------------------------------------------------------------------------
    // De-Initialization
    // ------------------------------------------------------------------------

    // Shutdown game
    game.Shutdown();

    // Close window and OpenGL context
    CloseWindow();

    return 0;
}

// ----------------------------------------------------------------------------
// Module Functions Definition
// ----------------------------------------------------------------------------

// Update and Draw one frame
static void UpdateDrawFrame(void) {
    game.Update();
    game.Render();
}
