#pragma once

#include "raylib.h"
#include <memory>
#include <cmath>
#include "../../Core/GameConfig.h"
#include "../../Core/Animator.h"
#include "../SpriteEntity.h"


enum class PlayerState {
    Idle = 0,
    Shoot
};


class Player : public SpriteEntity {
public:

    Player();
    virtual ~Player() override;

    void Initialize();
    void Reset();

    void PlayShootAnimation();
    Vector2 GetCannonPosition() const;

    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

private:

    float rotationSpeed = Config::CannonRotationSpeed;
    // Animation manager
    std::unique_ptr<Animator<PlayerState>> cannonAnimator;

};
