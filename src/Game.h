#pragma once

#include "raylib.h"
#include <vector>
#include <algorithm>
#include "Core/GameConfig.h"
#include "Core/ResourceManager.h"
#include "Entities/Background/Background.h"
#include "Entities/Player/Player.h"
#include "Entities/Player/Bullet.h"
#include "Entities/Enemy/EnemyPlatform.h"
#include "Entities/Enemy/Enemy.h"
#include "Entities/Hexagon/Hexagon.h"
#include "Entities/Explosion/Explosion.h"
#include "Managers/GameState.h"
#include "Managers/CollisionManager.h"


enum class GameScreen {
    Menu = 0,
    Instructions,
    Credits,
    Playing
};


class Game {
public:

    Game();
    ~Game();

    void Initialize();
    void Shutdown();
    void Reset();

    void Update();
    void Render();

private:

    GameScreen currentScreen = GameScreen::Menu;

    // Resources and managers
    ResourceManager resourceManager;
    CollisionManager collisionManager;
    GameState gameState;

    // Entities
    Background background;
    Player player;
    std::vector<Bullet> bullets;
    EnemyPlatform enemyPlatforms[4];
    Enemy enemies[4];
    std::vector<Hexagon> hexagons;
    std::vector<Explosion> explosions;

    // Timers
    float hexagonSpawnTimer = 0.0f;
    float hexagonSpawnInterval = Config::GameHexagonSpawnInterval;

    // Render target
    RenderTexture2D target {};

    // Helpers

    void DrawTextCentered(
        Font font, 
        const char* text, 
        float y, 
        float fontSize, 
        float spacing, 
        Color color
    );

    bool DrawButtonCentered(
        Font font, 
        const char* text, 
        float y, 
        float fontSize, 
        float spacing, 
        Color defaultColor, 
        Color hoverColor
    );

    void DrawMenu();
    void DrawInstructions();
    void DrawCredits();
    void DrawPlaying();
    void UpdatePlaying(float deltaTime);

    bool gameOverSoundPlayed = false;
    float gameOverDelayTimer = Config::GameOverDelayTimer;
};
