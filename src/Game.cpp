#include "Game.h"


Game::Game() {}


Game::~Game() {}


void Game::Initialize() {

    InitAudioDevice();

    resourceManager.Initialize();
    Assets& assets = resourceManager.GetAssets();

    background.Initialize();
    player.SetAssets(&assets);
    player.Initialize();

    for (int i = 0; i < 4; i++) {
        enemyPlatforms[i].SetAssets(&assets);
        enemyPlatforms[i].Initialize(i);

        enemies[i].SetAssets(&assets);
        enemies[i].Initialize(i);
    }

    // Render target
    target = LoadRenderTexture(
        Config::ScreenWidth,
        Config::ScreenHeight
    );

    SetTextureFilter(
        target.texture,
        TEXTURE_FILTER_BILINEAR
    );

    // Reset game state
    gameState.Reset();
    Reset();
}


void Game::Shutdown() {
    UnloadRenderTexture(target);
    resourceManager.Shutdown();
    CloseAudioDevice();
}


void Game::Reset() {

    gameState.Reset();

    player.Reset();

    for (int i = 0; i < 4; i++) {
        enemyPlatforms[i].Reset();
        enemies[i].Reset();
    }

    bullets.clear();
    hexagons.clear();
    hexagonSpawnTimer = 0.0f;
    hexagonSpawnInterval = Config::GameHexagonSpawnInterval;
    explosions.clear();

    gameOverSoundPlayed = false;
    gameOverDelayTimer = Config::GameOverDelayTimer;
}



void Game::Update() {

    float deltaTime = GetFrameTime();

    Music& bgMusic = resourceManager.GetAssets().GetMusic(MusicId::Background);
    UpdateMusicStream(bgMusic);

    if (currentScreen == GameScreen::Playing) {
        UpdatePlaying(deltaTime);
    }
}


