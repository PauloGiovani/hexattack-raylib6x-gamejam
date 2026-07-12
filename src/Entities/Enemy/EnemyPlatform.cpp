#include "EnemyPlatform.h"


EnemyPlatform::EnemyPlatform() {
    active = false; 
}


EnemyPlatform::~EnemyPlatform() {}


// Initialize platform using a corner index
// 0: Top-Left, 1: Top-Right, 2: Bottom-Right, 3: Bottom-Left
void EnemyPlatform::Initialize(int cornerIndex) {

    float offsetX = Config::PlatformOffsetX;
    float offsetY = Config::PlatformOffsetY;

    // Set the correct texture and position
    switch (cornerIndex) {
        // Top-Left
        case 0:
            position = { 
                offsetX, 
                offsetY 
            };
            baseTexture = TextureId::EnemyBase01;
            break;
        // Top-Right
        case 1: 
            position = { 
                Config::ScreenWidth - offsetX, 
                offsetY 
            };
            baseTexture = TextureId::EnemyBase02;
            break;
        // Bottom-Right
        case 2: 
            position = { 
                Config::ScreenWidth - offsetX, 
                Config::ScreenHeight - offsetY 
            };
            baseTexture = TextureId::EnemyBase03;
            break;
        // Bottom-Left
        case 3:
            position = { 
                offsetX, 
                Config::ScreenHeight - offsetY 
            };
            baseTexture = TextureId::EnemyBase04;
            break;
        // Default
        default:
            position = { 
                offsetX, 
                offsetY 
            };
            baseTexture = TextureId::EnemyBase01;
            break;
    }
    // Set platform heading
    heading = 0.0f;
}


void EnemyPlatform::Reset() {
    active = true;
}


void EnemyPlatform::Draw() {
    if (!active || assets == nullptr) {
        return;
    }
    DrawSprite(baseTexture);
}
