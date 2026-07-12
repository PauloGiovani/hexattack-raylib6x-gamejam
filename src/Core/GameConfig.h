#pragma once

#include <raylib.h>

// Constants
namespace Config {
    // Window
    constexpr int ScreenWidth = 720;
    constexpr int ScreenHeight = 720;
    constexpr const char WindowTitle[] = "Hex Attack";

    // Game
    constexpr float GameHexagonSpawnInterval = 3.0f;
    constexpr float GameHexagonSpawnIntervalDecrement = 0.5f;
    constexpr float GameHexagonSpawnIntervalLimit = 0.2f;

    constexpr float GameOverDelayTimer = 0.0f;
    constexpr float GameOverMaxDelay = 2.5f;

    constexpr float GameHexagonSpawnPadding = 50.0f;
    constexpr float GameHexagonBaseSpeed = 150.0f;
    constexpr float GameHexagonSpeedIncrease = 25.0f;

    constexpr float GameMenuTitleSize = 80.0f;
    constexpr float GameMenuTitleVerticalPosition = 150.0f;

    constexpr float GameMenuTextSize = 40.0f;
    constexpr float GameMenuTextInstructions = 24.0f;
    constexpr float GameMenuTextCredits = 24.0f;
    constexpr float GameMenuTextBack = 20.0f;
    constexpr float GameMenuTextSpacing = 2.0f;

    constexpr float GamePlayerRadius = 40.0f;
    constexpr float GameHexagonRadius = 35.0f;
    constexpr float GameEnemyRadius = 80.0f;
    constexpr float GameCornerOffsetX = 50.0f;
    constexpr float GameCornerOffsetY = 30.0f;

    constexpr float GameWaveFontSize = 60.0f;
    constexpr float GameTimeFontSize = 28.0f;
    constexpr float GameFontSpacing = 8.0f;

    constexpr float GameTimeBarWidth = 250.0f;
    constexpr float GameTimeBarHeight = 18.0f;

    // Game State
    constexpr float GameStateElapsedTime = 0.0f;
    constexpr float GameStateCurrentWave = 1.0f;
    constexpr float GameStateWaveDuration = 30.0f;
    constexpr float GameStateWaveTimer = 30.0f;

    // Collision Manager
    constexpr float CollisionPlayerRadius = 40.0f;
    constexpr float CollisionBulletRadius = 10.0f;
    constexpr float CollisionHexagonRadius = 30.0f;
    constexpr float CollisionHexagonPadding = 4.0f;
    constexpr int CollisionIterationSteps  = 20;
    constexpr float CollisionEnemyPlatformOffsetX = 50.0f;
    constexpr float CollisionEnemyPlatformOffsetY = 80.0f;
    constexpr float CollisionEnemyPlatformRadius = 80.0f;
    constexpr float CollisionEnemyPlatformHexagonRadius = 35.0f;

    // Player
    constexpr float CannonRotationSpeed = 180.0f;
    constexpr float CannonLength = 40.0f;
    constexpr float CannonScale = 1.0f;

    // Bullet
    constexpr float BulletSpeed = 400.0f;

    // Explosion
    constexpr float ExplosionScale = 1.0f;

    // Hexagon
    constexpr float HexagonScale = 0.0f;
    constexpr float HexagonTargetScale = 0.5f;
    constexpr float HexagonGrowthSpeed = 2.5f;

    // Corner Enemies
    constexpr float EnemyOffsetX = 55.0f;
    constexpr float EnemyOffsetY = 42.0f;
    constexpr float EnemyScale = 1.0f;

    // Enemy Platforms
    constexpr float PlatformOffsetX = 50.0f;
    constexpr float PlatformOffsetY = 30.0f;

    // Entity
    constexpr float EntityHeading = 0.0f;
    constexpr float EntityAcceleration = 0.0f;

    // Debug
    constexpr bool Debug = false;
}
