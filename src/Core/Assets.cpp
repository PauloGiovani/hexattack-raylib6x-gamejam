#include "Assets.h"


Assets::Assets() {}


Assets::~Assets() {}


// Load assets
void Assets::Load() {

    // Player
    playerBase = LoadTexture("resources/images/player-base.png");
    playerCannon = LoadTexture("resources/images/player-cannon.png");
    playerBullet = LoadTexture("resources/images/player-bullet.png");
    playerShootSound = LoadSound("resources/audio/player-shoot.mp3");
    SetSoundVolume(playerShootSound, 0.9f);

    // Enemy
    enemyBase01 = LoadTexture("resources/images/enemy-base-01.png");
    enemyBase02 = LoadTexture("resources/images/enemy-base-02.png");
    enemyBase03 = LoadTexture("resources/images/enemy-base-03.png");
    enemyBase04 = LoadTexture("resources/images/enemy-base-04.png");
    enemy = LoadTexture("resources/images/enemy.png");

    // Hexagon
    hexagon = LoadTexture("resources/images/hexagon.png");
    hexagonHitSound = LoadSound("resources/audio/hexagon-hit.mp3");
    SetSoundVolume(hexagonHitSound, 0.18f);

    // Explosion
    explosion = LoadTexture("resources/images/explosion.png");
    explosionSound = LoadSound("resources/audio/explosion.mp3");
    SetSoundVolume(explosionSound, 0.65f); 

    // Game Over
    gameOverSound = LoadSound("resources/audio/game-over.mp3");
    SetSoundVolume(gameOverSound, 0.8f); 

    // Music
    backgroundMusic = LoadMusicStream("resources/audio/background-music.ogg");

    SetMusicVolume(backgroundMusic, 0.3f);

    // Fonts
    hudFont = LoadFont("resources/fonts/hex-attack.fnt");
}


// Unload assets
void Assets::Unload() {
    // Textures
    UnloadTexture(playerBase);
    UnloadTexture(playerCannon);
    UnloadTexture(playerBullet);
    UnloadTexture(enemyBase01);
    UnloadTexture(enemyBase02);
    UnloadTexture(enemyBase03);
    UnloadTexture(enemyBase04);
    UnloadTexture(enemy);
    UnloadTexture(hexagon);
    UnloadTexture(explosion);

    // Sounds
    UnloadSound(playerShootSound);
    UnloadSound(gameOverSound);
    UnloadSound(explosionSound);
    UnloadSound(hexagonHitSound);
    UnloadMusicStream(backgroundMusic);

    // Fonts
    UnloadFont(hudFont);
}


// Get correct texture
Texture2D& Assets::GetTexture(TextureId id) {
    switch (id) {
        // Player
        case TextureId::PlayerBase:
            return playerBase;
        case TextureId::PlayerCannon:
            return playerCannon;
        case TextureId::PlayerBullet:
            return playerBullet;

        // Enemy platforms
        case TextureId::EnemyBase01:
            return enemyBase01;
        case TextureId::EnemyBase02:
            return enemyBase02;
        case TextureId::EnemyBase03:
            return enemyBase03;
        case TextureId::EnemyBase04:
            return enemyBase04;

        // Enemy
        case TextureId::Enemy:
            return enemy;

        // Hexagon
        case TextureId::Hexagon:
            return hexagon;

        // Explosion
        case TextureId::Explosion:
            return explosion;
    }
}


// Get sound
Sound& Assets::GetSound(SoundId id) {
    switch (id) {
        case SoundId::PlayerShoot:
            return playerShootSound;
        case SoundId::GameOver:
            return gameOverSound;
        case SoundId::Explosion:
            return explosionSound;
        case SoundId::HexagonHit:
            return hexagonHitSound;
    }
}


// Get music
Music& Assets::GetMusic(MusicId id) {
    switch (id) {
        case MusicId::Background:
            return backgroundMusic;
    }
}


// Get font
Font& Assets::GetFont(FontId id) {
    switch (id) {
        case FontId::Hud:
            return hudFont;
    }
}