// Play game
void Game::UpdatePlaying(float deltaTime) {

    Music& bgMusic = resourceManager.GetAssets().GetMusic(MusicId::Background);
    gameState.AddElapsedTime(deltaTime);

    if (!gameState.IsPlaying()) {

        // Game Over
        if (gameState.IsGameOver()) {
            
            if (!gameOverSoundPlayed) {
                if (IsMusicStreamPlaying(bgMusic)) {
                    StopMusicStream(bgMusic);
                }
                PlaySound(resourceManager.GetAssets().GetSound(SoundId::GameOver));
                gameOverSoundPlayed = true;
            }

            gameOverDelayTimer += deltaTime;

            if (gameOverDelayTimer < Config::GameOverMaxDelay) {
                return; 
            }

        }
        
        // ENTER, to play again
        if (IsKeyPressed(KEY_ENTER)) {
            Reset();
            PlayMusicStream(bgMusic);
        }

        // ESC, to back to game menu
        if (gameState.IsGameOver() && IsKeyPressed(KEY_ESCAPE)) {
            currentScreen = GameScreen::Menu;
        }

        return;
    }

    // Wave update
    gameState.DecreaseWaveTimer(deltaTime);

    // Next wave
    if (gameState.GetWaveTimer() <= 0.0f) {
        gameState.NextWave();
        hexagonSpawnInterval -= Config::GameHexagonSpawnIntervalDecrement; 
        if (hexagonSpawnInterval < Config::GameHexagonSpawnIntervalLimit) {
            hexagonSpawnInterval = Config::GameHexagonSpawnIntervalLimit;
        }
    }

    // Hexagon spawn
    hexagonSpawnTimer += deltaTime;

    if (hexagonSpawnTimer >= hexagonSpawnInterval) {
        hexagonSpawnTimer = 0.0f;
        
        // (0: Top, 1: Right, 2: Bottom, 3: Left)
        int edge = GetRandomValue(0, 3);
        Vector2 startPos;
        float padding = Config::GameHexagonSpawnPadding;

        if (edge == 0) {
            startPos = { 
                static_cast<float>(
                    GetRandomValue(padding, Config::ScreenWidth - padding)
                ), 
                padding 
            };
        }

        if (edge == 1) {
            startPos = { 
                Config::ScreenWidth - padding, 
                static_cast<float>(
                    GetRandomValue(padding, Config::ScreenHeight - padding)
                ) 
            };
        }

        if (edge == 2) {
            startPos = { 
                static_cast<float>(
                    GetRandomValue(padding, Config::ScreenWidth - padding)
                ), 
                Config::ScreenHeight - padding 
            };
        }

        if (edge == 3) {
            startPos = { 
                padding, 
                static_cast<float>(
                    GetRandomValue(padding, Config::ScreenHeight - padding)
                ) 
            };
        }

        float baseSpeed = Config::GameHexagonBaseSpeed;
        float speedIncreasePerWave = Config::GameHexagonSpeedIncrease;
        float currentSpeed = baseSpeed + (gameState.GetCurrentWave() - 1) * speedIncreasePerWave;

        Hexagon newHexagon;
        newHexagon.SetAssets(&resourceManager.GetAssets());
        newHexagon.Initialize(startPos, currentSpeed);
        hexagons.push_back(newHexagon);
    }

    // Background shader
    background.Update(deltaTime);

    player.Update(deltaTime);

    for (int i = 0; i < 4; i++) {
        enemies[i].Update(deltaTime);
    }

    // SPACE or LEFT mouse button to shoot
    if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(
            resourceManager.GetAssets().GetSound(SoundId::PlayerShoot)
        );

        player.PlayShootAnimation();

        Bullet newBullet;
        newBullet.SetAssets(&resourceManager.GetAssets()); 
        newBullet.SetPosition(player.GetCannonPosition());
        newBullet.SetHeading(player.GetHeading());
        newBullet.Initialize();
        bullets.push_back(newBullet);
    }

    for (auto& bullet : bullets) {

        if (bullet.IsActive()) {
            bullet.Update(deltaTime);
            Vector2 pos = bullet.GetPosition();
            if (pos.x < 0 || pos.x > Config::ScreenWidth || 
                pos.y < 0 || pos.y > Config::ScreenHeight) {
                bullet.SetActive(false);
            }
        }
    }

    // Remove inative bullets
    bullets.erase(
        std::remove_if(
            bullets.begin(), 
            bullets.end(),
            [](const Bullet& b) { 
                return !b.IsActive(); 
            }
        ), 
        bullets.end()
    );

    for (auto& hexagon : hexagons) {

        if (hexagon.IsActive()) {
            hexagon.Update(deltaTime);
            Vector2 pos = hexagon.GetPosition();
            if (!hexagon.IsLinked() && (
                pos.x < -200 || pos.x > Config::ScreenWidth + 200 || 
                pos.y < -200 || pos.y > Config::ScreenHeight + 200)
            ) {
                hexagon.SetActive(false);
            }
        }
    }

    for (auto& explosion : explosions) {
        if (explosion.IsActive()) {
            explosion.Update(deltaTime);
        }
    }

    // Remove inactive explosions
    explosions.erase(
        std::remove_if(
            explosions.begin(), 
            explosions.end(), 
            [](const Explosion& e) { 
                return !e.IsActive(); 
            }
        ), 
        explosions.end()
    );

    // Bullet x Hexagon
    collisionManager.CheckBulletHexagonCollisions(
        bullets, 
        hexagons, 
        explosions, 
        &resourceManager.GetAssets()
    );

    // Hexagon x Player
    collisionManager.CheckHexagonLinking(
        hexagons, 
        player.GetPosition(), 
        &resourceManager.GetAssets()
    );

    // Hexagon x Corner
    collisionManager.CheckHexagonEnemyCollisions(hexagons, gameState);
    
    // ESC, to return back to game menu
    if (gameState.IsGameOver() && IsKeyPressed(KEY_ESCAPE)) {
        if (IsMusicStreamPlaying(bgMusic)) {
            StopMusicStream(bgMusic);
        }
        currentScreen = GameScreen::Menu;
    }
}


// ---


// UI Helpers

void Game::DrawTextCentered(
    Font font, 
    const char* text, 
    float y, 
    float fontSize, 
    float spacing, 
    Color color
) {

    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);

    DrawTextEx(
        font,
        text,
        Vector2 {
            (Config::ScreenWidth - size.x) * 0.5f,
            y
        },
        fontSize,
        spacing,
        color
    );

}


