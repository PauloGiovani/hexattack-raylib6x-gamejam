#include "Player.h"


Player::Player() {
    type = EntityType::Player;
}


Player::~Player() {}


void Player::Initialize() {

    if (assets != nullptr) {

        // Get cannon texture
        Texture2D& textureCannon = assets->GetTexture(TextureId::PlayerCannon);
        
        // Initialize animator
        cannonAnimator = std::make_unique<Animator<PlayerState>>(textureCannon);

        // Frame dimensions
        int frameCount = 8;
        int frameW = textureCannon.width / frameCount;
        int frameH = textureCannon.height;

        // Idle animation
        cannonAnimator->AddAnimation(PlayerState::Idle, Animation{
            1,
            1.0f,
            0,
            0.0f,
            frameW,
            frameH,
            0,
            true
        });

        // Shoot animation
        cannonAnimator->AddAnimation(PlayerState::Shoot, Animation{
            7,
            0.05f,
            0,
            0.9f,
            frameW,
            frameH,
            0,
            false 
        });

        // Set player initial state
        cannonAnimator->SetState(PlayerState::Idle);

    }

    Reset();
}


void Player::Reset() {
    position.x = static_cast<float>(Config::ScreenWidth) / 2.0f;
    position.y = static_cast<float>(Config::ScreenHeight) / 2.0f;
    heading = 0.0f;
    active = true;

    // Set player initial state
    if (cannonAnimator) {
        cannonAnimator->SetState(PlayerState::Idle);
    }
}


void Player::PlayShootAnimation() {
    if (cannonAnimator) {
        cannonAnimator->SetState(PlayerState::Shoot);
    }
}


Vector2 Player::GetCannonPosition() const {
    float cannonLength = Config::CannonLength;
    float rad = heading * DEG2RAD; 

    // Origin
    return Vector2 {
        position.x + cosf(rad) * cannonLength,
        position.y + sinf(rad) * cannonLength
    };
}


void Player::Update(float deltaTime) {

    if (!active) {
        return;
    }

    // Get mouse movement
    Vector2 mouseDelta = GetMouseDelta();

    // Calculate rotation speed
    if (mouseDelta.x != 0.0f || 
        mouseDelta.y != 0.0f || 
        IsMouseButtonDown(MOUSE_BUTTON_LEFT)
    ) {
        Vector2 mousePos = GetMousePosition();
        heading = atan2f(mousePos.y - position.y, mousePos.x - position.x) * RAD2DEG;
    }
    
    // Left rotation
    if (IsKeyDown(KEY_LEFT)) {
        heading -= rotationSpeed * deltaTime;
    }

    // Right rotation
    if (IsKeyDown(KEY_RIGHT)) {
        heading += rotationSpeed * deltaTime;
    }

    // Angle safety
    if (heading < 0.0f) {
        heading += 360.0f;
    }
    if (heading >= 360.0f) {
        heading -= 360.0f;
    }

    // Update animator
    if (cannonAnimator) {
        cannonAnimator->Update(deltaTime);
        // Reset cannon animation to idle state
        if (cannonAnimator->GetState() == PlayerState::Shoot && cannonAnimator->IsAnimationFinished()) {
            cannonAnimator->SetState(PlayerState::Idle);
        }
    }
}


void Player::Draw() {
    
    if (!active || assets == nullptr) {
        return;
    }

    // Save current heading
    float currentHeading = heading;

    // Base heading ponting left
    heading = 0.0f; 

    // Draw hexagonal base
    DrawSprite(TextureId::PlayerBase);

    // Get current heading
    heading = currentHeading;

    // Cannon animation
    if (cannonAnimator) {
        float scale = Config::CannonScale;
        // Origin
        Vector2 customOrigin = {
            (cannonAnimator->GetFrameSize().x * scale) / 2.0f,
            (cannonAnimator->GetFrameSize().y * scale) / 3.0f
        };
        // Draw cannon
        cannonAnimator->DrawWithRotation(position, heading - 90.0f, customOrigin, scale);
    }
}
