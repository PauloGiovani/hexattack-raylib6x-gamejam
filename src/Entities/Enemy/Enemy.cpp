#include "Enemy.h"
#include <cmath>


Enemy::Enemy() : corner(0) {
    type = EntityType::Player;
    active = false;
}


Enemy::~Enemy() {}


// Initialize enemy using a corner index
// 0: Top-Left, 1: Top-Right, 2: Bottom-Right, 3: Bottom-Left
void Enemy::Initialize(int cornerIndex) {

    corner = cornerIndex;

    if (assets != nullptr) {

        // Get enemy texture
        Texture2D& enemyTexture = assets->GetTexture(TextureId::Enemy);
        
        // Initialize animator
        enemyAnimator = std::make_unique<Animator<EnemyState>>(enemyTexture);

        // Frame dimensions
        int frameCount = 8;
        int frameW = enemyTexture.width / frameCount;
        int frameH = enemyTexture.height;

        // Idle animation
        enemyAnimator->AddAnimation(EnemyState::Idle, Animation{
            frameCount,
            0.4f,
            0,
            0.0f,
            frameW,
            frameH,
            0,
            true
        });

        // Set enemy initial state
        enemyAnimator->SetState(EnemyState::Idle);

    }

    Reset();
}


void Enemy::Reset() {

    float offsetX = Config::EnemyOffsetX;
    float offsetY = Config::EnemyOffsetX;

    // Enemy position
    switch (corner) {
        // Top-Left
        case 0: 
            position = { 
                offsetX, 
                offsetY + 5.0f 
            };
            flipX = true; 
            break;
        // Top-Right
        case 1: 
            position = { 
                Config::ScreenWidth - offsetX, 
                offsetY + 5.0f 
            }; 
            flipX = false;
            break;
        // Bottom-Right
        case 2: 
            position = { 
                Config::ScreenWidth - offsetX, 
                Config::ScreenHeight - offsetY - 3.0f 
            };
            flipX = false; 
            break;
        // Bottom-Left
        case 3: 
            position = { 
                offsetX, 
                Config::ScreenHeight - offsetY - 3.0f 
            }; 
            flipX = true;
            break;
    }

    heading = 0.0f; 
    active = true;
}


void Enemy::Update(float deltaTime) {
    if (!active) {
        return;
    }

    if (enemyAnimator) {
        enemyAnimator->Update(deltaTime);
    }
}


void Enemy::Draw() {
    
    if (!active || assets == nullptr) {
        return;
    }

    // Enemy animation
    if (enemyAnimator) {

        float scale = Config::EnemyScale; 

        Vector2 origin = { 
            enemyAnimator->GetFrameSize().x / 2.0f, 
            enemyAnimator->GetFrameSize().y / 2.0f 
        };

        enemyAnimator->DrawWithRotation(position, heading, origin, scale, flipX);
    }
}