bool Game::DrawButtonCentered(
    Font font, 
    const char* text, 
    float y, 
    float fontSize, 
    float spacing, 
    Color defaultColor, 
    Color hoverColor
) {
    
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);
    
    Rectangle btnBounds = {
        (Config::ScreenWidth - size.x) * 0.5f,
        y,
        size.x,
        size.y
    };
    
    Vector2 mousePos = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mousePos, btnBounds);
    bool isClicked = false;
    
    if (isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        isClicked = true;
    }
    
    DrawTextEx(
        font,
        text,
        Vector2 { btnBounds.x, btnBounds.y },
        fontSize,
        spacing,
        isHovering ? hoverColor : defaultColor
    );
    
    return isClicked;
}


void Game::DrawMenu() {

    Font font = resourceManager.GetAssets().GetFont(FontId::Hud);
    
    // Draw background image
    // DrawTexture(
    //     resourceManager.GetAssets().GetTexture(TextureId::TitleScreen), 
    //     0, 
    //     0, 
    //     WHITE
    // );

    DrawTextCentered(
        font, 
        "HEX ATTACK", 
        Config::GameMenuTitleVerticalPosition, 
        Config::GameMenuTitleSize, 
        Config::GameMenuTextSpacing, 
        YELLOW
    );

    if (DrawButtonCentered(
            font, 
            "PLAY GAME", 
            350.0f, 
            Config::GameMenuTextSize, 
            Config::GameMenuTextSpacing, 
            LIGHTGRAY, 
            YELLOW
        )
    ) {

        Reset();
        PlayMusicStream(
            resourceManager.GetAssets().GetMusic(MusicId::Background)
        );
        currentScreen = GameScreen::Playing;

    }

    if (DrawButtonCentered(
            font, 
            "INSTRUCTIONS", 
            420.0f, 
            Config::GameMenuTextSize, 
            Config::GameMenuTextSpacing, 
            LIGHTGRAY, 
            YELLOW
        )
    ) {
        currentScreen = GameScreen::Instructions;
    }

    if (DrawButtonCentered(
            font, 
            "CREDITS", 
            490.0f, 
            Config::GameMenuTextSize, 
            Config::GameMenuTextSpacing, 
            LIGHTGRAY, 
            YELLOW
        )
    ) {
        currentScreen = GameScreen::Credits;
    }

}


