#include "Explosion.h"


Explosion::Explosion() {
    active = false;
}


Explosion::~Explosion() {}


void Explosion::Initialize(Vector2 pos) {

    position = pos;

    if (assets != nullptr) {

        // Get explosion texture
        Texture2D& textureExplosion = assets->GetTexture(TextureId::Explosion);

        // Initialize animator
        animator = std::make_unique<Animator<ExplosionState>>(textureExplosion);

        // Frame dimensions 
        int frameCount = 8;
        int frameW = textureExplosion.width / frameCount;
        int frameH = textureExplosion.height;

        // Explosion animation
        animator->AddAnimation(ExplosionState::Play, Animation{
            frameCount,
            0.05f,
            0,
            0.0f,
            frameW,
            frameH,
            0,
            false
        });

        // Set explosion initial state
        animator->SetState(ExplosionState::Play);
    }

    active = true;
}


void Explosion::Update(float deltaTime) {
    if (!active) {
        return;
    }

    if (animator) {
        animator->Update(deltaTime);
        if (animator->IsAnimationFinished()) {
            active = false;
        }
    }
}


void Explosion::Draw() {

    if (!active || !animator) {
        return;
    }

    float scale = Config::ExplosionScale; 

    Vector2 origin = { 
        animator->GetFrameSize().x / 2.0f, 
        animator->GetFrameSize().y / 2.0f 
    };

    animator->DrawWithRotation(position, 0.0f, origin, scale);
}

