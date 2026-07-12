#pragma once

#include "raylib.h"

// Textures
enum class TextureId {
    PlayerBase = 0,
    PlayerCannon,
    PlayerBullet,
    EnemyBase01, // Top-Left
    EnemyBase02, // Top-Right
    EnemyBase03, // Bottom-Right
    EnemyBase04, // Bottom-Left
    Enemy,
    Hexagon,
    Explosion
};


// Sounds
enum class SoundId {
    PlayerShoot = 0,
    GameOver,
    Explosion,
    HexagonHit
};


// Musics
enum class MusicId {
    Background = 0
};


// Fonts
enum class FontId {
    Hud = 0
};


class Assets {
public:
    Assets();
    ~Assets();

    void Load();
    void Unload();

    // Getters
    Texture2D& GetTexture(TextureId id);
    Sound& GetSound(SoundId id);
    Music& GetMusic(MusicId id);
    Font& GetFont(FontId id);

private:
    // Player
    Texture2D playerBase {};
    Texture2D playerCannon {};
    Texture2D playerBullet {};

    // Enemy
    Texture2D enemyBase01 {};
    Texture2D enemyBase02 {};
    Texture2D enemyBase03 {};
    Texture2D enemyBase04 {};
    Texture2D enemy {};

    // Hexagon
    Texture2D hexagon {};

    // Explosion
    Texture2D explosion {};

    // Sounds
    Sound playerShootSound {};
    Sound gameOverSound {};
    Sound explosionSound {};
    Sound hexagonHitSound {};
    
    // Musics
    Music backgroundMusic {};

    // Fonts
    Font hudFont {};
};