void Game::DrawInstructions() {

    Font font = resourceManager.GetAssets().GetFont(FontId::Hud);

    DrawTextCentered(
        font, 
        "How to Play", 
        Config::GameMenuTitleVerticalPosition, 
        Config::GameMenuTitleSize, 
        Config::GameMenuTextSpacing, 
        YELLOW
    );
    
    const char* text = "Keyboard: Press the Left and Right Arrow\nkeys to rotate the cannon. Press the \nSpacebar to fire. Mouse: Move the mouse \nto aim the cannon. Click the Left Mouse \nButton to shoot. Destroy all the flying \nhexagons before they connect to the \ncentral platform and build a bridge to the \nenemy in one of the screen's corners. \nCan you survive every wave?";
    
    DrawTextEx(
        font, 
        text, 
        Vector2{
            Config::ScreenWidth * 0.5 - 300, 
            300
        }, 
        Config::GameMenuTextInstructions, 
        Config::GameMenuTextSpacing, 
        WHITE
    );
    
    Vector2 backSize = MeasureTextEx(
        font, 
        "Back to Menu", 
        Config::GameMenuTextBack, 
        Config::GameMenuTextSpacing
    );

    Rectangle backBounds = {
        Config::ScreenWidth - backSize.x - 40.0f, 
        Config::ScreenHeight - 60.0f, 
        backSize.x, 
        backSize.y 
    };

    bool isHovering = CheckCollisionPointRec(GetMousePosition(), backBounds);
    
    if ((isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = GameScreen::Menu;
    }
    
    DrawTextEx(
        font, 
        "Back to Menu", 
        Vector2{ 
            backBounds.x, 
            backBounds.y 
        }, 
        Config::GameMenuTextBack, 
        Config::GameMenuTextSpacing, 
        isHovering ? YELLOW : LIGHTGRAY
    );
}


void Game::DrawCredits() {

    Font font = resourceManager.GetAssets().GetFont(FontId::Hud);

    DrawTextCentered(
        font, 
        "Game Credits", 
        Config::GameMenuTitleVerticalPosition, 
        Config::GameMenuTitleSize, 
        Config::GameMenuTextSpacing, 
        YELLOW
    );

    DrawTextCentered(
        font, 
        "Raylib 6.x Gamejam", 
        Config::GameMenuTitleVerticalPosition + 100.0f, 
        Config::GameMenuTextSize, 
        Config::GameMenuTextSpacing, 
        WHITE
    );
    
    const char* text = "Game created by Paulo Giovani\n\nAssets by Itch.io, Pixabay and DaFont:\n\n- Zintoki: Ground Shaker\n- QwerryAnimation: Robot D-8080 Enemy\n- Ansimuz: Explosion Animations Pack\n- Biww: Small Explosion Blast Impact\n- Seventhpaw: Game Over Halo\n- Ribhav Agrawal: Metal Beaten SFX\n- Game Font: Nineteen Ninety Seven";
    
    DrawTextEx(
        font, 
        text, 
        Vector2{
            Config::ScreenWidth * 0.5 - 300, 
            330
        }, 
        Config::GameMenuTextCredits, 
        Config::GameMenuTextSpacing, 
        WHITE
    );

    Vector2 backSize = MeasureTextEx(
        font, 
        "Back to Menu", 
        Config::GameMenuTextBack, 
        Config::GameMenuTextSpacing
    );

    Rectangle backBounds = {
        Config::ScreenWidth - backSize.x - 40.0f, 
        Config::ScreenHeight - 60.0f, 
        backSize.x, 
        backSize.y 
    };

    bool isHovering = CheckCollisionPointRec(GetMousePosition(), backBounds);
    
    if ((isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = GameScreen::Menu;
    }

    DrawTextEx(
        font, 
        "Back to Menu", 
        Vector2{ 
            backBounds.x, 
            backBounds.y 
        }, 
        Config::GameMenuTextBack, 
        Config::GameMenuTextSpacing, 
        isHovering ? YELLOW : LIGHTGRAY
    );

}


void Game::DrawPlaying() {

    for (int i = 0; i < 4; i++) {
        enemyPlatforms[i].Draw();
    }

    for (int i = 0; i < 4; i++) {
        enemies[i].Draw();
    }

    player.Draw();

    for (auto& hexagon : hexagons) {
        if (hexagon.IsActive()) {
            hexagon.Draw();
        }
    }

    for (auto& bullet : bullets) {
        if (bullet.IsActive()) {
            bullet.Draw();
        }
    }

    for (auto& explosion : explosions) {
        if (explosion.IsActive()) {
            explosion.Draw();
        }
    }

    if (!gameState.IsGameOver()) {

        // HUD

        Font font = resourceManager.GetAssets().GetFont(FontId::Hud);
        float waveFontSize = Config::GameWaveFontSize;
        float timeFontSize = Config::GameTimeFontSize;
        float fontSpacing = Config::GameFontSpacing;

        const char* waveText = TextFormat("WAVE %i", gameState.GetCurrentWave());

        Vector2 waveTextSize = MeasureTextEx(
            font, 
            waveText, 
            waveFontSize, 
            fontSpacing
        );
        
        DrawTextEx(
            font, 
            waveText, 
            Vector2{ 
                (Config::ScreenWidth - waveTextSize.x) * 0.5f, 
                10.0f 
            }, 
            waveFontSize, 
            fontSpacing, 
            LIGHTGRAY
        );

        float barWidth = Config::GameTimeBarWidth;
        float barHeight = Config::GameTimeBarHeight;
        float barX = (Config::ScreenWidth - barWidth) * 0.5f + 60.0f;
        float barY = Config::ScreenHeight - 40.0f;

        Color barWhite = Color { 235, 235, 235, 255 };
        Color barRed = Color { 230, 0, 0, 255 };
        Color barYellow = Color { 255, 255, 0, 255 };

        float timeRatio = gameState.GetWaveTimer() / gameState.GetWaveDuration();

        DrawRectangle(barX - 1, barY - 1, barWidth + 2, barHeight + 2, barWhite);
        DrawRectangle(barX, barY, barWidth, barHeight, barRed);
        DrawRectangle(barX, barY, barWidth * timeRatio, barHeight, barYellow);

        const char* timeText = "TIME";

        Color textWhite = Color { 235, 235, 235, 255 };

        Vector2 timeTextSize = MeasureTextEx(font, timeText, timeFontSize, fontSpacing);

        DrawTextEx(
            font, 
            timeText, 
            Vector2{ 
                barX - timeTextSize.x - 25.0f, 
                barY + (barHeight - timeTextSize.y) * 0.5f 
            }, 
            timeFontSize, 
            fontSpacing, 
            textWhite
        );

        // FPS
        if (Config::Debug) {
            DrawFPS(20, 110);
        }

    }

    // ---

    // Game Over
    if (gameState.IsGameOver()) {

        if (gameOverDelayTimer >= Config::GameOverMaxDelay) {

            Font font = resourceManager.GetAssets().GetFont(FontId::Hud);

            DrawRectangle(
                0, 
                0, 
                Config::ScreenWidth, 
                Config::ScreenHeight,
                Fade(BLACK, 0.7f)
            );

            DrawTextCentered(
                font, 
                "Game Over", 
                Config::ScreenHeight * 0.5f - 100.0f,
                Config::GameMenuTitleSize, 
                Config::GameMenuTextSpacing, 
                YELLOW
            );

            if (DrawButtonCentered(
                font, 
                "Play Again", 
                Config::ScreenHeight * 0.5f + 20.0f, 
                Config::GameMenuTextSize, 
                Config::GameMenuTextSpacing, 
                LIGHTGRAY, 
                YELLOW
            )) {

                Reset();
                Music& bgMusic = resourceManager.GetAssets().GetMusic(MusicId::Background);
                PlayMusicStream(bgMusic);
            }

            Vector2 backSize = MeasureTextEx(
                font, 
                "Back to Menu", 
                Config::GameMenuTextBack, 
                Config::GameMenuTextSpacing
            );

            Rectangle backBounds = {
                Config::ScreenWidth - backSize.x - 40.0f, 
                Config::ScreenHeight - 60.0f, 
                backSize.x, 
                backSize.y 
            };

            bool isHovering = CheckCollisionPointRec(GetMousePosition(), backBounds);
            
            if (isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Music& bgMusic = resourceManager.GetAssets().GetMusic(MusicId::Background);
                if (IsMusicStreamPlaying(bgMusic)) {
                    StopMusicStream(bgMusic);
                }
                currentScreen = GameScreen::Menu;
            }

            DrawTextEx(
                font, 
                "Back to Menu", 
                Vector2{ 
                    backBounds.x, 
                    backBounds.y 
                }, 
                Config::GameMenuTextBack, 
                Config::GameMenuTextSpacing, 
                isHovering ? YELLOW : LIGHTGRAY
            );

        }

    }

}

// Render game
void Game::Render() {

    BeginTextureMode(target);

        ClearBackground(BLACK);

        background.Draw();

        switch (currentScreen) {
            // Menu
            case GameScreen::Menu:
                DrawMenu();
                break;
            // Instructions
            case GameScreen::Instructions:
                DrawInstructions();
                break;
            // Credits
            case GameScreen::Credits:
                DrawCredits();
                break;
            // Play Game
            case GameScreen::Playing:
                DrawPlaying();
                break;
        }

    EndTextureMode();

    // Render to screen
    BeginDrawing();

        ClearBackground(BLACK);

        DrawTexturePro(
            target.texture,
            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(target.texture.width),
                -static_cast<float>(target.texture.height)
            },
            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(target.texture.width),
                static_cast<float>(target.texture.height)
            },
            Vector2{0.0f, 0.0f},
            0.0f,
            WHITE
        );

    EndDrawing();

}
